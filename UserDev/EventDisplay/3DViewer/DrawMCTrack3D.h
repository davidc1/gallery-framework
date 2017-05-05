/**
 * \file DrawMCTrack3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawMCTrack3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWMCTRACK3D_H
#define LARLITE_DRAWMCTRACK3D_H

#include <iostream>
#include "Analysis/ana_base.h"

#include "lardataobj/MCBase/MCTrack.h"
#include "DrawTrack3D.h"
#include "RecoBase3D.h"
/**
   \class DrawMCTrack3D
   User defined class DrawMCTrack3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

typedef Track3D MCTrack3D;


class DrawMCTrack3D : public galleryfmwk::ana_base, public RecoBase3D<MCTrack3D> {

public:

    /// Default constructor
    DrawMCTrack3D();

    /// Default destructor
    ~DrawMCTrack3D();

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

    void SetViewCosmicOption(int toggleValue) { _draw_cosmic = toggleValue; }


private:

    MCTrack3D getMCTrack3d(const sim::MCTrack & track);

    bool _draw_cosmic;
};

} // evd

#endif
/** @} */ // end of doxygen group

