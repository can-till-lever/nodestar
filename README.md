Network Infrastructure Data Management

Current repository based upon build files from within a NetBeans C++ environment.  This is only the beginning.
Nothing in operation yet.

Dependencies:
<br>https://www.over-yonder.net/~fullermd/projects/libcidr:  for manipulation and maintaining ip addresses
<br>http://www.boost.org/users/history/version_1_56_0.html:  general C++ library
<br>git clone git://github.com/kdeforche/wt.git:  web application environment

Build notes for boost and wt:
<br>http://blog.raymond.burkholder.net/index.php?/archives/607-Installing-Wt-on-Ubuntu.html

2014/09/22

Defined the initial schema for two PostgreSQL tables:  organization, ipaddress.  The schema relies on PostgreSQL's 
special datatypes uuid and cidr, so is not portable to other SQL implementations.  Wt's ORM library 
used for defining and building the schema.
