/* 
 * File:   ImportCsv.h
 * 
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on October 1, 2014, 3:37 PM
 */

#pragma once

#include <string>
#include <fstream>

#define BOOST_SPIRIT_USE_PHOENIX_V3 1

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_symbols.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/spirit/include/support_istream_iterator.hpp>

struct NetworkCsvFields {
  std::string sRecordId;
  std::string sNetwork;
  std::string sSubnet;
  std::string sAllocatedDate;
  std::string sCustId;
  std::string sAddedBy;
  std::string sSwip;
  std::string sWhois;
  std::string sIssued;
  std::string sComments;
};

// based upon:
// select n_rec_id, n_network, n_subnet, n_allocated_date, n_cust_id, n_added_by, n_swip, n_whois, n_comments,n_issued from networks into outfile '/tmp/networks.csv';
// echo 'select n_rec_id, n_network, n_subnet, n_allocated_date, n_cust_id, n_added_by, n_swip, n_whois, n_comments,n_issued from networks' | mysql -p --user username --database dbname 
// where \n may exist in n_comments field

BOOST_FUSION_ADAPT_STRUCT(
  NetworkCsvFields,
  (std::string, sRecordId)
  (std::string, sNetwork)
  (std::string, sSubnet)
  (std::string, sAllocatedDate)
  (std::string, sCustId)
  (std::string, sAddedBy)
  (std::string, sSwip)
  (std::string, sWhois)
  (std::string, sComments)
  (std::string, sIssued)
  )

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
namespace ascii = boost::spirit::ascii;

template<typename Iterator>
struct NetworkCsvLineParser: qi::grammar<Iterator, NetworkCsvFields()> {
  NetworkCsvLineParser( void ): NetworkCsvLineParser::base_type(start) {
    
    rNotATab      %= qi::lexeme[ +( qi::char_ - '\t' ) ];
    rNotATabCr    %= qi::lexeme[ +( qi::char_ - '\t' - '\n' ) ];

    rRecordId      %= rNotATab;
    rNetwork       %= rNotATab;
    rSubnet        %= rNotATab;
    rAllocatedDate %= rNotATab;
    rCustId        %= rNotATab; 
    rAddedBy       %= rNotATab;
    rSwip          %= rNotATab;
    rWhois         %= rNotATab;
    rComments      %= rNotATab;
    rIssued        %= rNotATabCr;

    // note the '-' in front of optional fields
    start %=               rRecordId
      > qi::lit( '\t' ) >  rNetwork 
      > qi::lit( '\t' ) >  rSubnet   
      > qi::lit( '\t' ) > -rAllocatedDate 
      > qi::lit( '\t' ) > -rCustId
      > qi::lit( '\t' ) > -rAddedBy 
      > qi::lit( '\t' ) > -rSwip 
      > qi::lit( '\t' ) > -rWhois 
      > qi::lit( '\t' ) > -rComments 
      > qi::lit( '\t' ) > -rIssued 
      > ( qi::eol | qi::eps )
            ;

  }

  qi::rule<Iterator, std::string()> rNotATab;
  qi::rule<Iterator, std::string()> rNotATabCr;
  qi::rule<Iterator, std::string()> rRecordId;
  qi::rule<Iterator, std::string()> rNetwork;
  qi::rule<Iterator, std::string()> rSubnet;
  qi::rule<Iterator, std::string()> rAllocatedDate;
  qi::rule<Iterator, std::string()> rCustId;
  qi::rule<Iterator, std::string()> rAddedBy;
  qi::rule<Iterator, std::string()> rSwip;
  qi::rule<Iterator, std::string()> rWhois;
  qi::rule<Iterator, std::string()> rComments;
  qi::rule<Iterator, std::string()> rIssued;
  qi::rule<Iterator, NetworkCsvFields()> start;
};

// http://boost-spirit.com/home/2010/01/05/stream-based-parsing-made-easy/

template<typename Function>
void ImportNetworkCsvFile( Function& f ) {
    
  try {
    std::ifstream in( "data/networks.csv" );
    in.unsetf( std::ios::skipws ); // turn off whitespace skipping
    
    boost::spirit::istream_iterator begin( in );
    boost::spirit::istream_iterator end;
    
    NetworkCsvLineParser<boost::spirit::istream_iterator> parserNetworkCsvLine;
    
    bool b;
    NetworkCsvFields empty;
    NetworkCsvFields fields;
    while ( b = boost::spirit::qi::parse( begin, end, parserNetworkCsvLine, fields ) ) {
      //std::cout << "INCF: " << fields.sRecordId << ", " << fields.sNetwork << fields.sSubnet << ", " << fields.sCustId << ", '" << fields.sComments << "'" << std::endl;
      (f)( fields.sCustId, fields.sNetwork + fields.sSubnet, "unknown", fields.sComments, "network.csv" );
      fields = empty;
    }
    
  }
  catch (...) {
    std::cout << "errors parsing networks.csv";
  }
  
}


