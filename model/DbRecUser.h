/* 
 * File:   DbRecUser.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on December 5, 2014, 5:56 PM
 */

#pragma once

#include <Wt/Dbo/Types>
#include <Wt/WGlobal>

namespace dbo = Wt::Dbo;

class DbRecUser;
typedef Wt::Auth::Dbo::AuthInfo<DbRecUser> AuthInfo;

class DbRecUser {
public:
  
  DbRecUser();
  virtual ~DbRecUser();
  
  template<class Action>
  void persist(Action& a) {
    // add more once the table is filled by default wt stuff
  }
  
protected:
private:

};

DBO_EXTERN_TEMPLATES(DbRecUser);
