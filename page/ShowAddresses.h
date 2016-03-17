/* 
 * File:   ShowAddresses.h
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 *
 * Created on March 16, 2016, 1:00 PM
 */

#pragma once

#include <Wt/Dbo/Dbo>
//#include <Wt/Dbo/ptr>

#include <Wt/WContainerWidget>

class ShowAddresses {
public:
  ShowAddresses( Wt::WContainerWidget* pcw, dbo::Session& session );
  virtual ~ShowAddresses();
protected:
private:

};
