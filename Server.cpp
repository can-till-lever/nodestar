/* 
 * File:   Server.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on January 25, 2016, 3:03 PM
 */

#include "Server.h"

Server::Server( 
  dbo::FixedSqlConnectionPool& pool,
  const std::string &wtApplicationPath, 
  const std::string &wtConfigurationFile 
)
: Wt::WServer( wtApplicationPath, wtConfigurationFile), m_pool( pool )
{
  
}

Server::Server( 
  int argc, 
  char *argv[], 
  dbo::FixedSqlConnectionPool& pool,
  const std::string &wtConfigurationFile 
) 
: Wt::WServer( argc, argv, wtConfigurationFile ), m_pool( pool )
{
  
}

Server::~Server() {
}

dbo::FixedSqlConnectionPool& Server::GetConnectionPool( void ) {
  return m_pool;
}

