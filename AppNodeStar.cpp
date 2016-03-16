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

#include "page/Upload.h"

#include "AppNodeStar.h"

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
  
  std::string sTitle( "NodeStar: Network Infrastructure Data Management" );
  setTitle( sTitle );
  auto pTitle( new Wt::WText( "NodeStar: Network Infrastructure Data Management" ) );
  pTitle->setStyleClass( "MainTitle" );
  
  root()->addWidget( pTitle );

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
  
  m_pAuth.reset( new Auth( m_pServer->GetConnectionPool() ) );
  
  
  ShowDefault( root() );
}

AppNodeStar::~AppNodeStar() {
}

void AppNodeStar::ShowAddresses( Wt::WContainerWidget* pcw ) {
  
  auto pTable = new Wt::WTable( pcw );
  
  int iRow( 0 );
  
  typedef std::map<boost::uuids::uuid,std::string> mapParents_t;
  mapParents_t mapParents;
  
  typedef dbo::collection<ptrIpAddress_t> ptrIpAddresses_t;
  try {
    dbo::Transaction transaction( m_Session );
    //ptrIpAddresses_t ptrIpAddresses = session.query<ptrIpAddress_t>( "select * from ipaddress").where("source=?").bind(sSource);
    ptrIpAddresses_t ptrIpAddresses = m_Session.find<DbRecIpAddress>().orderBy( "ipaddress");
    for ( ptrIpAddresses_t::iterator iter = ptrIpAddresses.begin(); iter != ptrIpAddresses.end(); ++iter ) {
      //iter->remove();
      std::string prefix;
      boost::uuids::uuid id = iter->get()->uuidId;
      if ( !iter->get()->children.empty() ) {
        if ( 0 != iter->get()->ptrParent.get() ) {
          boost::uuids::uuid parent = iter->get()->ptrParent->uuidId;
          mapParents_t::const_iterator iter = mapParents.find( parent );
          if ( mapParents.end() == iter )
            prefix = "??";
          else
            prefix = iter->second;
          
        }
        mapParents.insert( mapParents_t::value_type( id, prefix + "..") );
      }
      prefix = "";
      boost::uuids::uuid parent;
      if ( 0 != iter->get()->ptrParent.get() ) {
        parent = iter->get()->ptrParent->uuidId;
        mapParents_t::const_iterator iter = mapParents.find( parent );
        if ( mapParents.end() == iter )
          prefix = "??";
        else 
          prefix = iter->second;
      }
      std::stringstream ss;
      ss << iter->get()->cidrIpAddress;
      pTable->elementAt( iRow, 0 )->addWidget( new Wt::WText( prefix + ss.str() ) );
      pTable->elementAt( iRow, 1 )->addWidget( new Wt::WText( iter->get()->sName ) );
      pTable->elementAt( iRow, 2 )->addWidget( new Wt::WText( iter->get()->sDescription ) );
      ++iRow;
    }
    transaction.commit();  
  }
  catch ( dbo::Exception& e ) {
    std::cout << "AppNodeStar error " << e.what() << std::endl;
  }
  
}

void AppNodeStar::HandleShowAddresses( const Wt::WMouseEvent& event ) {
  root()->clear();
  this->setInternalPath( "/show/addresses", false );
  ShowMainMenu( root() );
  ShowAddresses( root() );  
}

void AppNodeStar::initialize() {
}

void AppNodeStar::finalize() {
}

void AppNodeStar::HandleInternalPathChanged( const std::string& sPath ) {
  root()->clear();
  std::cout << "HandleInternalPathChanged: " << sPath << std::endl;
  mapInternalPathChanged_t::const_iterator iter = m_mapInternalPathChanged.find( sPath );
  if ( m_mapInternalPathChanged.end() != iter ) {
    iter->second( root() );
  }
  else {
    // default home page, or error page, and register a default page
    ShowDefault( root() );
  }
}

void AppNodeStar::HandleInternalPathInvalid( const std::string& s ) {
  std::cout << "*** HandleInternalPathInvalid: " << s << std::endl;
}
  
void AppNodeStar::RegisterPath( const std::string& sPath, const slotInternalPathChanged_t& slot ) {
  mapInternalPathChanged_t::const_iterator iter = m_mapInternalPathChanged.find( sPath );
  if ( m_mapInternalPathChanged.end() != iter ) std::runtime_error( "path exists" );
  m_mapInternalPathChanged.insert( mapInternalPathChanged_t::value_type( sPath, slot ) );
}

void AppNodeStar::UnRegisterPath( const std::string& sPath ) {
  mapInternalPathChanged_t::const_iterator iter = m_mapInternalPathChanged.find( sPath );
  if ( m_mapInternalPathChanged.end() == iter ) std::runtime_error( "path not found" );
  m_mapInternalPathChanged.erase( iter );
}

void AppNodeStar::AddLink( Wt::WContainerWidget* pcw, const std::string& sClass, const std::string& sPath, const std::string& sAnchor ) {
  auto pContainer = new Wt::WContainerWidget( pcw );
  pContainer->setStyleClass( sClass );
  Wt::WLink link( Wt::WLink::InternalPath, sPath );
  Wt::WAnchor* pAnchor = new Wt::WAnchor( link, sAnchor, pContainer );
}

void AppNodeStar::ShowSignIn( Wt::WContainerWidget* pcw ) {
  ShowMainMenu( pcw );
  // need to register this, or put in menu somewhere, so can call on demand
  pcw->addWidget( m_pAuth->NewAuthWidget() );  // stateful show of: login, register, logged in
}

void AppNodeStar::ShowDefault( Wt::WContainerWidget* pcw ) {
  ShowMainMenu( pcw );
}

void AppNodeStar::ShowMainMenu( Wt::WContainerWidget* pcw ) {
  
  namespace args = boost::phoenix::arg_names;
  
  AddLink( pcw, "admin", "/", "Home" );
  RegisterPath( "/", boost::phoenix::bind( &AppNodeStar::ShowDefault, this, args::arg1 ) );

  AddLink( pcw, "admin", "/auth/signin", "Sign In" );
  RegisterPath( "/auth/signin", boost::phoenix::bind( &AppNodeStar::ShowSignIn, this, args::arg1 ) );
  
  AddLink( pcw, "admin", "/show/addresses", "Address List" );
  RegisterPath( "/show/addresses", boost::phoenix::bind( &AppNodeStar::ShowAddresses, this, args::arg1 ) );
  
  AddLink( pcw, "admin", "/admin/tables/upload", "Upload" );
  RegisterPath( "/admin/tables/upload", boost::phoenix::bind( &AppNodeStar::Upload, this, args::arg1 ) );
  
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
    AddLink( pMenu, "admin", "/admin/tables/populate/mysql",  "Populate Tables: MySQL sourced" );
    // use the Upload class to do this one:
    AddLink( pMenu, "admin", "/admin/tables/populate/smcxml", "Populate Tables: SMC XML sourced" );
  }
  
  // sample button code, but now implemented as a link
  //Wt::WPushButton* pBtn = new Wt::WPushButton( "Show Addresses" );
  //pBtn->clicked().connect(this, &AppNodeStar::HandleShowAddresses );
  //pcw->addWidget( pBtn );
  
}

void AppNodeStar::Upload( Wt::WContainerWidget* pcw ) {
  ::Upload* upload( new ::Upload( pcw ) );
  
}