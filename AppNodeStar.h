/* 
 * File:   WebApp.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on September 22, 2014, 4:38 PM
 * 
 * access functions in cidr library to show ranges and such
 * parse zone files
 * parse stonegate xml file (wt has xml parser library?)
 */

#pragma once

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/function.hpp>

#include <Wt/Dbo/ptr>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>

// requires style/nodestar.css
// ln -s style /var/www/wt/style
// style also needs to be in --doc-root, which is a source of static files

#include "Auth.h"

#include "Server.h"

class AppNodeStar: public Wt::WApplication {
public:
  
  typedef boost::signals2::signal<void (Wt::WContainerWidget*)> signalInternalPathChanged_t;
  typedef signalInternalPathChanged_t::slot_type slotInternalPathChanged_t;
  
  AppNodeStar( const Wt::WEnvironment& );  // constructor as an application
  virtual ~AppNodeStar( void );
  
  void RegisterPath( const std::string& sPath, const slotInternalPathChanged_t& slot );
  void UnRegisterPath( const std::string& sPath );
  
  virtual void initialize( void ); // Initializes the application, post-construction. 
  virtual void finalize( void );  // Finalizes the application, pre-destruction.
  
protected:
private:
  
  typedef boost::shared_ptr<Auth> pAuth_t;
  pAuth_t m_pAuth;
  
  typedef boost::function<void(Wt::WContainerWidget*)> FTemplate;
  
  Server* m_pServer; // object managed by wt
  dbo::Session m_Session;

  typedef std::map<const std::string, const slotInternalPathChanged_t> mapInternalPathChanged_t;
  mapInternalPathChanged_t m_mapInternalPathChanged;
  
  Wt::Signals::connection m_connectionLoginChanged;
  
  void AddLink( Wt::WContainerWidget*, const std::string& sClass, const std::string& sPath, const std::string& sAnchor );

  void ShowMainMenu( Wt::WContainerWidget* );
  
  void HomeRoot( Wt::WContainerWidget* );
  void Home( Wt::WContainerWidget* );
  void AuthSignIn( Wt::WContainerWidget* );
  void AuthExpired( Wt::WContainerWidget* );
  void AuthSignOut( Wt::WContainerWidget* );
  void GoodBye( Wt::WContainerWidget* );
  
  void MemberHome( Wt::WContainerWidget* );
  void MemberShowAddresses( Wt::WContainerWidget* pcw );
  void MemberUploadTables( Wt::WContainerWidget* );
  
  void DropTables( Wt::WContainerWidget* );
  void CreateTables( Wt::WContainerWidget* );
  
  void HandleInternalPathChanged( const std::string& );
  void HandleInternalPathInvalid( const std::string& );
  
  void HandleAuthLoginChangedStep1( void );
  void HandleAuthLoginChangedStep2( void );
  
  void TemplatePage( Wt::WContainerWidget*, FTemplate );
  void TemplatePageMember( Wt::WContainerWidget*, FTemplate );
};
