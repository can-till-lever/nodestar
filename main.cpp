/* 
 * File:   main.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on September 17, 2014, 1:25 PM
 */

// helpful tutorials: http://www.webtoolkit.eu/widgets
// wt redmine: http://redmine.webtoolkit.eu/projects/wt

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Wt/WEnvironment>
#include <Wt/Dbo/backend/Postgres>

#include "SqlTraits.h"

#include "model/DbRecOrganization.h"
#include "model/DbRecIpAddress.h"

#include "utility/PopulateBasicIpAddresses.h"
#include "utility/ImportNetworkCsv.h"
#include "utility/ImportSmcXml.h"

#include "Cidr.hpp"

#include "Server.h"
#include "AppNodeStar.h"
#include "AppAuth.h"

// http://www.postgresql.org/docs/current/static/functions-net.html

namespace dbo = Wt::Dbo;

/*
 * Debian/Ubuntu:
 * sudo su - postgres
 * create role nodestar login password 'nodenode';
 * create database nodestar with owner nodestar;
 */

typedef dbo::ptr<DbRecOrganization> ptrOrganization_t;
typedef dbo::ptr<DbRecIpAddress> ptrIpAddress_t;

struct InsertNetwork {
  
  struct Network {
    std::string sIdOrganization;
    Cidr cidr;
    std::string sName;
    std::string sDescription;
    std::string sSource;
    Network( const std::string& sNetwork, const std::string& sName_, const std::string& sDescription_, const std::string& sSource_ )
            : cidr( sNetwork ), sName( sName_ ), sDescription( sDescription_ ), sSource( sSource_ ) {}
    Network( const std::string& sIdOrganization_, const std::string& sNetwork, const std::string& sName_, const std::string& sDescription_, const  std::string& sSource_ )
            : sIdOrganization( sIdOrganization_ ), cidr( sNetwork ), sName( sName_ ), sDescription( sDescription_ ), sSource( sSource_ ) {}
  };
  
  typedef std::vector<Network> vNetwork_t;
  typedef std::map<int,vNetwork_t> mapNetworks_t; // a vector per mask length
  typedef mapNetworks_t::iterator mapNetworks_iter;
  
  dbo::Session& session;
  ptrOrganization_t ptrOrganization;
  ptrIpAddress_t ptrIpAddress;
  
  mapNetworks_t mapNetworks;
  InsertNetwork( dbo::Session& session_, ptrOrganization_t ptrOrganization_ )
    : session( session_ ), ptrOrganization( ptrOrganization_ ) {};
  ~InsertNetwork( void ) {
    }
      
  // http://stackoverflow.com/questions/313970/c-stdstring-to-lower-case
  
  void operator()( const std::string& sNetwork, const std::string& sName, const std::string& sComment, const  std::string& sSource ) {
    // collect addresses, sort masks shortest to longest, then insert as hierarchy into database
    Network network( sNetwork, sName, sComment, sSource );
    //std::transform(network.sName.begin(), network.sName.end(), network.sName.begin(), ::tolower );
    boost::algorithm::to_lower( network.sName );
    int len = network.cidr.pflen();
    mapNetworks_iter iter = mapNetworks.find( len );
    if ( mapNetworks.end() == iter ) {
      std::cout << "adding vector of length " << len << " for " << sNetwork << std::endl;
      vNetwork_t vNetwork; // zero length vector
      // mapNetworks[ len ] = vNetwork;
      // iter = mapNetworks.find( len );
      iter = mapNetworks.insert( mapNetworks.begin(), mapNetworks_t::value_type( len, vNetwork ) );
    }
    iter->second.push_back(network);
  }
  
  void operator()( const std::string& sIdOrganization, const std::string& sNetwork, const std::string& sName, const std::string& sComment, const std::string& sSource ) {
    // collect addresses, sort masks shortest to longest, then insert as hierarchy into database
    //Network network( sIdOrganization, sNetwork, sName, sComment );  // need an org insertion capability to do this
    // Name is defaulted to 'unknown' on entry
    Network network( sNetwork, sName, sComment, sSource );
    if ( "" != sIdOrganization ) {
      network.sName = sIdOrganization;
    }
    boost::algorithm::to_lower( network.sName );
    int len = network.cidr.pflen();
    mapNetworks_iter iter = mapNetworks.find( len );
    if ( mapNetworks.end() == iter ) {
      std::cout << "adding vector of length " << len << " for " << sNetwork << std::endl;
      vNetwork_t vNetwork;
      //mapNetworks[ len ] = vNetwork;
      //iter = mapNetworks.find( len );
      iter = mapNetworks.insert( mapNetworks.begin(), mapNetworks_t::value_type( len, vNetwork ) );
    }
    iter->second.push_back(network);
  }
  
