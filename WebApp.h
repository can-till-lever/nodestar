/* 
 * File:   WebApp.h
 * Author: sysadmin
 *
 * Created on September 22, 2014, 4:38 PM
 */

#ifndef WEBAPP_H
#define	WEBAPP_H

#include <Wt/WApplication>
#include <Wt/Dbo/backend/Postgres>

namespace dbo = Wt::Dbo;

class WebApp: public Wt::WApplication {
public:
  WebApp( const Wt::WEnvironment&, dbo::backend::Postgres& );
  virtual ~WebApp();
protected:
private:

};

#endif	/* WEBAPP_H */

