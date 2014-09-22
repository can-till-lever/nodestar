#include <string.h>
#include <iostream>

#include "Cidr.hpp"

cidr::cidr( void ) {
  memset(&m_cidr, 0, sizeof(cidr_addr));
}

cidr::cidr( const struct in_addr& in ) {
  cidr_addr* p = cidr_from_inaddr( &in );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    m_cidr = *p;
    cidr_free( p );
  }
}

cidr::cidr( const struct in6_addr& in ) {
  cidr_addr* p = cidr_from_in6addr( &in );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    m_cidr = *p;
    cidr_free( p );
  }
}

cidr::cidr( const std::string& s ) {
  cidr_addr* p = cidr_from_str( s.c_str() );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    m_cidr = *p;
    cidr_free( p );
  }
}

cidr::cidr( const cidr& rhs ): m_cidr( rhs.m_cidr ) {};

cidr::~cidr( void ) {
  
}

void cidr::divide( cidr& a, cidr& b ) const {
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

void cidr::supernet(cidr& c ) const {
  cidr_addr* p = cidr_net_supernet( &m_cidr );
  if ( 0 == p ) {
    throw error( errno );
  }
  else {
    c.m_cidr = *p;
    cidr_free( p );
  }
}

const std::string cidr::str( int flags ) const {
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

std::ostream& operator<<(std::ostream& o, const cidr& c ) {
  return o << c.str();
}
