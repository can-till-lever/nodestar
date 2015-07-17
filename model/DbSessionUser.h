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

#ifndef DBSESSIONUSER_H
#define	DBSESSIONUSER_H

#include <Wt/Auth/Login>

#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Session>
//#include <Wt/Dbo/backend/Sqlite3>
//#include <Wt/Dbo/backend/Postgres>

#include "DbRecUser.h"

namespace dbo = Wt::Dbo;

// need to check this out for fit into the scheme of things
typedef Wt::Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;

// 2015/02/18 this whole class might be a problem as it attempts to create the database tables.
//   need to merge the table creation elsewhere, possibly as a call back
//class DbSessionUser: public dbo::Session {
class DbSessionUser {
public:
  
  DbSessionUser( dbo::Session& session );
  virtual ~DbSessionUser();
  
  dbo::ptr<DbRecUser> user() const;
  
  Wt::Auth::AbstractUserDatabase& users();
  Wt::Auth::Login& login() { return m_login; }
  
  static void configureAuth();
  
  static const Wt::Auth::AuthService& auth();
  static const Wt::Auth::PasswordService& passwordAuth();
  static const std::vector<const Wt::Auth::OAuthService *>& oAuth();
  
private:

  //dbo::backend::Postgres& m_connection;  // this may not be correct, database may be opened elsewhere
  dbo::Session& m_session;
  UserDatabase* m_users;
  Wt::Auth::Login m_login;

};

#endif	/* DBSESSIONUSER_H */

