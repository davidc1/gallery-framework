/**
 * \file DrawCosmicTag.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawCosmicTag
 *
 * @author davidc
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWCOSMICTAG_H
#define EVD_DRAWCOSMICTAG_H

#include "Analysis/anabase.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/AnalysisBase/CosmicTag.h"
#include "canvas/Persistency/Common/FindMany.h"
#include <iostream>

#include "RecoBase.h"
#include "DrawTrack.h"

/**
   \class DrawCosmicTag
   User defined class DrawCosmicTag ... these comments are used to generate
   doxygen documentation!
 */

namespace evd {

// typedef std::vector<std::pair<float, float> > Track2D;
  class CosmicTag : public evd::Track2D {
public:
  friend class DrawCosmicTag;

  void setscore(float s) { _score = s; }

  const float &score() { return _score; }

protected:

  float _score;

};


class DrawCosmicTag : public galleryfmwk::anabase, public RecoBase<CosmicTag> {

public:
  /// Default constructor
  DrawCosmicTag();

  /// Default destructor
  ~DrawCosmicTag();

  virtual bool initialize();

  virtual bool analyze(gallery::Event *event);

  virtual bool finalize();

private:
  CosmicTag getTrack2D(recob::Track track, unsigned int plane);
};

} // evd

#endif
/** @} */ // end of doxygen group
