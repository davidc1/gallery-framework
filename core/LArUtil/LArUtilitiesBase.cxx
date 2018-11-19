#ifndef GALLERY_FMWK_LARUTILBASE_CXX
#define GALLERY_FMWK_LARUTILBASE_CXX

#include "LArUtilitiesBase.h"

namespace larutil {

//-----------------------------------------------------
LArUtilitiesBase::LArUtilitiesBase()
  : _file_name(""),
    _tree_name("")
    //-----------------------------------------------------
{
  _name   = "LArUtilitiesBase";
  _loaded = false;
}

//-----------------------------------------------------
bool LArUtilitiesBase::LoadData(bool force_reload)
//-----------------------------------------------------
{
  if (!force_reload && _loaded) return true;

  galleryfmwk::Message::send( __FUNCTION__,
        Form("Reading-in data for %s", _name.c_str()));

  if (_file_name.empty() || _tree_name.empty()) {

    throw LArUtilException(Form("Either file or tree name not specified to load data for %s", _name.c_str()));

    return false;
  }

  bool status = false;
  try {

    galleryfmwk::Message::send(__FUNCTION__,
          Form("Loading data for %s...\n     file=%s ", _name.c_str(), _file_name.c_str()));
    ClearData();
    status = ReadTree();
    if (!status)
      throw LArUtilException("Failed to load !");
  }
  catch (LArUtilException &e) {

    galleryfmwk::Message::send(__FUNCTION__,
          Form("Failed loading data for %s...", _name.c_str()));

    galleryfmwk::Message::send( __FUNCTION__,
          e.what());

    throw e;
    status = false;
  }

  return status;
}

}
#endif
