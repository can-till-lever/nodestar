Network Infrastructure Data Management

Current repository based upon build files from within a NetBeans C++ environment.  This is only the beginning.
Nothing in operation yet.

Dependencies:
<ul>
  <li>https://www.over-yonder.net/~fullermd/projects/libcidr:  for manipulation and maintaining CIDR addresses (IPv4 and IPv6) [requires ldconfig after make install]
  <li>http://www.boost.org/users/history/version_1_56_0.html:  general C++ library
  <li>git clone git://github.com/kdeforche/wt.git:  web application environment
  </ul>

Build notes for boost and wt:
<br>http://blog.raymond.burkholder.net/index.php?/archives/607-Installing-Wt-on-Ubuntu.html

2014/10/02

Added capability to parse a tab delimited file containing network addresses via boost::spirit.

2014/10/01

Can do a query like the following:

nodestar=# select idorganization_idorganization as cabr, ipaddress, name, description from ipaddress where ipaddress >>= '10.122.39.211/32' order by ipaddress;
 cabr |     ipaddress     |         name         | description 
------+-------------------+----------------------+-------------
 QVSL | 10.0.0.0/8        | rfc1918              | 
 QVSL | 10.122.39.0/24    | net-10.122.39.0/24   | 
 QVSL | 10.122.39.208/29  | Corp 1               | 
 QVSL | 10.122.39.211/32  | Host 2               | 
(4 rows)


2014/09/30
Mcafee Next Generation Firewall has a rule export capability.  This generates an XML file.
Wrote some initial code to parse the file, and find network addresses and place into database.
Next step is to add the networks in a hierarchy, and then to extract host addresses add them to the database.

2014/09/22

Defined the initial schema for two PostgreSQL tables:  organization, ipaddress.  The schema relies on PostgreSQL's 
special datatypes uuid and cidr, so is not portable to other SQL implementations.  Wt's ORM library 
used for defining and building the schema.

Pre-populating ipaddress tables with some common reservations.

Added the Wt wrapper so can now start creating some user interface stuff.
