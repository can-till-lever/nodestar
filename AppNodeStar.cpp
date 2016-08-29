/* 
 * File:   WebApp.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on September 22, 2014, 4:38 PM
 */

#include <sstream>
#include <map>

#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/core/argument.hpp>

#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WAnchor>
#include <Wt/WLink>

#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "model/DbRecOrganization.h"
#include "model/DbRecIpAddress.h"

#include "page/ShowAddresses.h"
#include "page/Upload.h"

#include "AppNodeStar.h"

// to do:
//   on auth, need to go to a menu
//   need to show the user and the logout button (how to get that?)
//   with the login menu, there should be no sign in, so the login should be by itself, or just the home menu

// line 177 is where we start: need a logged in event to know when to rebuild the page 
//   or revert back to the normal sequence of events

namespace {

}

typedef dbo::ptr<DbRecOrganization> ptrOrganization_t;
typedef dbo::ptr<DbRecIpAddress> ptrIpAddress_t;

AppNodeStar::AppNodeStar( const Wt::WEnvironment& env ): Wt::WApplication( env ), m_pServer( 0 ) {
  
  m_pServer = dynamic_cast<Server*>( env.server() );
  //m_pUserAuth.reset( new UserAuth( m_pServer->GetConnectionPool() ) );
  
  //useStyleSheet(Wt::WLink("/css/nodestar.css"));
  //useStyleSheet("css/nodestar.css");
  useStyleSheet("style/nodestar.css");
  
  m_Session.setConnectionPool( m_pServer->GetConnectionPool() );
  
  m_Session.mapClass<DbRecOrganization>( "organization" );
  m_Session.mapClass<DbRecIpAddress>( "ipaddress" ); 
  
  internalPathChanged().connect( this, &AppNodeStar::HandleInternalPathChanged );
  internalPathInvalid().connect( this, &AppNodeStar::HandleInternalPathInvalid );
  
//  Wt::WLink link( Wt::WLink::InternalPath, "/link1" );
//  Wt::WAnchor* pAnchor = new Wt::WAnchor( link, "Test Link" );
//  root()->addWidget( pAnchor );
  
  //auto pTest = new Wt::WText( "<p>this is test</p>", root() );
  //auto pTest = new Wt::WText( "this is test", root() );
  //pTest->setStyleClass( "test" );
  
  // make use of the authEvent to change state (show login, or show logout) via Auth.h)
  m_pAuth.reset( new Auth( m_pServer->GetConnectionPool() ) );
  connectionLoginChanged = m_pAuth->RegisterLoginChanged( this, &AppNodeStar::HandleAuthLoginChanged );
  
  namespace args = boost::phoenix::arg_names;
  RegisterPath( "/home", boost::phoenix::bind( &AppNodeStar::ShowDefault, this, args::arg1 ) );
  RegisterPath( "/show/addresses", boost::phoenix::bind( &AppNodeStar::ShowAddresses, this, args::arg1 ) );
  RegisterPath( "/admin/tables/upload", boost::phoenix::bind( &AppNodeStar::Upload, this, args::arg1 ) );
  RegisterPath( "/auth/signin", boost::phoenix::bind( &AppNodeStar::ShowSignIn, this, args::arg1 ) );
  
  ShowDefault( root() );
}

AppNodeStar::~AppNodeStar() {
}

void AppNodeStar::initialize() {
}

void AppNodeStar::finalize() {
}

void AppNodeStar::DropTables(Wt::WContainerWidget*) {
  
}

void AppNodeStar::CreateTables(Wt::WContainerWidget*) {
  
}

void AppNodeStar::HandleInternalPathChanged( const std::string& sPath ) {
  root()->clear();
  std::cout << "HandleInternalPathChanged: " << sPath << std::endl;
  mapInternalPathChanged_t::const_iterator iter = m_mapInternalPathChanged.find( sPath );
  if ( m_mapInternalPathChanged.end() != iter ) {
    root()->addWidget( new Wt::WText( "Path Changed - " ) );
    iter->second( root() );
  }
  else {
    // default home page, or error page, and register a default page
    ShowDefault( root() );
  }
  std::cout << "end HandleInternalPathChanged" << std::endl;
}

void AppNodeStar::HandleInternalPathInvalid( const std::string& s ) {
  std::cout << "*** HandleInternalPathInvalid: " << s << std::endl;
}
  
void AppNodeStar::RegisterPath( const std::string& sPath, const slotInternalPathChanged_t& slot ) {
  mapInternalPathChanged_t::const_iterator iter = m_mapInternalPathChanged.find( sPath );
  if ( m_mapInternalPathChanged.end() != iter ) 
    std::runtime_error( "path exists" );
  m_mapInternalPathChanged.insert( mapInternalPathChanged_t::value_type( sPath, slot ) );
}

void AppNodeStar::UnRegisterPath( const std::string& sPath ) {
  mapInternalPathChanged_t::const_iterator iter = m_mapInternalPathChanged.find( sPath );
  if ( m_mapInternalPathChanged.end() == iter ) 
    std::runtime_error( "path not found" );
  m_mapInternalPathChanged.erase( iter );
}

