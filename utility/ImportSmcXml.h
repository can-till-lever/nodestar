/* 
 * File:   ImportSmcXml.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on September 30, 2014, 10:44
 */

#ifndef IMPORTSMCXML_H
#define	IMPORTSMCXML_H

#include <fstream>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/foreach.hpp>

#include <Wt/Dbo/Dbo>
namespace dbo = Wt::Dbo;

using boost::property_tree::ptree;


template<typename Function>
//void ProcessElement( const ptree& tree, vNetwork_t& v ) {  // todo:  pass in a function for processing
void ProcessElement( const ptree& tree, Function& f ) {  
  struct Network {
    bool bBroadcast;
    std::string sNetwork;
    std::string sName;
    std::string sComment;
    Network( void ): bBroadcast( true ) {};
  };
  BOOST_FOREACH( ptree::value_type const& element, tree ) {
    //std::cout << "[" << element.first << "]";
    if ( "network" == element.first ) {
      Network n;
      n.sNetwork = element.second.get<std::string>( "<xmlattr>.ipv4_network" );
      n.sName = element.second.get<std::string>( "<xmlattr>.name" );
      n.sComment = element.second.get<std::string>( "<xmlattr>.comment", "" );
      std::cout << "network: " << n.sNetwork << ", " << n.sName << std::endl;
      std::string sBroadcast = element.second.get<std::string>( "<xmlattr>.broadcast" );
      if ( "true" != sBroadcast ) throw std::runtime_error( "bad broadcast" );
      (f)( n.sNetwork, n.sName, n.sComment, "smc xml network" );
    }
    if ( "host" == element.first ) {
      Network n;
      n.sName = element.second.get<std::string>( "<xmlattr>.name" );
      n.sNetwork = element.second.get<std::string>( "mvia_address.<xmlattr>.address" );
      n.sComment = element.second.get<std::string>( "<xmlattr>.comment", "" );
      std::cout << "host: " << n.sNetwork << ", " << n.sName << std::endl;
      (f)( n.sNetwork, n.sName, n.sComment, "smc xml host" );
    }
    if ( "router" == element.first ) {
      Network n;
      n.sName = element.second.get<std::string>( "<xmlattr>.name" );
      n.sNetwork = element.second.get<std::string>( "mvia_address.<xmlattr>.address" );
      n.sComment = element.second.get<std::string>( "<xmlattr>.comment", "" );
      std::cout << "host: " << n.sNetwork << ", " << n.sName << std::endl;
      (f)( n.sNetwork, n.sName, n.sComment, "smc xml router" );
    }
    
    if ( "<xmlattr>" != element.first ) {
      if ( !element.second.empty() ) {
        ProcessElement( element.second, f );
      }
    }
  }
}

template<typename Function>
void ImportSmcXml( Function& f ) {
//void ImportSmcXml( dbo::Session&, Function f ) {
  
  std::string sName( "data/exported_data.xml" );
  
  std::ifstream file;
  
  std::cout << "Opening Input Symbol File ";
  std::cout << sName;
  std::cout << " ... ";
  
  file.open( sName.c_str() );
  if ( file.bad() ) {
    throw  std::runtime_error( "Can't open input file" );
  }
  
  std::cout << std::endl;
  
  std::cout << "Loading SMC XML ..." << std::endl;

  boost::property_tree::ptree tree;
  boost::property_tree::read_xml( file, tree );
  
  file.close();
  
  std::cout << " ... complete. " << std::endl;
  
  std::cout << "Processing ..." << std::endl;
  
  //vNetwork_t vNetwork;
  ProcessElement( tree, f );
  
  std::cout << " ... complete. " << std::endl;
  
}


#endif	/* IMPORTSMCXML_H */

