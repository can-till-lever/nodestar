/* 
 * File:   TableOrganization.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on September 18, 2014, 5:23 PM
 */

#ifndef DBRECORGANIZATION_H
#define	DBRECORGANIZATION_H

#include <Wt/Dbo/Dbo>

namespace dbo = Wt::Dbo;

class DbRecIpAddress;
class DbRecOrganization;

namespace Wt {
  namespace Dbo {
    template<>
    struct dbo_traits<DbRecOrganization>: public dbo_default_traits {
      typedef std::string IdType;
      static IdType invalidId() {return std::string(); }
      static const char* surrogateIdField() { return 0; }
      static const char* versionField() { return 0; }
    };
  }
}

class DbRecOrganization {
public:
  
  DbRecOrganization( void ): nAsn( 0 ) {};
  DbRecOrganization( 
          const std::string& sIdOrganization_, 
          const std::string& sName_,
          const std::string& sDescription_,
          const std::string& sUrl_,
          int nAsn_ = 0
          ) : sIdOrganization( sIdOrganization_ ), sName( sName_ ), sDescription( sDescription_ ), sUrl( sUrl_), nAsn( nAsn_ )
  {}

  std::string sIdOrganization;
  std::string sName;
  std::string sDescription;
  std::string sUrl;
  int nAsn;
  
  typedef dbo::collection<dbo::ptr<DbRecIpAddress> > collectionIpAddresses_t;
  collectionIpAddresses_t collectionIpAddresses;
  
  template<class Action>
  void persist( Action& a ) {
    dbo::id( a, sIdOrganization, "idorganization" );
    dbo::field( a, sName, "name" );
    dbo::field( a, sDescription, "description" );
    dbo::field( a, sUrl, "url" );
    dbo::field( a, nAsn, "asn" );
    dbo::hasMany( a, collectionIpAddresses, dbo::ManyToOne, "idorganization" );
  }
  
protected:
private:
  
};

#endif	/* DBRECORGANIZATION_H */

