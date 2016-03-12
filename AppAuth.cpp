/* 
 * File:   AppAuth.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on January 27, 2016, 2:59 PM
 */

#include <Wt/WEnvironment>

#include <Wt/Auth/AuthWidget>
#include <Wt/WContainerWidget>

#include "AppAuth.h"

AppAuth::AppAuth( const Wt::WEnvironment& env ): Wt::WApplication( env ), m_pServer( 0 ) {
  
  m_pServer = dynamic_cast<Server*>( env.server() );
  m_Session.setConnectionPool( m_pServer->GetConnectionPool() );
    
  setTitle( "NodeStar: Network Infrastructure Data Management" );
 
  //useStyleSheet("css/style.css");

  m_pAuth.reset( new Auth( m_pServer->GetConnectionPool() ) );
  
  root()->addWidget( m_pAuth->NewAuthWidget() );  
  
}

AppAuth::~AppAuth() {
}

void AppAuth::initialize() {
}

void AppAuth::finalize() {
}