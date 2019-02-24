#ifndef EVD_DRAWNUMUSELECTION_CXX
#define EVD_DRAWNUMUSELECTION_CXX

#include "DrawNumuSelection.h"

namespace evd {

  NumuSelection2D DrawNumuSelection::getNumuSelection2D(const recob::Vertex vtx,const std::vector<recob::Track> tracks, unsigned int plane) {

  NumuSelection2D result;

  // Vertex
  try {
    double pos[3];
    vtx.XYZ(pos);
    auto point = geoHelper->Point_3Dto2D(pos, plane);
    result._vertex = point;
  } catch (...) {
  }
  // Tracks
  double max_length = -1;
  size_t muon_index = 0;
  std::vector<Track2D> track_v;
  track_v.reserve(tracks.size());
  for (size_t t = 0; t < tracks.size(); t++) {
    auto trk = tracks.at(t);
    Track2D trk_out;
    trk_out._track.reserve(trk.NumberTrajectoryPoints());
    for (unsigned int i = 0; i < trk.NumberTrajectoryPoints(); i++) {
      try {
        if (trk.HasValidPoint(i)) {
	  auto loc = trk.LocationAtPoint(i);
	  TVector3 xyz(loc.X(),loc.Y(),loc.Z());
          auto point = geoHelper->Point_3Dto2D(xyz, plane);
          trk_out._track.push_back(std::make_pair(point.w, point.t));
        }
      } catch (...) {
        continue;
      }
    }
    track_v.emplace_back(trk_out);

    // the longest track is the muon
    double length = trk.Length();
    if (length > max_length) {
      muon_index = t;
      max_length = length;
    }
  }

  result._tracks = track_v;
  result._muon_index = muon_index;

  return result;
}

DrawNumuSelection::DrawNumuSelection() {
  _name = "DrawNumuSelection";
  _fout = 0;
}

bool DrawNumuSelection::initialize() {

  // Resize data holder
  if (_dataByPlane.size() != geoService->Nviews()) {
    _dataByPlane.resize(geoService->Nviews());
  }
  return true;
}

bool DrawNumuSelection::analyze(gallery::Event *ev) {

  // get a handle to the showers
  art::InputTag pfp_tag(_producer);
  auto const & pfpHandle = 
    ev -> getValidHandle<std::vector <recob::PFParticle> >(pfp_tag);

  // build PFParticle map  for this event
  _pfpmap.clear();
  for (unsigned int p=0; p < pfpHandle->size(); p++)
    _pfpmap[pfpHandle->at(p).Self()] = p;
  
  // grab tracks associated with PFParticles
  art::InputTag trk_tag(_producer);
  art::FindMany<recob::Track>  pfp_track_assn_v (pfpHandle, *ev, trk_tag);

  // grab vertices associated with PFParticles
  art::InputTag vtx_tag(_producer);
  art::FindMany<recob::Vertex> pfp_vertex_assn_v(pfpHandle, *ev, vtx_tag);
  
  // grab showers associated with PFParticles
  art::FindMany<recob::Shower> pfp_shower_assn_v(pfpHandle, *ev, pfp_tag);

  // grab associated metadata
  //art::FindMany< larpandoraobj::PFParticleMetadata > pfPartToMetadataAssoc(pfpHandle, ev, pfp_tag);

  // create list of tracks and showers associated to neutrino candidate
  std::vector<recob::Track  > sliceTracks;
  std::vector<recob::Shower > sliceShowers;
  // save  vertex  assocaited to neutrino candidate
  recob::Vertex nuvtx;

  // count neutrino candidates
  size_t neutrinos = 0;

  // loop through PFParticles
  for (size_t p=0; p < pfpHandle->size(); p++) {
    
    auto pfp = pfpHandle->at(p);
    //const art::Ptr<recob::PFParticle> pfp_ptr(pfpHandle, p );
    
    // get metadata for this PFP
    //auto &pfParticleMetadataList(pfPartToMetadataAssoc.at(p));
    //const std::vector< art::Ptr<larpandoraobj::PFParticleMetadata> > &pfParticleMetadataList(pfPartToMetadataAssoc.at(p));
    
    //  find neutrino candidate
    if (pfp.IsPrimary() == false) continue;
    
    auto PDG = fabs(pfp.PdgCode());
    if ( (PDG == 12) || (PDG == 14) ) {

      neutrinos += 1;

      auto pfpkey = p;
      auto ass_vtx_v  =pfp_vertex_assn_v.at( p );
      if (ass_vtx_v.size() != 1) 
	std::cout << "ERROR. Neutrino not associated with a single vertex..." << std::endl;
      nuvtx = *(ass_vtx_v.at(0));
      
      auto daughters = pfp.Daughters();

      for(auto const& daughterid : daughters) {
	
	if (_pfpmap.find(daughterid) == _pfpmap.end()) {
	  std::cout << "Did not find DAUGHTERID in map! error"<< std::endl;
	  continue;
	}
	
	const auto daughter = pfpHandle->at( _pfpmap.at(daughterid) );
	//const art::Ptr<recob::PFParticle> daughter_ptr(pfpHandle, _pfpmap.at(daughterid) );

	auto daughterkey = _pfpmap.at(daughterid);
	// if there is a track associated to the PFParticle, add it
	auto const& ass_trk_v = pfp_track_assn_v.at( daughterkey );
	if (ass_trk_v.size() == 1) sliceTracks.push_back( *(ass_trk_v.at(0)) );
	// if there is a shower associated to the PFParticle, add it
	auto const& ass_shr_v = pfp_shower_assn_v.at( daughterkey );
	if (ass_shr_v.size() == 1) sliceShowers.push_back( *(ass_shr_v.at(0)) );
      }// for all PFParticles in the slice
      
    }// if a neutrino PFParticle
  }// for all PFParticles
  
  if (neutrinos != 1) return true;

  // Clear out the data but reserve some space for the tracks
  for (unsigned int p = 0; p < geoService->Nviews(); p++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(1.);
    _wireRange.at(p).first = 99999;
    _timeRange.at(p).first = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }

  for (unsigned int view = 0; view < geoService->Nviews(); view++) {
    _dataByPlane.at(view).push_back(this->getNumuSelection2D(nuvtx, sliceTracks, view));
  }
  
  return true;
}

bool DrawNumuSelection::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.
  //
  // Say you made a histogram pointer h1 to store. You can do this:
  //
  // if(_fout) { _fout->cd(); h1->Write(); }
  //
  // else
  //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!!
  //   File not opened?");
  //
  return true;
}

DrawNumuSelection::~DrawNumuSelection() {}

} // larlite

#endif
