/**
 * \file LArUtilitiesConfig.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class LArUtilitiesConfig
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef GALLERY_FMWK_LARUTILCONFIG_H
#define GALLERY_FMWK_LARUTILCONFIG_H

#include <iostream>
#include "Base/GeoConstants.h"
#include "Base/messenger.h"
#include "TString.h"

/* DAVIDC
namespace galleryfmwk{
  
  namespace geo {

    enum DetId_t {
      kBo,         ///< Bo id
      kArgoNeuT,   ///< ArgoNeuT id
      kMicroBooNE, ///< MicroBoone id
      kLBNE10kt,   ///< LBNE 10kt id
      kJP250L,     ///< JPARC 250 L id
      kLBNE35t,    ///< 35t prototype id
      kLBNE34kt,   ///< LBNE 34kt id
      kCSU40L,     ///< CSU 40 L id
      kLArIAT,     ///< LArIAT id
      kICARUS,     ///< ICARUS T600 id
      kDetIdMax 
    };
  };
};
*/

namespace larutil {
  /**
     \class LArUtilitiesConfig
     User defined class LArUtilitiesConfig ... these comments are used to generate
     doxygen documentation!
  */
  class LArUtilitiesConfig {
    
  private:
    
    /// Default constructor
    LArUtilitiesConfig(){};
    
    /// Default destructor
    virtual ~LArUtilitiesConfig(){};
    
    static galleryfmwk::geo::DetId_t _detector;
    
  public:
    
    static bool SetDetector(galleryfmwk::geo::DetId_t type);
    
    static galleryfmwk::geo::DetId_t Detector() { return _detector;}
    
  };
}

#endif
/** @} */ // end of doxygen group 