void AppNodeStar::AddLink( Wt::WContainerWidget* pcw, const std::string& sClass, const std::string& sPath, const std::string& sAnchor ) {
  auto pContainer = new Wt::WContainerWidget( pcw );
  pContainer->setStyleClass( sClass );
  Wt::WLink link( Wt::WLink::InternalPath, sPath );
  Wt::WAnchor* pAnchor = new Wt::WAnchor( link, sAnchor, pContainer );
}

void AppNodeStar::HandleAuthLoginChanged() {
  
  root()->clear();
  root()->addWidget( new Wt::WText( "Login Changed - " ) );

  if ( m_pAuth->LoggedIn() ) {
    Wt::log("notice") << "AppNodeStar User " /* << m_SignIn.user().id() */ << " logged in.";
  }
  else {
    Wt::log("notice") << "AppNodeStar User logged out.";
  }
  
  if ( "/home" == internalPath() ) {
  }
  else {
    setInternalPath( "/home", false ); // a true on this will generate an exception
  }

  // ** submit as event, causes an error
  //ShowMainMenu( root() );
}

void AppNodeStar::ShowSignIn( Wt::WContainerWidget* pcw ) {
  // upon signin, this widget still shows, so need to detect sign in somehow, and change state
  //  and show that widget
  // and a bunch of sessions have been created, then destroyed once logged in and the new menu supplied
  pcw->addWidget( new Wt::WText( "Show Sign In - " ) );
  ShowHome( pcw );
  //ShowMainMenu( pcw );
  // need to register this, or put in menu somewhere, so can call on demand
  // this needs to be added to the global page, and the page decorated depending upon where we are
  pcw->addWidget( m_pAuth->NewAuthWidget() );  // stateful show of: login, register, logged in
}

void AppNodeStar::ShowHome( Wt::WContainerWidget* pcw ) {

  std::string sTitle( "NodeStar: Network Infrastructure Data Management" );
  setTitle( sTitle );
  auto pTitle( new Wt::WText( "NodeStar: Network Infrastructure Data Management" ) );
  pTitle->setStyleClass( "MainTitle" );
  
  root()->addWidget( pTitle );

  pcw->addWidget( new Wt::WText( "Home Main Menu" ) );

  AddLink( pcw, "admin", "/home", "Home" );
  
}

void AppNodeStar::ShowMainMenu( Wt::WContainerWidget* pcw ) {
  
  ShowHome( pcw );
  
  AddLink( pcw, "admin", "/show/addresses", "Address List" );
  
  AddLink( pcw, "admin", "/admin/tables/upload", "Upload" );
  
  // still need to show the widget, and there needs to be a click event so when 
  //  logged in we can refresh
  // so devote a page to the login, clean out main menu,
  // but still need an event on when logged in is successful
  //   because nothing else happens on the page
  
  if ( m_pAuth->LoggedIn() ) {
    
    // <a id="ov7qcp1" 
    //    href="admin/tables/populate/mysql?wtd=jLpA57e4vgIIoYxI" 
    //    class="Wt-rr"><span id="ov7qcp0">Populate Tables: MySQL sourced</span>
    //    </a>
    auto pMenu = new Wt::WContainerWidget( pcw );
    pMenu->setList(true); // ==> sub WContainerWidget added as <li> elements
    AddLink( pMenu, "admin", "/admin/tables/init", "Init Tables" );
    AddLink( pMenu, "admin", "/admin/tables/populate/basics", "Populate Tables: Basics" );
    // use the Upload class to do this one:
    AddLink( pMenu, "admin", "/ad min/tables/populate/mysql",  "Populate Tables: MySQL sourced" );
    // use the Upload class to do this one:
    AddLink( pMenu, "admin", "/admin/tables/populate/smcxml", "Populate Tables: SMC XML sourced" );
    
    pcw->addWidget( m_pAuth->NewAuthWidget() );

  }
  else {
    pcw->addWidget( new Wt::WText( "Not Logged In" ) );
    // if not logged in, show link to sign in
    // if logged in, put log out button.
    AddLink( pcw, "admin", "/auth/signin", "Sign In" );
  }
  
  // sample button code, but now implemented as a link
  //Wt::WPushButton* pBtn = new Wt::WPushButton( "Show Addresses" );
  //pBtn->clicked().connect(this, &AppNodeStar::HandleShowAddresses );
  //pcw->addWidget( pBtn );
  
}

void AppNodeStar::ShowDefault( Wt::WContainerWidget* pcw ) {
  pcw->addWidget( new Wt::WText( "Show Default -  " ) );
  ShowMainMenu( pcw );
}

void AppNodeStar::Upload( Wt::WContainerWidget* pcw ) {
  ::Upload* upload( new ::Upload( pcw ) );  
}

void AppNodeStar::ShowAddresses( Wt::WContainerWidget* pcw ) {
  ShowHome( pcw );
  pcw->addWidget( new Wt::WText( "Addresses:" ) );
  auto p( new ::ShowAddresses( pcw, m_Session ) );
}
