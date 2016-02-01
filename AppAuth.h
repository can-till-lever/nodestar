/* 
 * File:   AppAuth.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on January 27, 2016, 2:59 PM
 */

#pragma once

#include <boost/shared_ptr.hpp>

#include <Wt/Dbo/ptr>
#include <Wt/WApplication>

#include "model/UserAuth.h"

#include "Server.h"

class AppAuth: public Wt::WApplication {
public:
  
  AppAuth( const Wt::WEnvironment& );
  virtual ~AppAuth();
  
  virtual void initialize( void ); // Initializes the application, post-construction. 
  virtual void finalize( void );  // Finalizes the application, pre-destruction.
  
protected:
private:

  typedef boost::shared_ptr<UserAuth> pUserAuth_t;
  pUserAuth_t m_pUserAuth;
  
  Server* m_pServer; // object managed by wt
  
  dbo::Session m_Session;

  void HandleInternalPathChanged( const std::string& );
  void HandleInternalPathInvalid( const std::string& );
  
};


