/* 
 * File:   TableOrganization.h
 * Author: sysadmin
 *
 * Created on September 18, 2014, 5:23 PM
 */

#ifndef TABLEORGANIZATION_H
#define	TABLEORGANIZATION_H

#include <Wt/Dbo/Dbo>

namespace dbo = Wt::Dbo;

class TableIpAddress;
class TableOrganization;

namespace Wt {
  namespace Dbo {
    template<>
    struct dbo_traits<TableOrganization>: public dbo_default_traits {
      typedef std::string IdType;
      static IdType invalidId() {return std::string(); }
      static const char* surrogateIdField() { return 0; }
      static const char* versionField() { return 0; }
    };
  }
}

class TableOrganization {
public:
  
  TableOrganization( void ): nAsn( 0 ) {};
  TableOrganization( 
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
  
  typedef dbo::collection<dbo::ptr<TableIpAddress> > collectionIpAddresses_t;
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

#endif	/* TABLEORGANIZATION_H */

