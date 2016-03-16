/* 
 * File:   Upload.cpp
 * Author: Raymond Burkholder
 *         raymond@burkholder.net
 * 
 * Created on March 15, 2016, 9:36 PM
 */

#include <Wt/WBreak>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
#include <Wt/WText>

#include "Upload.h"

Upload::Upload( Wt::WContainerWidget* pcw ) {
  
  //Wt::WContainerWidget *container = new Wt::WContainerWidget();

  Wt::WFileUpload *fu = new Wt::WFileUpload( pcw );
  fu->setFileTextSize( 10000 ); // Set the maximum file size (in KB )
  fu->setProgressBar(new Wt::WProgressBar());
  fu->setMargin(10, Wt::Right);

  // Provide a button to start uploading.
  Wt::WPushButton *uploadButton = new Wt::WPushButton("Send", pcw );
  uploadButton->setMargin(10, Wt::Left | Wt::Right);

  Wt::WText *out = new Wt::WText( pcw );

  // Upload when the button is clicked.
  uploadButton->clicked().connect(std::bind([=] () {
      fu->upload();
      uploadButton->disable();
  }));

  // Upload automatically when the user entered a file.
  fu->changed().connect(std::bind([=] () {
      fu->upload();
      uploadButton->disable();
      std::string s( "File upload is changed." );
      out->setText( s );
  }));

  // React to a succesfull upload.
  fu->uploaded().connect(std::bind([=] () {
    std::string s( "File upload is finished: " );
    s += fu->clientFileName().toUTF8();
    s += ",";
    //s += fu->fileTextSize()
    s += fu->spoolFileName();
    //fu->stealSpooledFile()
    out->setText( s );
  }));

  // React to a file upload problem.
  fu->fileTooLarge().connect(std::bind([=] () {
      out->setText("File is too large.");
  }));
}

Upload::~Upload() {
}
