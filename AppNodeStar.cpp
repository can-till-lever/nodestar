/* 
 * File:   WebApp.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on September 22, 2014, 4:38 PM
 */

#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WText>

#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "AppNodeStar.h"

AppNodeStar::AppNodeStar( const Wt::WEnvironment& env ): Wt::WApplication( env ), m_pServer( 0 ) {
  
  m_pServer = dynamic_cast<Server*>( env.server() );
  m_pUserAuth.reset( new UserAuth( m_pServer->GetConnectionPool() ) );
  
  setTitle( "NodeStar: Network Infrastructure Data Management" );
  root()->addWidget( new Wt::WText( "More to Come" ) );
}

AppNodeStar::~AppNodeStar() {
}

void AppNodeStar::initialize() {
}

void AppNodeStar::finalize() {
}