/* 
 * File:   SqlTraits.h
 * Author: sysadmin
 *
 * Created on September 18, 2014, 5:32 PM
 */

#ifndef SQLTRAITS_H
#define	SQLTRAITS_H


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Wt/Dbo/Dbo>

#include "Cidr.hpp"

namespace Wt {
namespace Dbo {

template<>
struct sql_value_traits<boost::uuids::uuid, void> {
  static const bool specialized = true;

  static const char *type(SqlConnection *conn, int size);
  static void bind(const boost::uuids::uuid& v, SqlStatement *statement, int column, int size);
  static bool read(boost::uuids::uuid& v, SqlStatement *statement, int column, int size);
};

inline const char *sql_value_traits<boost::uuids::uuid, void>::type(SqlConnection *conn, int size) {
//  return "uuid not null";
  return "uuid";
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
  return "cidr";
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



#endif	/* SQLTRAITS_H */

