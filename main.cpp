/* 
 * File:   main.cpp
 * Author: sysadmin
 *
 * Created on September 17, 2014, 1:25 PM
 */

#include <iostream>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Postgres>

#include "cidr.hpp"

namespace dbo = Wt::Dbo;

/*
 * sudo su - postgres
 * create role nodestar login password 'nodenode';
 * create database nodestar with owner nodestar;
 */

class TableOrganization {
public:
  
  std::string sIdOrganization;
  std::string sName;
  std::string sDescription;
  int nAsn;
  std::string sUrl;
  
  template<class Action>
  void persist( Action& a ) {
    dbo::field( a, sIdOrganization, "idorganization" );
    dbo::field( a, sName, "name" );
    dbo::field( a, sDescription, "description" );
    dbo::field( a, nAsn, "asn" );
    dbo::field( a, sUrl, "url" );
  }
  
protected:
private:
  
};

namespace Wt {
namespace Dbo {

// http://www.boost.org/doc/libs/1_56_0/libs/uuid/uuid.html

template<>
struct sql_value_traits<boost::uuids::uuid, void> {
  static const bool specialized = true;

  static const char *type(SqlConnection *conn, int size);
  static void bind(const boost::uuids::uuid& v, SqlStatement *statement, int column, int size);
  static bool read(boost::uuids::uuid& v, SqlStatement *statement, int column, int size);
};

inline const char *sql_value_traits<boost::uuids::uuid, void>::type(SqlConnection *conn, int size) {
  return "uuid not null";
}

inline void sql_value_traits<boost::uuids::uuid, void>::bind(const boost::uuids::uuid& v, SqlStatement *statement, int column, int size) {
  statement->bind(column, boost::uuids::to_string( v ) );
//  if (v.isNull())
//    statement->bindNull(column);
//  else
//    statement->bind(column, v.toPosixTime(), SqlDateTime);
}

inline bool sql_value_traits<boost::uuids::uuid, void>::read(boost::uuids::uuid& v, SqlStatement *statement, int column, int size) {

  std::string d;
  if (statement->getResult(column, &d, size)) {
    boost::uuids::string_generator gen;
    v = gen(d);
    return true;
  } 
  else {
    boost::uuids::nil_generator gen; 
    v = gen();
    return false;
  }
}

}
}

namespace Wt {
namespace Dbo {

template<>
struct sql_value_traits<cidr, void> {
  
  static const bool specialized = true;

  static const char *type(SqlConnection *conn, int size);
  static void bind(const cidr& v, SqlStatement *statement, int column, int size);
  static bool read(cidr& v, SqlStatement *statement, int column, int size);
};

inline const char *sql_value_traits<cidr, void>::type(SqlConnection *conn, int size) {
  return "cidr not null";
}

inline void sql_value_traits<cidr, void>::bind(const cidr& v, SqlStatement *statement, int column, int size) {
  statement->bind( column, v.str() );

//  if (v.isNull())
//    statement->bindNull(column);
//  else
//    statement->bind(column, v.toPosixTime(), SqlDateTime);
}

inline bool sql_value_traits<cidr, void>::read(cidr& v, SqlStatement *statement, int column, int size) {

  std::string d;
  if (statement->getResult(column, &d, size)) {
    cidr c( d );
    v = c;
    return true;
  } 
  else {
    return false;
  }
  
}

}
}

namespace Wt {
namespace Dbo {

inline const char *sql_value_traits<boost::posix_time::ptime, void>::type(SqlConnection *conn, int size) {
  return conn->dateTimeType(SqlDateTime);
}

inline void sql_value_traits<boost::posix_time::ptime, void>
::bind(const boost::posix_time::ptime& v, SqlStatement *statement, int column, int size) {
  if ( v.is_not_a_date_time() )
    statement->bindNull(column);
  else
    statement->bind(column, v, SqlDateTime);
}

inline bool sql_value_traits<boost::posix_time::ptime, void>
::read(boost::posix_time::ptime& v, SqlStatement *statement, int column, int size) {

  if (statement->getResult(column, &v, SqlDateTime)) {
    return true;
  } else {
    v = boost::posix_time::ptime();
    return false;
  }
}

}
}




class TableIpAddress {
public:
  
  boost::uuids::uuid uuidId;
  boost::uuids::uuid uuidParent;
  cidr cidrIpAddress;
  std::string sOrganization;
  std::string sName;
  std::string sDescription;
  std::string sFqdn;
  std::string sUrl;
  std::string sSource; // software module used for creating the entry
  boost::posix_time::ptime dtCreation;
  
  template<class Action>
  void persist( Action& a ) {
    dbo::field( a, uuidId, "idipaddress" );
    dbo::field( a, uuidParent, "parent" );
    dbo::field( a, cidrIpAddress, "ipaddress" );
    dbo::field( a, sOrganization, "idorganization" );
    dbo::field( a, sName, "name" );
    dbo::field( a, sDescription, "description" );
    dbo::field( a, sFqdn, "fqdn" );
    dbo::field( a, sUrl, "url" );
    dbo::field( a, sSource, "source" );
    dbo::field( a, dtCreation, "datetimecreation" );
  }
  
protected:
private:
};

void run( void ) {
  
  try {
    dbo::backend::Postgres pq( "host=127.0.0.1 user=nodestar password=nodenode port=5432 dbname=nodestar" );
    dbo::Session session;
    session.setConnection( pq );

    session.mapClass<TableOrganization>( "organization" );
    session.mapClass<TableIpAddress>( "ipaddress" );

    session.createTables();
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

