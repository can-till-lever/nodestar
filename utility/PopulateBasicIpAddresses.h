/* 
 * File:   PopulateBasicIpAddresses.h
 * Author: sysadmin
 *
 * Created on September 19, 2014, 3:39 PM
 */

#ifndef POPULATEBASICIPADDRESSES_H
#define	POPULATEBASICIPADDRESSES_H

#include <Wt/Dbo/Dbo>
namespace dbo = Wt::Dbo;

// may need to be able to segment this and do rfc 1918 per organization

void PopulateBasicIpAddresses( dbo::Session& );

#endif	/* POPULATEBASICIPADDRESSES_H */

