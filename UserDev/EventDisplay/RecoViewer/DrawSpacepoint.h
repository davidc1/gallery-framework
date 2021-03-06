/**
 * \file DrawSpacepoint.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawSpacepoint
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWSPACEPOINT_H
#define EVD_DRAWSPACEPOINT_H

#include <iostream>
#include "Analysis/anabase.h"
#include "lardataobj/RecoBase/SpacePoint.h"

#include "RecoBase.h"
/**
   \class DrawSpacepoint
   User defined class DrawSpacepoint ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {


  class DrawSpacepoint : public galleryfmwk::anabase, public RecoBase<larutil::Point2D> {

public:

    /// Default constructor
    DrawSpacepoint();

    /// Default destructor
    ~DrawSpacepoint();

    /** IMPLEMENT in DrawCluster.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in DrawCluster.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze(gallery::Event * event);

    /** IMPLEMENT in DrawCluster.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();


private:


};

} // evd

#endif
/** @} */ // end of doxygen group

