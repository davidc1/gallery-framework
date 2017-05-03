/**
 * \file DrawMCTruth3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawMCTruth3D
 *
 * @author Marco Del Tutto
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWMCTRUTH3D_H
#define LARLITE_DRAWMCTRUTH3D_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "RecoBase3D.h"
#include "DrawVertex3D.h"
/**
   \class DrawSeed3D
   User defined class DrawSeed3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

class MCTruth3D{

public:
 double _nux, _nuy, _nuz; ///< True neutrino vertex
 double _nupdg;           ///< Neutrino pdg code
 int    _origin;          ///< Origin

 const double & NuX()    { return _nux;    }
 const double & NuY()    { return _nuy;    }
 const double & NuZ()    { return _nuz;    }
 const double & NuPdg()  { return _nupdg;  }
 const int    & Origin() { return _origin; }

};


class DrawMCTruth3D : public galleryfmwk::ana_base, public RecoBase3D<MCTruth3D> {

public:

    /// Default constructor
    DrawMCTruth3D();

    /// Default destructor
    ~DrawMCTruth3D();

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

