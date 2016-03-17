/* 
 * File:   ShowAddresses.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on March 16, 2016, 1:00 PM
 */

#include <map>

#include <Wt/WTable>
#include <Wt/WText>

#include "../model/DbRecIpAddress.h"

#include "ShowAddresses.h"

// should all this occur in the constructor, or should we use a op()?)
ShowAddresses::ShowAddresses( Wt::WContainerWidget* pcw, dbo::Session& session ) {
  
  typedef dbo::ptr<DbRecIpAddress> ptrIpAddress_t;
  
  auto pTable = new Wt::WTable( pcw );
  
  int iRow( 0 );
  
  typedef std::map<boost::uuids::uuid,std::string> mapParents_t;
  mapParents_t mapParents;
  
  typedef dbo::collection<ptrIpAddress_t> ptrIpAddresses_t;
  try {
    dbo::Transaction transaction( session );
    //ptrIpAddresses_t ptrIpAddresses = session.query<ptrIpAddress_t>( "select * from ipaddress").where("source=?").bind(sSource);
    ptrIpAddresses_t ptrIpAddresses = session.find<DbRecIpAddress>().orderBy( "ipaddress");
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

ShowAddresses::~ShowAddresses() {
}

