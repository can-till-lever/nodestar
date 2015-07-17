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

#include "DbSessionUser.h"

namespace { // anonymous

  class MyOAuth : public std::vector<const Wt::Auth::OAuthService *>
  {
  public:
    ~MyOAuth()
    {
      for (unsigned i = 0; i < size(); ++i)
	delete (*this)[i];
    }
  };

  Wt::Auth::AuthService myAuthService;
  Wt::Auth::PasswordService myPasswordService(myAuthService);
  MyOAuth myOAuthServices;

} // namespace anonymous

void DbSessionUser::configureAuth()
{
  myAuthService.setAuthTokensEnabled(true, "logincookie");
  myAuthService.setEmailVerificationEnabled(true);

  Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
  verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
  myPasswordService.setVerifier(verifier);
  myPasswordService.setAttemptThrottlingEnabled(true);
  myPasswordService.setStrengthValidator
    (new Wt::Auth::PasswordStrengthValidator());

  if (Wt::Auth::GoogleService::configured())
    myOAuthServices.push_back(new Wt::Auth::GoogleService(myAuthService));

  if (Wt::Auth::FacebookService::configured())
    myOAuthServices.push_back(new Wt::Auth::FacebookService(myAuthService));
}

DbSessionUser::DbSessionUser( dbo::Session& session )
  : m_session( session )
{
  // m_connection.setProperty("show-queries", "true"); // set elsewhere, probably main

  //setConnection(m_connection);

  // from http://www.webtoolkit.eu/wt/doc/reference/html/classWt_1_1Auth_1_1Dbo_1_1AuthInfo.html
  m_session.mapClass<DbRecUser>("user");
  m_session.mapClass<AuthInfo>("auth_info");
  m_session.mapClass<AuthInfo::AuthIdentityType>("auth_identity");
  m_session.mapClass<AuthInfo::AuthTokenType>("auth_token");

  /* session will need to take care of this elsewhere 
  try {
    m_connection.createTables();  // this sh
    std::cerr << "Created database." << std::endl;
  } 
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Using existing database";
  }
   */ 

  // what is the UserDatabase?
  // does the above createTables need to be performed before this call works?
  m_users = new UserDatabase(m_session);
}

DbSessionUser::~DbSessionUser() {
  delete m_users;
}

Wt::Auth::AbstractUserDatabase& DbSessionUser::users() {
  return *m_users;
}

dbo::ptr<DbRecUser> DbSessionUser::user() const {
  if (m_login.loggedIn()) {
    dbo::ptr<AuthInfo> authInfo = m_users->find(m_login.user());
    return authInfo->user();
  } 
  else
    return dbo::ptr<DbRecUser>();
}

const Wt::Auth::AuthService& DbSessionUser::auth() {
  return myAuthService;
}

const Wt::Auth::PasswordService& DbSessionUser::passwordAuth() {
  return myPasswordService;
}

const std::vector<const Wt::Auth::OAuthService *>& DbSessionUser::oAuth() {
  return myOAuthServices;
}

