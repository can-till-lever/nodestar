/* 
 * File:   Auth.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on March 11, 2016, 10:05 PM
 */

#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "Auth.h"

Auth::Auth( dbo::FixedSqlConnectionPool& pool ) {
    
  std::cout << "new m_pUserAuth" << std::endl;
  m_pUserAuth.reset( new UserAuth( pool ) );
  
  // need to generalize this link with ** linkAuthEvent ** in UserAuth.h
  m_pUserAuth->login().changed().connect( m_pUserAuth.get(), &UserAuth::authEvent );

}

Auth::~Auth() {
}

Wt::Auth::AuthWidget* Auth::NewAuthWidget( void ) {
    
  Wt::Auth::AuthWidget* authWidget
    = new Wt::Auth::AuthWidget(UserAuth::auth(), m_pUserAuth->Users(), m_pUserAuth->login() );

  authWidget->model()->addPasswordAuth(&UserAuth::passwordAuth());
  authWidget->model()->addOAuth(UserAuth::oAuth());
  authWidget->setRegistrationEnabled(true);

  authWidget->processEnvironment();

  return authWidget;  
}