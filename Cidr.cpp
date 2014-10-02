/* 
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 */

#include <string.h>
#include <iostream>

#include "Cidr.hpp"

Cidr::Cidr( void ) {
  memset(&m_cidr, 0, sizeof(cidr_addr));
}

Cidr::Cidr( const struct in_addr& in ) {
  cidr_addr* p = cidr_from_inaddr( &in );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    m_cidr = *p;
    cidr_free( p );
  }
}

Cidr::Cidr( const struct in6_addr& in ) {
  cidr_addr* p = cidr_from_in6addr( &in );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    m_cidr = *p;
    cidr_free( p );
  }
}

Cidr::Cidr( const std::string& s ) {
  cidr_addr* p = cidr_from_str( s.c_str() );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    m_cidr = *p;
    cidr_free( p );
  }
}

Cidr::Cidr( const Cidr& rhs ): m_cidr( rhs.m_cidr ) {};

Cidr::~Cidr( void ) {
  
}

void Cidr::divide( Cidr& a, Cidr& b ) const {
  cidr_addr** p = cidr_net_subnets( &m_cidr );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    a.m_cidr = *(p[0]);
    b.m_cidr = *(p[1]);
    cidr_free(p[0]);
    cidr_free(p[1]);
    free( p );
  }
}

void Cidr::supernet(Cidr& c ) const {
  cidr_addr* p = cidr_net_supernet( &m_cidr );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    c.m_cidr = *p;
    cidr_free( p );
  }
}

const std::string Cidr::str( int flags ) const {
  char* p = cidr_to_str( &m_cidr, flags );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    std::string s( p );
    free( p );
    return s;
  }
  return ""; // shouldn't be reachable
}

std::ostream& operator<<(std::ostream& o, const Cidr& c ) {
  return o << c.str();
}
