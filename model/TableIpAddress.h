/* 
 * File:   TableIpAddress.h
 * Author: sysadmin
 *
 * Created on September 18, 2014, 5:26 PM
 */

#ifndef TABLEIPADDRESS_H
#define	TABLEIPADDRESS_H

#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/random_generator.hpp>
// http://www.boost.org/doc/libs/1_56_0/libs/uuid/uuid.html

#include <Wt/Dbo/Dbo>
#include <Wt/WDateTime>

#include <Wt/Dbo/WtSqlTraits>

#include "../cidr.hpp"

namespace dbo = Wt::Dbo;

class TableIpAddress;
class TableOrganization;

namespace Wt {
  namespace Dbo {
    template<>
    struct dbo_traits<TableIpAddress>: public dbo_default_traits {
      typedef boost::uuids::uuid IdType;
      static IdType invalidId() { return boost::uuids::uuid(); }
      static const char* surrogateIdField() { return 0; }
      static const char* versionField() { return 0; }
    };
  }
}

class TableIpAddress {
public:
  
  TableIpAddress( void ) {
    boost::uuids::random_generator gen;
    uuidId = gen();
    dtCreation = Wt::WDateTime::currentDateTime();
  }
  
  boost::uuids::uuid uuidId;
  dbo::ptr<TableIpAddress> ptrParent;
  cidr cidrIpAddress;
  dbo::ptr<TableOrganization> ptrOrganization;
  std::string sName;
  std::string sDescription;
  std::string sFqdn;
  std::string sUrl;
  std::string sSource; // software module used for creating the entry
  //boost::posix_time::ptime dtCreation;
  Wt::WDateTime dtCreation;
  
  typedef dbo::collection<dbo::ptr<TableIpAddress> > collectionIpAddresses_t;
  collectionIpAddresses_t children;
  
  template<class Action>
  void persist( Action& a ) {
    dbo::id( a, uuidId, "idipaddress" );
    dbo::field( a, cidrIpAddress, "ipaddress" );
    dbo::field( a, sName, "name" );
    dbo::field( a, sDescription, "description" );
    dbo::field( a, sFqdn, "fqdn" );
    dbo::field( a, sUrl, "url" );
    dbo::field( a, sSource, "source" );
    dbo::field( a, dtCreation, "datetimecreation" );
    dbo::belongsTo( a, ptrOrganization, "idorganization", dbo::NotNull );
    dbo::belongsTo( a, ptrParent, "parent" );
    dbo::hasMany( a, children, dbo::ManyToOne, "parent" );
  }
  
protected:
private:
};


#endif	/* TABLEIPADDRESS_H */

