/* 
 * File:   WCAdmin.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on February 1, 2016, 4:08 PM
 */

#include <Wt/WLink>
#include <Wt/WAnchor>
#include <Wt/WText>

#include "WCAdmin.h"

WCAdmin::WCAdmin( Wt::WContainerWidget *parent )
: Wt::WContainerWidget( parent )
{
  // <a id="ov7qcp1" 
  //    href="admin/tables/populate/mysql?wtd=jLpA57e4vgIIoYxI" 
  //    class="Wt-rr"><span id="ov7qcp0">Populate Tables: MySQL sourced</span>
  //    </a>
  // following is in the dependent order, top first
  setList(true); // ==> sub WContainerWidget added as <li> elements
  AddLink( "admin", "/admin/tables/init", "Init Tables" );
  AddLink( "admin", "/admin/tables/populate/basics", "Populate Tables: Basics" );
  AddLink( "admin", "/admin/tables/populate/mysql",  "Populate Tables: MySQL sourced" );
  AddLink( "admin", "/admin/tables/populate/smcxml", "Populate Tables: SMC XML sourced" );
  // populate basics should actually auto-execute after the init
  // basics are from in-line code

  
  // these will clear their corresponding? 
  // this will clear mysql and smc entries
  // need ability to upload this file

  // this may be dependent upon the previous table
  // this clears only the smc prior to repopulation
  // need ability to upload this file

}

WCAdmin::~WCAdmin() {
}

void WCAdmin::AddLink( const std::string& sClass, const std::string& sPath, const std::string& sAnchor ) {
  auto pContainer = new Wt::WContainerWidget( this );
  pContainer->setStyleClass( sClass );
  Wt::WLink link( Wt::WLink::InternalPath, sPath );
  Wt::WAnchor* pAnchor = new Wt::WAnchor( link, sAnchor, pContainer );
}