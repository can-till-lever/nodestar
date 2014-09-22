/* 
 * File:   main.cpp
 * Author: sysadmin
 *
 * Created on September 17, 2014, 1:25 PM
 */

#include <iostream>
#include <string>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Wt/Dbo/backend/Postgres>

#include "SqlTraits.h"

#include "model/TableOrganization.h"
#include "model/TableIpAddress.h"

#include "PopulateBasicIpAddresses.h"
#include "WebApp.h"

namespace dbo = Wt::Dbo;

/*
 * Debian/Ubuntu:
 * sudo su - postgres
 * create role nodestar login password 'nodenode';
 * create database nodestar with owner nodestar;
 */

void InitDatabase( dbo::backend::Postgres& be ) {
  
  try {
    
    dbo::Session session;
    session.setConnection( be );

    session.mapClass<TableOrganization>( "organization" );
    session.mapClass<TableIpAddress>( "ipaddress" );
    
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
  //catch ( dbo::backend::PostgresException& e ) {
  catch ( dbo::Exception& e ) {
    std::cout << "error " << e.what() << std::endl;
  }
  catch (...) {
    std::cout << "errors" << std::endl;
  }
  
}

dbo::backend::Postgres* pq;

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
  return Wt::WRun( argc, argv, &CreateApplication );
}

