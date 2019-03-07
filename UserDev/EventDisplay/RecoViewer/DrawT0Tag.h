/**
 * \file DrawT0Tag.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawT0Tag
 *
 * @author elenag
 */

/** \addtogroup RecoViewer

    @{*/

#ifndef EVD_DRAWT0TAG_H
#define EVD_DRAWT0TAG_H

#include "Analysis/anabase.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/AnalysisBase/T0.h" 
#include "canvas/Persistency/Common/FindMany.h"
#include <iostream>

#include "RecoBase.h"
#include "DrawTrack.h"

/**
   \class DrawT0Tag
   User defined class DrawT0Tag ... these comments are used to generate
   doxygen documentation!
 */

namespace evd {

// typedef std::vector<std::pair<float, float> > Track2D;
  class T0Tag : public evd::Track2D {
public:
  friend class DrawT0Tag;

  void setscore(float s) { _score = s; }

  const float &score() { return _score; }

protected:

  float _score;

};


class DrawT0Tag : public galleryfmwk::anabase, public RecoBase<T0Tag> {

public:
  /// Default constructor
  DrawT0Tag();

  /// Default destructor
  ~DrawT0Tag();

  virtual bool initialize();

  virtual bool analyze(gallery::Event *event);

  virtual bool finalize();

private:
  T0Tag getTrack2D(recob::Track track, unsigned int plane);
};

} // evd

#endif
/** @} */ // end of doxygen group
