/* 
 * File:   WCAdmin.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on February 1, 2016, 4:08 PM
 */

#pragma once

#include <Wt/WContainerWidget>

class WCAdmin: public Wt::WContainerWidget {
public:
  WCAdmin( Wt::WContainerWidget *parent=0 );
  virtual ~WCAdmin();
protected:
private:
  void AddLink( const std::string& sClass, const std::string& sPath, const std::string& sAnchor );
};
