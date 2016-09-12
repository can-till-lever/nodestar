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
  m_connectionLoginChanged = m_pAuth->RegisterLoginChanged( this, &AppNodeStar::HandleAuthLoginChangedStep1 );
  
  namespace args = boost::phoenix::arg_names;

  RegisterPath( "/", boost::phoenix::bind( &AppNodeStar::HomeRoot, this, args::arg1 ) );
  RegisterPath( "/home", boost::phoenix::bind( &AppNodeStar::Home, this, args::arg1 ) );

  RegisterPath( "/auth/signin", boost::phoenix::bind( &AppNodeStar::AuthSignIn, this, args::arg1 ) );
  RegisterPath( "/auth/expired", boost::phoenix::bind( &AppNodeStar::AuthExpired, this, args::arg1 ) );
  RegisterPath( "/auth/signout", boost::phoenix::bind( &AppNodeStar::AuthSignOut, this, args::arg1 ) );
  RegisterPath( "/goodbye", boost::phoenix::bind( &AppNodeStar::GoodBye, this, args::arg1 ) );

  RegisterPath( "/member/home", boost::phoenix::bind( &AppNodeStar::MemberHome, this, args::arg1 ) );
  RegisterPath( "/member/show/addresses", boost::phoenix::bind( &AppNodeStar::MemberShowAddresses, this, args::arg1 ) );
  RegisterPath( "/member/upload/tables", boost::phoenix::bind( &AppNodeStar::MemberUploadTables, this, args::arg1 ) );
  
  struct callback {
    void operator()( Wt::WContainerWidget* pcw ) {
      
    }
  };
  TemplatePage( root(), callback() );

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
  std::cout << "HandleInternalPathChanged: " << sPath << " ";
  mapInternalPathChanged_t::const_iterator iter = m_mapInternalPathChanged.find( sPath );
  if ( m_mapInternalPathChanged.end() != iter ) {
    std::cout << "iter" << std::endl;
    iter->second( root() );
  }
  else {
    // default home page, or error page, and register a default page
    std::cout << "root" << std::endl;
    Home( root() );
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

void AppNodeStar::HandleAuthLoginChangedStep1() {
  std::cout << "auth step1" << std::endl;
  // ** submit as event, otherwise gui changes will cause an error
  //m_pServer->post( sessionId(), boost::phoenix::bind( &AppNodeStar::HandleAuthLoginChangedStep2, this ) );
  HandleAuthLoginChangedStep2();
}

void AppNodeStar::HandleAuthLoginChangedStep2( void ) {
  // 2016/09/04:   goto the logged out page (or special leaving page), or to the /member/home page)
  //root()->clear();
  //root()->addWidget( new Wt::WText( "Login Changed - " ) );
  std::cout << "auth step2" << std::endl;
  if ( m_pAuth->LoggedIn() ) {
    Wt::log("notice") << "AppNodeStar User " /* << m_SignIn.user().id() */ << " logged in.";
    setInternalPath( "/member/home", false );
    //HandleInternalPathChanged( internalPath() );
    //redirect( "/member/home" );
    //this->refresh();
  }
  else {
    Wt::log("notice") << "AppNodeStar User logged out.";
    setInternalPath( "/goodbye", false );
    //HandleInternalPathChanged( internalPath() );
    //redirect( "/goodbye" );
    //this->refresh();
  }
  
  //root()->clear();
  HandleInternalPathChanged( internalPath() );
//  ShowMainMenu( root() );
}

void AppNodeStar::MemberHome( Wt::WContainerWidget* pcw ) {
  struct callback {
    void operator()( Wt::WContainerWidget* pcw ) {
      auto p( new Wt::WText( "Member Status and Operations" ) );
      p->setStyleClass( "MemberHome" );
      pcw->addWidget( p );      
    }
  };
  std::cout << "memberHome" << std::endl;
  TemplatePageMember( pcw, callback() );
}

void AppNodeStar::AuthSignIn( Wt::WContainerWidget* pcw ) {
  struct callback {  // show widget only if not logged in
    void operator()( Wt::WContainerWidget* pcw ) {
      if ( 0 != m_p ) pcw->addWidget( m_p ); 
    }
    callback( Wt::WWidget* p ): m_p( p ) {}
    Wt::WWidget* m_p;
  };
  
  TemplatePage( pcw, callback( m_pAuth->LoggedIn() ? 0 : m_pAuth->NewAuthWidget() ) );
}

void AppNodeStar::AuthSignOut( Wt::WContainerWidget* pcw ) {
  struct callback {  // show widget only if not logged in
    void operator()( Wt::WContainerWidget* pcw ) {
      if ( 0 != m_p ) pcw->addWidget( m_p ); 
    }
    callback( Wt::WWidget* p ): m_p( p ) {}
    Wt::WWidget* m_p;
  };
  
  if ( m_pAuth->LoggedIn() ) {
    m_pAuth->LogOut();
  }
  
  TemplatePage( pcw, callback( 0 ) );
}

void AppNodeStar::AuthExpired( Wt::WContainerWidget* pcw ) {
  struct callback {
    void operator()( Wt::WContainerWidget* pcw ) {
      auto p( new Wt::WText( "Session Has Expired:" ) );
      p->setStyleClass( "AuthExpired" );
      pcw->addWidget( p );
    }
  };
  TemplatePage( pcw, callback() );
}

void AppNodeStar::HomeRoot( Wt::WContainerWidget* pcw ) {
  std::cout << "root home" << std::endl;
  namespace args = boost::phoenix::arg_names;

  TemplatePage( pcw, boost::phoenix::bind( &AppNodeStar::ShowMainMenu, this, args::arg1 ) );
}

void AppNodeStar::Home( Wt::WContainerWidget* pcw ) {
  std::cout << "main home" << std::endl;
  namespace args = boost::phoenix::arg_names;

  TemplatePage( pcw, boost::phoenix::bind( &AppNodeStar::ShowMainMenu, this, args::arg1 ) );
}

void AppNodeStar::GoodBye( Wt::WContainerWidget* pcw ) {
  std::cout << "goodbye" << std::endl;
  namespace args = boost::phoenix::arg_names;

  TemplatePage( pcw, boost::phoenix::bind( &AppNodeStar::ShowMainMenu, this, args::arg1 ) );
}

void AppNodeStar::ShowMainMenu( Wt::WContainerWidget* pcw ) {
  
  // still need to show the widget, and there needs to be a click event so when 
  //  logged in we can refresh
  // so devote a page to the login, clean out main menu,
  // but still need an event on when logged in is successful
  //   because nothing else happens on the page
  
  if ( m_pAuth->LoggedIn() ) {

    AddLink( pcw, "member", "/show/addresses", "Address List" );
    AddLink( pcw, "member", "/admin/tables/upload", "Upload" );
    
    // <a id="ov7qcp1" 
    //    href="admin/tables/populate/mysql?wtd=jLpA57e4vgIIoYxI" 
    //    class="Wt-rr"><span id="ov7qcp0">Populate Tables: MySQL sourced</span>
    //    </a>
    auto pMenu = new Wt::WContainerWidget( pcw );
    // test against database, and figure out which can be shown, particularily the tables/init one
    pMenu->setList(true); // ==> sub WContainerWidget added as <li> elements
    AddLink( pMenu, "admin", "/admin/tables/init", "Init Tables" );
    AddLink( pMenu, "admin", "/admin/tables/populate/basics", "Populate Tables: Basics" );
    // use the Upload class to do this one:
    AddLink( pMenu, "admin", "/ad min/tables/populate/mysql",  "Populate Tables: MySQL sourced" );
    // use the Upload class to do this one:
    AddLink( pMenu, "admin", "/admin/tables/populate/smcxml", "Populate Tables: SMC XML sourced" );
    
  }
  else {
    
  }
  
  // sample button code, but now implemented as a link
  //Wt::WPushButton* pBtn = new Wt::WPushButton( "Show Addresses" );
  //pBtn->clicked().connect(this, &AppNodeStar::HandleShowAddresses );
  //pcw->addWidget( pBtn );
  
}

void AppNodeStar::MemberUploadTables( Wt::WContainerWidget* pcw ) {
  ::Upload* upload( new ::Upload( pcw ) );  
}

void AppNodeStar::MemberShowAddresses( Wt::WContainerWidget* pcw ) {
  struct callback {
    void operator()( Wt::WContainerWidget* pcw ) {
      auto pAddresses( new Wt::WText( "Addresses:" ) );
      pAddresses->setStyleClass( "MemberShowAddresses" );
      pcw->addWidget( pAddresses );

      auto p( new ::ShowAddresses( pcw, m_session ) );
    }
    callback( dbo::Session& session ): m_session( session ) {}
    dbo::Session& m_session;
  };
  TemplatePageMember( pcw, callback( m_Session ) );
}

void AppNodeStar::TemplatePage(Wt::WContainerWidget* pcw, FTemplate f) {
  
  std::string sTitle( "NodeStar: Network Infrastructure Data Management" );
  setTitle( sTitle );
  auto pTitle( new Wt::WText( "NodeStar: Network Infrastructure Data Management" ) );
  pTitle->setStyleClass( "MainTitle" );
  
  pcw->addWidget( pTitle );
  
    // should show up to the right of the title
  if ( this->internalPathMatches( "/auth/signin" ) ) {
    // don't show sign in status
  }
  else {
    if ( m_pAuth->LoggedIn() ) {
      //pcw->addWidget( m_pAuth->NewAuthWidget() );
      AddLink( pcw, "member", "/auth/signout", "Sign Out" );
      AddLink( pcw, "member", "/member/home", "Home" );
    }
    else {
      AddLink( pcw, "admin", "/auth/signin", "Sign In" );
      AddLink( pcw, "default", "/home", "Home" );
    }
    
  }

  

  f( pcw );
}

void AppNodeStar::TemplatePageMember(Wt::WContainerWidget* pcw, FTemplate f) {
  struct callback {
    void operator()( Wt::WContainerWidget* pcw ) {
      std::cout << "callback ";
      auto pMemberTop( new Wt::WText( "member top" ) );
      pMemberTop->setStyleClass( "MemmberTop" );
      pcw->addWidget( pMemberTop );
      
      m_f( pcw );

      auto pMemberBottom( new Wt::WText( "member bottom" ) );
      pMemberBottom->setStyleClass( "MemberBottom" );
      pcw->addWidget( pMemberBottom );
    }
    callback( FTemplate f ): m_f( f ) {}
    FTemplate m_f;
  };
  std::cout << "templatepagemember ";
  TemplatePage( pcw, callback( f ) );
  std::cout << "done" << std::endl;
}