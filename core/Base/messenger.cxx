#ifndef GALLERY_FMWK_MESSENGER_CXX
#define GALLERY_FMWK_MESSENGER_CXX

#include "messenger.h"

namespace galleryfmwk {
  Message* Message::me = 0;
  
  void Message::send(std::string msg)
  {
    std::cout 
      << message::ColorPrefix[3].c_str()
      << message::StringPrefix[3].c_str()
      << "\033[0m"
      << msg.c_str()
      << std::endl;
  }
  
  void Message::send(std::string where, std::string msg)
  {
    std::cout 
      << message::ColorPrefix[3].c_str()
      << message::StringPrefix[3].c_str()
      << "\033[0m"
      << "\033[95m"
      << "<"
      << where.c_str()
      << "> "
      << "\033[0m"
      << msg.c_str()
      << std::endl;
  }

}
#endif
