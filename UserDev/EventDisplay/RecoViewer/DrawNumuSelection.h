/**
 * \file DrawNumuSelection.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawNumuSelection
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWNUMUSELECTION_H
#define EVD_DRAWNUMUSELECTION_H

#include "Analysis/anabase.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/Vertex.h"
#include <iostream>

#include "DrawShower.h"
#include "RecoBase.h"
#include "DrawTrack.h"
#include "DrawVertex.h"
/**
   \class DrawNumuSelection
   User defined class DrawNumuSelection ... these comments are used to generate
   doxygen documentation!
 */

namespace evd {

class NumuSelection2D {
public:
  friend class DrawNumuSelection;

  const std::vector<Track2D> &tracks() { return _tracks; }
  const std::vector<Shower2D> &showers() { return _showers; }
  const Vertex2D &vertex() { return _vertex; }

  size_t muon_index(){return _muon_index;}
  const Track2D & muon(){return _tracks.at(_muon_index);}


protected:
  std::vector<Track2D> _tracks;
  std::vector<Shower2D> _showers;
  Vertex2D _vertex;
  size_t _muon_index;
  std::vector<Hit2D> _hits;

};

// typedef std::vector<std::pair<float, float> > NumuSelection2D;

class DrawNumuSelection : public galleryfmwk::anabase, public RecoBase<NumuSelection2D> {

public:
  /// Default constructor
  DrawNumuSelection();

  /// Default destructor
  ~DrawNumuSelection();

  virtual bool initialize();

  virtual bool analyze(gallery::Event *event);

  virtual bool finalize();

  evd::Shower2D getShower2d(recob::Shower shower, unsigned int plane);

private:
  NumuSelection2D getNumuSelection2D(const recob::Vertex vtx,const std::vector<recob::Track> tracks,
				     const std::vector<recob::Shower> showers, unsigned int plane);

  // a map linking the PFP Self() attribute used for hierarchy building to the PFP index in the event record
  std::map<unsigned int, unsigned int> _pfpmap;



};

} // evd

#endif
/** @} */ // end of doxygen group
