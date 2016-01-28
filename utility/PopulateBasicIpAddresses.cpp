/* 
 * File:   PopulateBasicIpAddresses.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on September 19, 2014, 3:39 PM
 */

#include <string>

#include "../model/DbRecOrganization.h"
#include "../model/DbRecIpAddress.h"

#include "PopulateBasicIpAddresses.h"

void PopulateBasicIpAddresses( dbo::Session& session ) {
  
  try {
    dbo::Transaction transaction( session );
     
    DbRecOrganization* piana = new DbRecOrganization();
    piana->sIdOrganization = "IANA";
    piana->sName = "Internet Assigned Numbers Authority";
    piana->sDescription = "responsible for the global coordination of the DNS Root, IP addressing, and other Internet protocol resources";
    piana->sUrl = "http://www.iana.org";
    dbo::ptr<DbRecOrganization> ptrIana  = session.add( piana );
    
    DbRecOrganization* parin = new DbRecOrganization();
    parin->sIdOrganization = "ARIN";
    parin->sName = "American Registry For Internet Numbers";
    parin->sDescription = "provides services related to the technical coordination and management of Internet number resources";
    parin->sUrl = "https://www.arin.net/";
    dbo::ptr<DbRecOrganization> ptrArin  = session.add( parin );
    
    // organization assignments below may not be correct, may need some additional proofing
    // also needs the IPv6 equivalents
    // ordering below is important as prior record may be parent of following record
   
    // http://en.wikipedia.org/wiki/List_of_assigned_/8_IPv4_address_blocks
    // http://en.wikipedia.org/wiki/Reserved_IP_addresses
    std::string sSource( "PopulateBasicIpAddresses" );
    dbo::ptr<DbRecIpAddress> ptrIp;
    dbo::ptr<DbRecIpAddress> ptrIp1;
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "0.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrArin, "10.0.0.0/8", "reserved", "RFC1918", "", "http://tools.ietf.org/html/rfc1918", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "100.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, ptrIp, "100.64.0.0/10", "reserved", "RFC6598 Carrier Grade NAT", "", "http://tools.ietf.org/html/rfc6598", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "127.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "169.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, ptrIp, "169.254.0.0/16", "reserved", "RFC3927 autoconfiguration", "", "http://tools.ietf.org/html/rfc3927", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "172.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrArin, ptrIp, "172.16.0.0/12", "reserved", "RFC1918", "", "http://tools.ietf.org/html/rfc1918", sSource ) );
    ptrIp1 = session.add( new DbRecIpAddress( ptrIana, "192.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, ptrIp1, "192.0.0.0/24", "reserved", "RFC5736", "", "http://tools.ietf.org/html/rfc5736", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, ptrIp1, "192.0.2.0/24", "reserved", "RFC5737 TEST-NET-1", "", "http://tools.ietf.org/html/rfc5737", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrArin, ptrIp1, "192.168.0.0/16", "reserved", "RFC1918", "", "http://tools.ietf.org/html/rfc1918", sSource ) );
    ptrIp1 = session.add( new DbRecIpAddress( ptrIana, "198.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, ptrIp1, "198.18.0.0/15", "reserved", "RFC5735", "", "http://tools.ietf.org/html/rfc5735", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, ptrIp1, "198.51.100.0/24", "reserved", "RFC5735 TEST-NET-2", "", "http://tools.ietf.org/html/rfc5735", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "203.0.0.0/8", "reserved", "", "", "", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, ptrIp, "203.0.113.0/24", "reserved", "RFC5737 TEST-NET-3", "", "http://tools.ietf.org/html/rfc5737", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "224.0.0.0/4", "multicast", "RFC5771", "", "http://tools.ietf.org/html/rfc5771", sSource ) );
    ptrIp = session.add( new DbRecIpAddress( ptrIana, "240.0.0.0/4", "future use", "RFC1112", "", "http://tools.ietf.org/html/rfc1112", sSource ) );

    transaction.commit();
  
  }
  catch (...) {
    std::cout << "PopulateBasicIpAddresses failed" << std::endl;
  }
}
