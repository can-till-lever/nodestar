/* 
 * File:   DbSession.cpp
 * Author: raymond@burkholder.net
 * 
 * Created on January 2, 2015, 4:59 PM
 */

#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"
#include "Wt/Auth/GoogleService"
#include "Wt/Auth/FacebookService"
#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"

#include "UserAuth.h"

namespace { // anonymous

  class MyOAuth : public std::vector<const Wt::Auth::OAuthService *> {
  public:
    ~MyOAuth()
    {
      for (unsigned i = 0; i < size(); ++i)
	delete (*this)[i];
    }
  };

  // sessions share these services, do not maintain transient session state
  // migrate to the Server class at some point, as these are fundamentally singletons
  Wt::Auth::AuthService myAuthService;
  Wt::Auth::PasswordService myPasswordService(myAuthService);
  MyOAuth myOAuthServices;

} // namespace anonymous

UserAuth::UserAuth( dbo::FixedSqlConnectionPool& pool ) {

  m_session.setConnectionPool( pool );
  
  MapClasses( m_session );
  //CreateTables();

  // creates structures for accessing the various records for maintaining information about the user
  m_users = new UserDatabase(m_session);  // to determine:  creating a new user here?  new user for the period of the app instantiation?
}

UserAuth::~UserAuth() {
  delete m_users;
}

void UserAuth::MapClasses( dbo::Session& session ) {
  // from http://www.webtoolkit.eu/wt/doc/reference/html/classWt_1_1Auth_1_1Dbo_1_1AuthInfo.html
  session.mapClass<DbRecUser>("auth");
  session.mapClass<AuthInfoTables>("authinfo");
  session.mapClass<AuthInfoTables::AuthIdentityType>("authidentity");
  session.mapClass<AuthInfoTables::AuthTokenType>("authtoken");
}

void UserAuth::CreateTables( void ) {
  try {
    m_session.createTables();  // this sh
    std::cerr << "Created database." << std::endl;
  } 
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Using existing UserAuth tables";
  }
}

void UserAuth::InitializeTables( void ) {
  MapClasses( m_session );
  //CreateTables();
}

dbo::ptr<DbRecUser> UserAuth::user() const {
  if (m_login.loggedIn()) {
    dbo::ptr<AuthInfoTables> authInfo = m_users->find( m_login.user() );
    return authInfo->user();
  } 
  else
    return dbo::ptr<DbRecUser>();
}

// static to one-time set services
void UserAuth::configureAuth() {
  
  myAuthService.setAuthTokensEnabled(true, "nodestar note");
  myAuthService.setEmailVerificationEnabled(false);   // don't do email at the moment

  Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
  verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
  myPasswordService.setVerifier(verifier);
  myPasswordService.setAttemptThrottlingEnabled(false);   // was true in example
//  myPasswordService.setStrengthValidator
//    (new Wt::Auth::PasswordStrengthValidator());

//  if (Wt::Auth::GoogleService::configured())
//    myOAuthServices.push_back(new Wt::Auth::GoogleService(myAuthService));

//  if (Wt::Auth::FacebookService::configured())
//    myOAuthServices.push_back(new Wt::Auth::FacebookService(myAuthService));
}

Wt::Auth::AbstractUserDatabase& UserAuth::Users() {
  return *m_users;
}

//UserDatabase* UserAuth::Users() {
//  return m_users;
//}

const Wt::Auth::AuthService& UserAuth::auth() {
  return myAuthService;
}

const Wt::Auth::PasswordService& UserAuth::passwordAuth() {
  return myPasswordService;
}

const std::vector<const Wt::Auth::OAuthService *>& UserAuth::oAuth() {
  return myOAuthServices;
}

