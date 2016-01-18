#ifndef TEST_H
#define TEST_H

#include <Magick++.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <thread>
#include <wx/wx.h>
#include "include/SteganoHide.h"
#include "include/SteganoUnhide.h"
#include "include/SteganoExceptions.h"
#include "GUI.h"

#define HIDE_FILE "test.jpg"
#define STEGANO_FILE "stegano.png"

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};

#endif // TEST_H
