/* 
 * File:   AppAuth.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on January 27, 2016, 2:59 PM
 */

#include <Wt/WEnvironment>

#include <Wt/WContainerWidget>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "AppAuth.h"

AppAuth::AppAuth( const Wt::WEnvironment& env ): Wt::WApplication( env ), m_pServer( 0 ) {
  
  m_pServer = dynamic_cast<Server*>( env.server() );
  m_pUserAuth.reset( new UserAuth( m_pServer->GetConnectionPool() ) );
  
  m_Session.setConnectionPool( m_pServer->GetConnectionPool() );
    
  setTitle( "NodeStar: Network Infrastructure Data Management" );
 
  m_pUserAuth->login().changed().connect( m_pUserAuth.get(), &UserAuth::authEvent );

  //useStyleSheet("css/style.css");

  Wt::Auth::AuthWidget *authWidget
    = new Wt::Auth::AuthWidget(UserAuth::auth(), m_pUserAuth->Users(), m_pUserAuth->login() );

  authWidget->model()->addPasswordAuth(&UserAuth::passwordAuth());
  authWidget->model()->addOAuth(UserAuth::oAuth());
  authWidget->setRegistrationEnabled(true);

  authWidget->processEnvironment();

  root()->addWidget(authWidget);  

}

AppAuth::~AppAuth() {
}


void AppAuth::initialize() {
}

void AppAuth::finalize() {
}