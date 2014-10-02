/* 
 * File:   ImportSmcXml.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on September 30, 2014, 10:44
 */

// example code:
// http://www.boost.org/doc/libs/1_56_0/doc/html/boost_propertytree/tutorial.html
// http://akrzemi1.wordpress.com/2011/07/13/parsing-xml-with-boost/

#include "ImportSmcXml.h"

// look for network: ipv4_network, name, comment
// look for host: name, comment, sub mvia_address
// look for router:  (multiple), logical_interface?, 
// look for physical_interface: comment, macaddress, sub vlan_interface: interface_id
// look for firewall_node, sub mvia_address

