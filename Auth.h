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

//#include <Wt/WContainerWidget>

#include "model/UserAuth.h"

class Auth {
public:
  Auth( dbo::FixedSqlConnectionPool& pool );
  virtual ~Auth();
  Wt::Auth::AuthWidget* NewAuthWidget( void );
protected:
private:
  typedef boost::shared_ptr<UserAuth> pUserAuth_t;
  pUserAuth_t m_pUserAuth;
};
