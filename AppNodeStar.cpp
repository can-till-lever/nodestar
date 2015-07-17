/* 
 * File:   WebApp.cpp
 * Author: sysadmin
 * 
 * Created on September 22, 2014, 4:38 PM
 */

#include <Wt/WContainerWidget>
#include <Wt/WText>

#include "AppNodeStar.h"

AppNodeStar::AppNodeStar( const Wt::WEnvironment& env, dbo::backend::Postgres& pq ): Wt::WApplication( env )  {
  setTitle( "NodeStar: Network Infrastructure Data Management" );
  root()->addWidget( new Wt::WText( "More to Come" ) );
}

AppNodeStar::~AppNodeStar() {
}

