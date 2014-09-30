/* 
 * File:   main.cpp
 * Author: sysadmin
 *
 * Created on September 17, 2014, 1:25 PM
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Wt/Dbo/backend/Postgres>

#include "SqlTraits.h"

#include "model/TableOrganization.h"
#include "model/TableIpAddress.h"

#include "utility/PopulateBasicIpAddresses.h"
#include "utility/ImportSmcXml.h"

#include "Cidr.hpp"

#include "WebApp.h"

namespace dbo = Wt::Dbo;

/*
 * Debian/Ubuntu:
 * sudo su - postgres
 * create role nodestar login password 'nodenode';
 * create database nodestar with owner nodestar;
 */

typedef dbo::ptr<TableOrganization> ptrOrganization_t;
typedef dbo::ptr<TableIpAddress> ptrIpAddress_t;

struct InsertNetwork {
  const std::string sSource;
  dbo::Session& session;
  ptrOrganization_t ptrOrganization;
  struct Network {
    Cidr cidr;
    std::string sName;
    std::string sDescription;
    Network( const std::string& sNetwork, const std::string& sName_, const std::string& sDescription_ )
            : cidr( sNetwork ), sName( sName_ ), sDescription( sDescription_ ) {}
  };
  typedef std::vector<Network> vNetwork_t;
  typedef std::map<int,vNetwork_t> mapNetworks_t; // a vector per mask length
  typedef mapNetworks_t::iterator mapNetworks_iter;
  mapNetworks_t mapNetworks;
  InsertNetwork( dbo::Session& session_, ptrOrganization_t ptrOrganization_, const std::string& sSource_ )
    : session( session_ ), ptrOrganization( ptrOrganization_ ), sSource( sSource_ ) {};
  ~InsertNetwork( void ) {
    }
    
  void operator()( const std::string& sNetwork, const std::string& sName, const std::string& sComment ) {
    // collect addresses, sort masks shortest to longest, then insert as hierarchy into database
    Network network( sNetwork, sName, sComment );
    int len = network.cidr.pflen();
    mapNetworks_iter iter = mapNetworks.find( len );
    if ( mapNetworks.end() == iter ) {
      std::cout << "adding vector of length " << len << " for " << sNetwork << std::endl;
      vNetwork_t vNetwork;
      mapNetworks[ len ] = vNetwork;
      iter = mapNetworks.find( len );
    }
    iter->second.push_back(network);
  }
  void Finish( void ) {
    for ( mapNetworks_iter miter = mapNetworks.begin(); mapNetworks.end() != miter; ++miter ) {
      vNetwork_t& vNetwork( miter->second );
      for ( vNetwork_t::const_iterator viter = vNetwork.begin(); vNetwork.end() != viter; ++viter ) {
        const Network& network( *viter );
        std::cout << "adding " << network.cidr.str() << std::endl;
        try {
          dbo::Transaction transaction( session );
          //ptrIpAddress_t ptrIpAddress;
          //ptrIpAddress = 
                  session.add( 
                  new TableIpAddress( ptrOrganization, network.cidr.str(), network.sName, network.sDescription, "", "", sSource ) );
          transaction.commit();
        }
        catch ( dbo::Exception& e ) {
          std::cout << "error finish " << e.what() << std::endl;
        }
      }
    }
  }
};

void InitDatabase( dbo::backend::Postgres& be ) {
  
  dbo::Session session;
  session.setConnection( be );

  session.mapClass<TableOrganization>( "organization" );
  session.mapClass<TableIpAddress>( "ipaddress" );
  
  try {
    
    //session.dropTables();  // for testing

    session.createTables();
    
    PopulateBasicIpAddresses( session );
    
    dbo::Transaction transaction( session );
    
    TableOrganization* pqvsl 
            = new TableOrganization( "QVSL", "QuoVadis Services Ltd.", "Bermuda based hosting services",
                 "https://www.quovadisglobal.bm/en-GB/HostingSolutions.aspx", 19626 );
    dbo::ptr<TableOrganization> ptrOrg = session.add( pqvsl );
    
    transaction.commit();
    
  }
  catch ( dbo::Exception& e ) {
    std::cout << "error 1 " << e.what() << std::endl;
  }
  catch (...) {
    std::cout << "errors" << std::endl;
  }
  
  // need to do some error checking reqarding above before performing following
  
  std::string sSource( "smc xml");
  
  typedef dbo::collection<ptrIpAddress_t> ptrIpAddresses_t;
  try {
    dbo::Transaction transaction( session );
    //ptrIpAddresses_t ptrIpAddresses = session.query<ptrIpAddress_t>( "select * from ipaddress").where("source=?").bind(sSource);
    ptrIpAddresses_t ptrIpAddresses = session.find<TableIpAddress>().where("source=?").bind(sSource);
    for ( ptrIpAddresses_t::iterator iter = ptrIpAddresses.begin(); iter != ptrIpAddresses.end(); ++iter ) {
      iter->remove();
    }
    transaction.commit();
  }
  catch ( dbo::Exception& e ) {
    std::cout << "error 4 " << e.what() << std::endl;
  }
  
  ptrOrganization_t ptrQvsl;
  try {
    dbo::Transaction transaction( session );
    ptrQvsl = session.find<TableOrganization>().where("idorganization=?").bind("QVSL");
    transaction.commit();
  }
  catch ( dbo::Exception& e ) {
    std::cout << "error 2 " << e.what() << std::endl;
  }
   
  try {
    //dbo::Transaction transaction( session );
    InsertNetwork net( session, ptrQvsl, "smc xml");
    ImportSmcXml( net );
    net.Finish();
    //transaction.commit();
  }
  catch ( dbo::Exception& e ) {
    std::cout << "error 3 " << e.what() << std::endl;
  }
  
}

dbo::backend::Postgres* pq = 0;

Wt::WApplication* CreateApplication( const Wt::WEnvironment& env ) {
  if ( 0 == pq ) {
    throw std::runtime_error( "no connection" );
  }
  return new WebApp( env, *pq );
}

// http://www.webtoolkit.eu/wt/doc/reference/html/InstallationUnix.html

int main(int argc, char** argv) {
  
  std::string sConnection( "host=127.0.0.1 user=nodestar password=nodenode port=5432 dbname=nodestar" );
  pq = new dbo::backend::Postgres( sConnection );
  //pq->setProperty( "show-queries", "true" );
  
  InitDatabase( *pq );
  
  //return Wt::WRun( argc, argv, &CreateApplication );
}

