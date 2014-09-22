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

namespace dbo = Wt::Dbo;

/*
 * Debian/Ubuntu:
 * sudo su - postgres
 * create role nodestar login password 'nodenode';
 * create database nodestar with owner nodestar;
 */

void run( void ) {
  
  try {
    dbo::backend::Postgres pq( "host=127.0.0.1 user=nodestar password=nodenode port=5432 dbname=nodestar" );
    //pq.setProperty( "show-queries", "true" );
    dbo::Session session;
    session.setConnection( pq );

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

int main(int argc, char** argv) {
  std::cout << "hello" << std::endl;
  run();
  std::cout << "bye" << std::endl;
  return 0;
}