  void Finish( void ) {
    // from short mask to longer mask
    for ( mapNetworks_iter miter = mapNetworks.begin(); mapNetworks.end() != miter; ++miter ) {
      vNetwork_t& vNetwork( miter->second );
      for ( vNetwork_t::const_iterator viter = vNetwork.begin(); vNetwork.end() != viter; ++viter ) {
        const Network& network( *viter );
        //std::cout << "adding " << network.cidr.str() << std::endl;
        try {
          dbo::Transaction transaction( session );
          int count = session.query<int>( "select count(1) from ipaddress" ).where("ipaddress = ?").bind( network.cidr.str() );
          if ( 0 != count ) {
            std::cout << "** " << network.cidr << " already exists" << std::endl;
          }
          else {
            typedef dbo::collection<ptrIpAddress_t> ptrIpAddresses_t;
            try {
              ptrIpAddress_t ptrIpAddress;
              int maxlen( 0 );
              ptrIpAddresses_t ptrIpAddresses = session.find<DbRecIpAddress>().where("ipaddress>>?").bind( network.cidr.str() );
              for ( ptrIpAddresses_t::iterator iter = ptrIpAddresses.begin(); iter != ptrIpAddresses.end(); ++iter ) {
                int masklen = (*iter)->cidrIpAddress.pflen();
                if ( maxlen < masklen ) {
                  ptrIpAddress = *iter;
                  maxlen = masklen;
                }
              }
              if ( 0 == maxlen ) {
                session.add( new DbRecIpAddress( ptrOrganization, network.cidr.str(), network.sName, network.sDescription, "", "", network.sSource ) );
              }
              else {
                session.add( new DbRecIpAddress( ptrOrganization, ptrIpAddress, network.cidr.str(), network.sName, network.sDescription, "", "", network.sSource ) );
              }
//              transaction.commit();
            }
            catch ( dbo::Exception& e ) {
              std::cout << "error 4 " << e.what() << std::endl;
            }
                      }
          //dbo::Transaction transaction( session );
          //ptrIpAddress = 
          //  session.add( 
          //    new TableIpAddress( ptrOrganization, network.cidr.str(), network.sName, network.sDescription, "", "", sSource ) );
          transaction.commit();
        }
        catch ( dbo::Exception& e ) {
          std::cout << "error finish " << e.what() << std::endl;
        }
      }
    }
  }
}; // struct InsertNetwork

void MapClasses( dbo::Session& session ) {
  
    session.mapClass<DbRecOrganization>( "organization" );
    session.mapClass<DbRecIpAddress>( "ipaddress" );
    
}

