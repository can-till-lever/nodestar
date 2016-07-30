/* 
 * File:   DbSession.h
 * Author: raymond@burkholder.net
 *
 * Created on January 2, 2015, 4:59 PM
 * 
 * Used for user session login and management as outlined by wt's examples
 * Modelled after WhenToBe's Sesssion.x files
 * 
 */

#pragma once

#include <Wt/Dbo/FixedSqlConnectionPool>
#include <Wt/Auth/Dbo/UserDatabase>
#include <Wt/Auth/Login>

#include "DbRecUser.h"

namespace dbo = Wt::Dbo;

// need to check this out for fit into the scheme of things
typedef Wt::Auth::Dbo::UserDatabase<AuthInfoTables> UserDatabase;  // AuthInfoRecord is typdef in DbRecUser

class UserAuth {
public:
  
  UserAuth( dbo::FixedSqlConnectionPool& pool );
  virtual ~UserAuth();
  
  Wt::Auth::AbstractUserDatabase& Users();
//  UserDatabase* Users();
  
  static void MapClasses( dbo::Session& session );  
  void InitializeTables( void );  // empty for now, but useful to pre-populate at some point
  
  static void configureAuth();
  
  Wt::Auth::Login& login() { return m_SignIn; }
  
  static const Wt::Auth::AuthService& auth();
  static const Wt::Auth::PasswordService& passwordAuth();
  static const std::vector<const Wt::Auth::OAuthService *>& oAuth();
  
  // ** linkAuthEvent ** in Auth.ccp
  void authEvent() {
    if ( m_SignIn.loggedIn() )
      Wt::log("notice") << "User " << m_SignIn.user().id()
			<< " logged in.";
    else
      Wt::log("notice") << "User logged out.";
  };
  
protected:
private:

  dbo::ptr<DbRecUser> user() const;  // does this belong here?
  
  dbo::Session m_session;
  UserDatabase* m_users;
  
  Wt::Auth::Login m_SignIn;
  
  void CreateTables( void );  // should now be obsolete, need to check if called

};

