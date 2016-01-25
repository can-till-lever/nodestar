/* 
 * File:   WebApp.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on September 22, 2014, 4:38 PM
 * 
 * need authentication module
 * access functions in cidr library to show ranges and such
 * parse zone files
 * parse stonegate xml file (wt has xml parser library?)
 */

#ifndef WEBAPP_H
#define	WEBAPP_H

#include <string>

#include <Wt/WApplication>
#include <Wt/Dbo/backend/Postgres>

#include <Wt/Auth/Login>

#include "Server.h"

//#include "Session.h"   // restore once session file is associated

namespace dbo = Wt::Dbo;

class AppNodeStar: public Wt::WApplication {
public:
  AppNodeStar( const Wt::WEnvironment& );
  virtual ~AppNodeStar( void );
  
  void initialize( void ); // Initializes the application, post-construction. 
  void finalize( void );  // Finalizes the application, pre-destruction.
  
  void authEvent() {
    if ( m_login.loggedIn() )
      Wt::log("notice") << "User " << m_login.user().id()
			<< " logged in.";
    else
      Wt::log("notice") << "User logged out.";
  };
  
protected:
private:
//  Session m_session;
  
  Server* m_pServer;
  
  Wt::Auth::Login m_login;  // line 96 in UserAuth needs to be fixed
  
  void AuthStuff( void );
  
  void HandleInternalPathChanged( const std::string& );
  void HandleInternalPathInvalid( const std::string& );
};

#endif	/* WEBAPP_H */

