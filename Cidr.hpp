/* 
 * File:   cidr.hpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 *   wraps libcidr c files into c++ class
 *   https://www.over-yonder.net/~fullermd/projects/libcidr 
 *
 * Created on September 18, 2014, 1:04 PM
 */

#ifndef CIDR_HPP
#define	CIDR_HPP

#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <errno.h>

#include <boost/lexical_cast.hpp>

#ifdef __cplusplus
extern "C" {
#endif

 
#include <libcidr.h>
  
#ifdef __cplusplus
}
#endif
  
    
class Cidr {
  friend std::ostream& operator<<(std::ostream& o, const Cidr& );
public:
  
  Cidr( void );
  explicit Cidr( const struct in_addr& );
  explicit Cidr( const struct in6_addr& );
  explicit Cidr( const std::string& );
  explicit Cidr( const Cidr& );
  ~Cidr( void );
  
  void broadcast( Cidr& c ) const { wrap1( &cidr_addr_broadcast, c ); };  
  void   hostmax( Cidr& c ) const { wrap1( &cidr_addr_hostmax, c ); };
  void   hostmin( Cidr& c ) const { wrap1( &cidr_addr_hostmin, c ); };
  void   network( Cidr& c ) const { wrap1( &cidr_addr_network, c ); };
  
  void divide( Cidr&, Cidr& ) const;
  void supernet( Cidr& ) const;
  
  uint8_t* addr( void ) const { return cidr_get_addr( &m_cidr ); } // need to free the result
  uint8_t* mask( void ) const { return cidr_get_mask( &m_cidr ); } // need to free the result
  
  int pflen( void ) const { return cidr_get_pflen( &m_cidr ); }
  int proto( void ) const { return cidr_get_proto( &m_cidr ); }  // CIDR_IPV4 and CIDR_IPV6.
  const char* numaddr( void ) const { return cidr_numaddr( &m_cidr ); } // do not use free on the result
  const char* numhost( void ) const { return cidr_numhost( &m_cidr ); }
  
  const char* numaddr_pflen( void ) const { return cidr_numaddr_pflen( pflen() ); }
  static const char* numaddr_pflen( int pflen ) { return cidr_numaddr_pflen( pflen ); }
  
  const char* numhost_pflen( void ) const { return cidr_numhost_pflen( pflen() ); }
  static const char* numhost_pflen( int pflen ) { return cidr_numhost_pflen( pflen ); }
  
  bool v4mapped( void ) const { return 0 == cidr_is_v4mapped( &m_cidr ); }
  
  int contains( const Cidr& c ) const { return cidr_contains( &m_cidr, &c.m_cidr ); } // Returns 0 if little is wholly contained within big. Returns -1 if it's not, or if an error occured.
  int   equals( const Cidr& c ) const { return   cidr_equals( &m_cidr, &c.m_cidr ); } // Returns 0 if the two CIDR structs describe the same netblock. Returns -1 otherwise.
  
  bool operator==( const Cidr& rhs ) const { return 0 == equals( rhs ); }
  bool operator!=( const Cidr& rhs ) const { return 0 != equals( rhs ); }
  
  void  inaddr(  in_addr& in ) const {  cidr_to_inaddr( &m_cidr, &in ); }
  void in6addr( in6_addr& in ) const { cidr_to_in6addr( &m_cidr, &in ); }
  
  const std::string str( int flags = CIDR_NOFLAGS ) const;
  
  const Cidr& operator=( const Cidr& rhs ) {
    if ( this != &rhs ) {
      m_cidr = rhs.m_cidr;
    }
    return *this;
  }
  
  struct error: public std::runtime_error {
    int err;
    error( int err_ ): std::runtime_error( "cidr " + boost::lexical_cast<int>( err_ ) ), err( err_ ) {};
  };
  
protected:
  
  typedef cidr_addr* (*wrap1_t)( const cidr_addr* );
  
  void wrap1( wrap1_t func, Cidr& c ) const {
    cidr_addr* p;
    p = func( &c.m_cidr );
    if ( 0 == p ) {
      throw error( errno );
    }
    else {
      c.m_cidr = *p;
      cidr_free( p );
    }
  }
  
private:
  
  cidr_addr m_cidr;
  
};  

std::ostream& operator<<(std::ostream& o, const Cidr& );

 
  
#endif	/* CIDR_HPP */

