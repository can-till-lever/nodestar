/* 
 * File:   Server.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on January 25, 2016, 3:03 PM
 */

#pragma once

#include <Wt/WServer>
#include <Wt/Dbo/FixedSqlConnectionPool>

namespace dbo = Wt::Dbo;

class Server: public Wt::WServer {
public:
  Server(
          dbo::FixedSqlConnectionPool& pool,
          const std::string &wtApplicationPath=std::string(), 
          const std::string &wtConfigurationFile=std::string()
          );
  Server(
          int argc, 
          char *argv[], 
          dbo::FixedSqlConnectionPool& pool,
          const std::string &wtConfigurationFile=std::string()
          );
  virtual ~Server();
  
  dbo::FixedSqlConnectionPool& GetConnectionPool( void );
  
protected:
private:
  dbo::FixedSqlConnectionPool& m_pool;
};