void PopulateDatabase( dbo::FixedSqlConnectionPool& pool ) {
  
  dbo::Session session;
  session.setConnectionPool( pool );
  
  MapClasses( session );

  try {
    
    dbo::Transaction transaction( session );    
    DbRecOrganization* pqvsl 
            = new DbRecOrganization( "QVSL", "QuoVadis Services Ltd.", "Bermuda based hosting services",
                 "https://www.quovadisglobal.bm/en-GB/HostingSolutions.aspx", 19626 );
    dbo::ptr<DbRecOrganization> ptrOrg = session.add( pqvsl );   
    transaction.commit();
    
    PopulateBasicIpAddresses( session );
    
  }
  catch ( dbo::Exception& e ) {
    std::cout << "PopulateDatabase: errors 1 " << e.what() << std::endl;
  }
  catch (...) {
    std::cout << "PopulateDatabase: errors 2" << std::endl;
  }
  
    // ensure we have the default organization id (does it need to there now, or is it added later?)
  // it is actually added above, so this code may not be required, but is here for future reference
  ptrOrganization_t ptrQvsl;
  try {
    dbo::Transaction transaction( session );
    ptrQvsl = session.find<DbRecOrganization>().where("idorganization=?").bind("QVSL");
    transaction.commit();
  }
  catch ( dbo::Exception& e ) {
    std::cout << "error 2 " << e.what() << std::endl;
  }

  // need to do some error checking reqarding above before performing following
  
  // remove some old records, as they will be re-populated in the ipaddress table
  typedef dbo::collection<ptrIpAddress_t> ptrIpAddresses_t;
  try {
    dbo::Transaction transaction( session );
    //ptrIpAddresses_t ptrIpAddresses = session.query<ptrIpAddress_t>( "select * from ipaddress").where("source=?").bind(sSource);
    ptrIpAddresses_t ptrIpAddresses = session.find<DbRecIpAddress>().where("source like ? or source = ?").bind("smc xml%").bind("network.csv");
    for ( ptrIpAddresses_t::iterator iter = ptrIpAddresses.begin(); iter != ptrIpAddresses.end(); ++iter ) {
      iter->remove();
    }
    transaction.commit();
  }
  catch ( dbo::Exception& e ) {
    std::cout << "error 4 " << e.what() << std::endl;
  }
  
  // put this under separate menu
  // also test for existance of file prior to attempting to import
  try {
    //dbo::Transaction transaction( session );
    InsertNetwork net( session, ptrQvsl );  // prep for insertions (first insertion wins, import order is important)
//    ImportNetworkCsvFile( net ); // data from mysql tables
//    ImportSmcXml( net );  // data from main smc
    net.Finish();  // 
    //transaction.commit();
  }
  catch ( dbo::Exception& e ) {
    std::cout << "error 3 " << e.what() << std::endl;
  }
  
}

// may need to refine how tables are redone:
// in some cases, depending upon foreign constraints,
//   can knock out one table, mapClass the one table to a session, drop it, and recreate it
//   and leave the other tables intact
// maintain code versions:  code segments to modify schema:  fields, records, foreign keys
void InitializeTables( dbo::FixedSqlConnectionPool& pool ) {
  
  try {
    
    dbo::Session session;
    session.setConnectionPool( pool );

    MapClasses( session );
    UserAuth::MapClasses( session );

    // need to split this out and conditionally drop tables, only if they exist.
    // attach this to a url?  but application hasn't been started yet,
    session.dropTables();  // for testing
    
    session.createTables();
    
    // populate auth table with default admin
    
    {  // initialize tables for user authentication/authorization functions
      typedef boost::shared_ptr<UserAuth> pUserAuth_t;
      pUserAuth_t pUserAuth;
      pUserAuth.reset( new UserAuth( pool ) );
      pUserAuth->InitializeTables();
    }
  }
  catch ( Wt::Dbo::Exception& e ) {
    std::cout << "InitializeTables Error: " << e.what() << std::endl;
    throw;
  }
  
    
}

Wt::WApplication* CreateAppNodeStar( const Wt::WEnvironment& env ) {
  return new AppNodeStar( env );
}

//Wt::WApplication* CreateAppAuth( const Wt::WEnvironment& env ) {
//  return new AppAuth( env );
//}

void StartAppManger( int argc, char** argv, dbo::FixedSqlConnectionPool& pool ) {
  
  try {
    
    InitializeTables( pool );
    
    PopulateDatabase( pool );  // turn on when new file available, but convert to gui import function at some point
    
    Server server(pool, argv[0]);
    
    UserAuth::configureAuth();

    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
    server.addEntryPoint( Wt::Application, CreateAppNodeStar);
    //server.addEntryPoint( Wt::Application, CreateAppAuth, "/auth" );
    
    if (server.start()) {
      Wt::WServer::waitForShutdown();
      server.stop();
    }
  } 
  catch (Wt::WServer::Exception& e) {
    std::cerr << e.what() << std::endl;
  } 
  catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << std::endl;
  }
  
}

// http://www.webtoolkit.eu/wt/doc/reference/html/InstallationUnix.html
int main(int argc, char** argv) {
  
  dbo::backend::Postgres* pq = 0;
  
  std::string sConnection( "host=127.0.0.1 user=nodestar password=nodenode port=5432 dbname=nodestar" );
  pq = new dbo::backend::Postgres( sConnection );
  
  //pq->setProperty( "show-queries", "true" );
  
  dbo::FixedSqlConnectionPool pool( pq, 4 );
  
  StartAppManger( argc, argv, pool );
}

