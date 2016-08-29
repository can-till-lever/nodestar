/* 
 * File:   Auth.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on March 11, 2016, 10:05 PM
 */

#pragma once

#include <boost/shared_ptr.hpp>

#include <Wt/Dbo/ptr>

#include "model/UserAuth.h"

class Auth {
public:
  Auth( dbo::FixedSqlConnectionPool& pool );
  virtual ~Auth();
  Wt::Auth::AuthWidget* NewAuthWidget( void );
  bool LoggedIn( void ) const { return m_pUserAuth->login().loggedIn(); };
  typedef Wt::Signals::connection (changed)(void);
  template<class T, class C>
  Wt::Signals::connection RegisterLoginChanged( T* target, C c ) {
     return m_pUserAuth->login().changed().connect( target, c );
  }
protected:
private:
  typedef boost::shared_ptr<UserAuth> pUserAuth_t;
  pUserAuth_t m_pUserAuth;
};
