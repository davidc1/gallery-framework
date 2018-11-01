/**
 * \file anabase.h
 *
 * \ingroup Analysis
 * 
 * \brief Base class def for all analysis classes
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    @{*/

#ifndef GALLERY_FMWK_ANABASE_H
#define GALLERY_FMWK_ANABASE_H


#include "gallery/Event.h"
#include "Base/messenger.h"
#include "TFile.h"

  /* DAVID C
namespace message {


/// Defines message level
enum Level {
  kDEBUG = 0,    ///< Message level ... useful to debug a crash
  kINFO,         ///< Debug info but not the lowest level
  kNORMAL,       ///< Normal stdout
  kWARNING,      ///< notify a user in the standard operation mode for an important finding.
  kERROR,        ///< notify a user when something is clearly wrong
  kMSG_TYPE_MAX
};
}
  */

namespace galleryfmwk {
  /**
     \class anabase
     A base class for analysis modules to be operated with event_waveform
     data class instance. 
  */
  class anabase  {
    
  public:
    
    /// Default constructor
    anabase() {_fout = 0;}
    
    /// Default destructor
    virtual ~anabase(){}
    
    /// Initialization method to be called before analyze any data
    virtual bool initialize(){return true;}

    /// Function to be called when new run is found
    virtual bool begin_run(gallery::Event * event){return event;}

    /// Function to be called when new sub-run is found
    virtual bool begin_subrun(gallery::Event * event){return event;}

    /// Analyze a data event-by-event  
    virtual bool analyze(gallery::Event * event){return event;}
    
    /// Finalize method to be called after all events processed.
    virtual bool finalize(){return true;}
    
    /// A setter for analysis output file poitner
    void set_output_file(TFile* fout){_fout=fout;}
    
    /// Setter for the verbosity level 
    virtual void set_verbosity(message::Level level);
    
    inline const std::string name() const {return _name;}

  protected:
    
    TFile* _fout; ///< Analysis output file pointer
    std::string _name;             ///< class name holder
    message::Level _verbosity_level;   ///< holder for specified verbosity level
  };
}
#endif
  
/** @} */ // end of doxygen group 
  
