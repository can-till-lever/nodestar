/* 
 * File:   WebApp.h
 * Author: sysadmin
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

//#include "Session.h"   // restore once session file is associated

namespace dbo = Wt::Dbo;

class AppNodeStar: public Wt::WApplication {
public:
  AppNodeStar( const Wt::WEnvironment&, dbo::backend::Postgres& );
  virtual ~AppNodeStar( void );
  
  void initialize( void ); // Initializes the application, post-construction. 
  void finalize( void );  // Finalizes the application, pre-destruction.
  
  void authEvent() {
//    if (m_session.login().loggedIn())
//      Wt::log("notice") << "User " << m_session.login().user().id()
//			<< " logged in.";
//    else
//      Wt::log("notice") << "User logged out.";
  };
  
protected:
private:
//  Session m_session;
  
  void AuthStuff( void );
  
  void HandleInternalPathChanged( const std::string& );
  void HandleInternalPathInvalid( const std::string& );
};

#endif	/* WEBAPP_H */

