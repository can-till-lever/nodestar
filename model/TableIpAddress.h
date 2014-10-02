/* 
 * File:   TableIpAddress.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on September 18, 2014, 5:26 PM
 */

#ifndef TABLEIPADDRESS_H
#define	TABLEIPADDRESS_H

// http://www.boost.org/doc/libs/1_56_0/libs/uuid/uuid.html
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#include <Wt/Dbo/Dbo>
#include <Wt/WDateTime>

#include <Wt/Dbo/WtSqlTraits>

#include "../SqlTraits.h"
#include "../Cidr.hpp"

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

// unused at the moment, to be completed
class QueryIpAddress {
public:
  dbo::ptr<TableIpAddress> ptrParent;
  Cidr cidrIpAddress;
protected:
private:
};

class TableIpAddress {
public:
  
  TableIpAddress( void ) { Init(); }
  TableIpAddress( 
          dbo::ptr<TableOrganization> ptrOrganization_, 
          const std::string& sIpAddress_, 
          const std::string& sName_,
          const std::string& sDescription_,
          const std::string& sFqdn_,
          const std::string& sUrl_,
          const std::string& sSource_
          ) : ptrOrganization( ptrOrganization_ ), cidrIpAddress( sIpAddress_ ), sName( sName_ ), sDescription( sDescription_ ), 
          sFqdn( sFqdn_ ), sUrl( sUrl_ ), sSource( sSource_ )
  {
    Init();
  }
  TableIpAddress( 
          dbo::ptr<TableOrganization> ptrOrganization_, 
          dbo::ptr<TableIpAddress> ptrParent_,
          const std::string& sIpAddress_, 
          const std::string& sName_,
          const std::string& sDescription_,
          const std::string& sFqdn_,
          const std::string& sUrl_,
          const std::string& sSource_
          ) : ptrOrganization( ptrOrganization_ ), ptrParent( ptrParent_ ),
          cidrIpAddress( sIpAddress_ ), sName( sName_ ), sDescription( sDescription_ ), 
          sFqdn( sFqdn_ ), sUrl( sUrl_ ), sSource( sSource_ )
  {
    Init();
  }
  
  boost::uuids::uuid uuidId;
  dbo::ptr<TableIpAddress> ptrParent;
  Cidr cidrIpAddress;
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
  void Init( void ) {
    boost::uuids::random_generator gen;
    uuidId = gen();
    dtCreation = Wt::WDateTime::currentDateTime();
  }
};


#endif	/* TABLEIPADDRESS_H */

