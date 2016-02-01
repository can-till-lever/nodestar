/* 
 * File:   WebApp.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on September 22, 2014, 4:38 PM
 * 
 * access functions in cidr library to show ranges and such
 * parse zone files
 * parse stonegate xml file (wt has xml parser library?)
 */

#pragma once

#include <boost/shared_ptr.hpp>

#include <Wt/Dbo/ptr>
#include <Wt/WApplication>

#include "model/UserAuth.h"

#include "Server.h"

class AppNodeStar: public Wt::WApplication {
public:
  AppNodeStar( const Wt::WEnvironment& );  // constructor as an application
  virtual ~AppNodeStar( void );
  
  virtual void initialize( void ); // Initializes the application, post-construction. 
  virtual void finalize( void );  // Finalizes the application, pre-destruction.
  
protected:
private:
  
  typedef boost::shared_ptr<UserAuth> pUserAuth_t;
  pUserAuth_t m_pUserAuth;
  
  Server* m_pServer; // object managed by wt
  dbo::Session m_Session;

  
  Wt::WTable* m_pTable;
  
  void HandleShowAddresses( const Wt::WMouseEvent& );

  void HandleInternalPathChanged( const std::string& );
  void HandleInternalPathInvalid( const std::string& );
};
