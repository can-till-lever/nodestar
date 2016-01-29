/* 
 * File:   WebApp.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on September 22, 2014, 4:38 PM
 */

#include <sstream>
#include <map>

#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>

#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "model/DbRecOrganization.h"
#include "model/DbRecIpAddress.h"

#include "AppNodeStar.h"

namespace {

}

typedef dbo::ptr<DbRecOrganization> ptrOrganization_t;
typedef dbo::ptr<DbRecIpAddress> ptrIpAddress_t;

AppNodeStar::AppNodeStar( const Wt::WEnvironment& env ): Wt::WApplication( env ), m_pServer( 0 ), m_pTable( 0 ) {
  
  m_pServer = dynamic_cast<Server*>( env.server() );
  m_pUserAuth.reset( new UserAuth( m_pServer->GetConnectionPool() ) );
  
  setTitle( "NodeStar: Network Infrastructure Data Management" );
  //root()->addWidget( new Wt::WText( "More to Come" ) );
  
  m_Session.setConnectionPool( m_pServer->GetConnectionPool() );
  
  m_Session.mapClass<DbRecOrganization>( "organization" );
  m_Session.mapClass<DbRecIpAddress>( "ipaddress" ); 
  
  Wt::WPushButton* p = new Wt::WPushButton( "select" );
  root()->addWidget( p );
  p->clicked().connect(this, &AppNodeStar::HandleShowAddresses );
  
  m_pTable = new Wt::WTable( root() );
  root()->addWidget( m_pTable );
  
}

AppNodeStar::~AppNodeStar() {
}

void AppNodeStar::HandleShowAddresses( const Wt::WMouseEvent& event ) {
  
  
  m_pTable->clear();
  
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
        if ( 0 != iter->get()->ptrParent ) {
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
      if ( 0 != iter->get()->ptrParent ) {
        parent = iter->get()->ptrParent->uuidId;
        mapParents_t::const_iterator iter = mapParents.find( parent );
        if ( mapParents.end() == iter )
          prefix = "??";
        else 
          prefix = iter->second;
      }
      std::stringstream ss;
      ss << iter->get()->cidrIpAddress;
      m_pTable->elementAt( iRow, 0 )->addWidget( new Wt::WText( prefix + ss.str() ) );
      m_pTable->elementAt( iRow, 1 )->addWidget( new Wt::WText( iter->get()->sName ) );
      m_pTable->elementAt( iRow, 2 )->addWidget( new Wt::WText( iter->get()->sDescription ) );
      ++iRow;
    }
    transaction.commit();  
  }
  catch ( dbo::Exception& e ) {
    std::cout << "AppNodeStar error " << e.what() << std::endl;
  }
}

void AppNodeStar::initialize() {
}

void AppNodeStar::finalize() {
}

/*
  

  }
 *  */