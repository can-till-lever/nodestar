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
  {
    auto pContainer = new Wt::WContainerWidget( this );
    pContainer->setStyleClass( "admin" );
    Wt::WLink link( Wt::WLink::InternalPath, "/admin/tables/init" );
    Wt::WAnchor* pAnchor = new Wt::WAnchor( link, "Init Tables", pContainer );
  }
  // populate basics should actually auto-execute after the init
  // basics are from in-line code
  {
    auto pContainer = new Wt::WContainerWidget( this );
    pContainer->setStyleClass( "admin" );
    Wt::WLink link( Wt::WLink::InternalPath, "/admin/tables/populate/basics" );
    Wt::WAnchor* pAnchor = new Wt::WAnchor( link, "Populate Tables: Basics", pContainer );
  }
  // these will clear their corresponding? 
  // this will clear mysql and smc entries
  // need ability to upload this file
  {
    auto pContainer = new Wt::WContainerWidget( this );
    pContainer->setStyleClass( "admin" );
    Wt::WLink link( Wt::WLink::InternalPath, "/admin/tables/populate/mysql" );
    Wt::WAnchor* pAnchor = new Wt::WAnchor( link, "Populate Tables: MySQL sourced", pContainer );
  }
  // this may be dependent upon the previous table
  // this clears only the smc prior to repopulation
  // need ability to upload this file
  {
    auto pContainer = new Wt::WContainerWidget( this );
    pContainer->setStyleClass( "admin" );
    Wt::WLink link( Wt::WLink::InternalPath, "/admin/tables/populate/smcxml" );
    Wt::WAnchor* pAnchor = new Wt::WAnchor( link, "Populate Tables: SMC XML sourced", pContainer );
  }
}

WCAdmin::~WCAdmin() {
}

