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

  std::string sIdOrganization;
  std::string sName;
  std::string sDescription;
  int nAsn;
  std::string sUrl;
  
  typedef dbo::collection<dbo::ptr<TableIpAddress> > collectionIpAddresses_t;
  collectionIpAddresses_t collectionIpAddresses;
  
  template<class Action>
  void persist( Action& a ) {
    dbo::id( a, sIdOrganization, "idorganization" );
    dbo::field( a, sName, "name" );
    dbo::field( a, sDescription, "description" );
    dbo::field( a, nAsn, "asn" );
    dbo::field( a, sUrl, "url" );
    dbo::hasMany( a, collectionIpAddresses, dbo::ManyToOne, "idorganization" );
  }
  
protected:
private:
  
};

#endif	/* TABLEORGANIZATION_H */

