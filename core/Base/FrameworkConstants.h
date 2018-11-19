/**
 * \file FrameworkConstants.h
 *
 * \ingroup Base
 *
 * \brief defines basic constants used in this framework
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef GALLERY_FMWK_FRAMEWORKCONSTANTS_H
#define GALLERY_FMWK_FRAMEWORKCONSTANTS_H
#include <string>
#include <limits>

/// Namespace of everything in this framework
namespace galleryfmwk {

namespace data {
const unsigned char  kINVALID_UCHAR  = std::numeric_limits<unsigned char>::max();
const char           kINVALID_CHAR   = std::numeric_limits<char>::max();
const unsigned short kINVALID_USHORT = std::numeric_limits<unsigned short>::max();
const short          kINVALID_SHORT  = std::numeric_limits<short>::max();
const unsigned int   kINVALID_UINT   = std::numeric_limits<unsigned int>::max();
const int            kINVALID_INT    = std::numeric_limits<int>::max();
const size_t         kINVALID_SIZE   = std::numeric_limits<size_t>::max();

const double kINVALID_DOUBLE = std::numeric_limits<double>::max();
const float  kINVALID_FLOAT  = std::numeric_limits<float>::max();
}

/// Defines constants for Message utility
namespace message {

  const std::string ColorPrefix[5] =
    {
      "\033[94m", ///< blue ... DEBUG
      "\033[92m", ///< green ... INFO
      "\033[95m", ///< magenta ... NORMAL
      "\033[93m", ///< yellow ... WARNING
      "\033[91m"  ///< red ... ERROR
    };
  ///< Color coding of message
  
  const std::string StringPrefix[5] =
    {
      "     [DEBUG]  ", ///< DEBUG message prefix
      "      [INFO]  ", ///< INFO message prefix
      "    [NORMAL]  ", ///< NORMAL message prefix
      "   [WARNING]  ", ///< WARNING message prefix
      "     [ERROR]  "  ///< ERROR message prefix
    };
  ///< Prefix of message
}

}
#endif
/** @} */ // end of doxygen group
