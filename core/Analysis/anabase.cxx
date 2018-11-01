#ifndef GALLERY_FMWK_ANABASE_CXX
#define GALLERY_FMWK_ANABASE_CXX

#include "anabase.h"

namespace galleryfmwk {
//#####################################################
void anabase::set_verbosity(message::Level level)
//#####################################################
{

  _verbosity_level = level;

  // for (size_t i = (size_t)(message::kDEBUG); i < (size_t)(msg::kMSG_TYPE_MAX); ++i)
  //   _verbosity[i] = false;

  // switch (level) {
  // case message::kDEBUG:
  //   _verbosity[message::kDEBUG] = true;
  // case message::kINFO:
  //   _verbosity[message::kINFO] = true;
  // case message::kNORMAL:
  //   _verbosity[message::kNORMAL] = true;
  // case message::kWARNING:
  //   _verbosity[message::kWARNING] = true;
  // case message::kERROR:
  //   _verbosity[message::kERROR] = true;
  // case message::kMSG_TYPE_MAX:
  //   break;
  // }

}
}

#endif
