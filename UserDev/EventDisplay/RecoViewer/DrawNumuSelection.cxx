#ifndef EVD_DRAWNUMUSELECTION_CXX
#define EVD_DRAWNUMUSELECTION_CXX

#include "DrawNumuSelection.h"

namespace evd {

  NumuSelection2D DrawNumuSelection::getNumuSelection2D(const recob::Vertex vtx,const std::vector<recob::Track> tracks, const std::vector<recob::Shower> showers, unsigned int plane) {

  NumuSelection2D result;

  // Vertex
  try {
    double pos[3];
    vtx.XYZ(pos);
    if (plane == 2)
      std::cout << "vtx : [ " << pos[0] << ", " << pos[1] << ", " << pos[2] << "  ]" << std::endl;
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
  // showers
  std::vector<Shower2D> shower_v;
  for (auto const& shr : showers) 
    shower_v.push_back( getShower2d(shr, plane) );
  
  
  if ( (muon_index >= 0) and (muon_index < tracks.size()) ) {
    auto mustart = tracks.at(muon_index).Vertex();
    auto muend   = tracks.at(muon_index).End();
    if (plane == 2) {
      std::cout << "muon start : [ " << mustart.X() << ", " << mustart.Y() << ", " << mustart.Z() << " ]" << std::endl;
      std::cout << "muon end   : [ " << muend.X()   << ", " << muend.Y()   << ", " << muend.Z()   << " ]" << std::endl;
    }
  }

  result._tracks = track_v;
  result._showers = shower_v;
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
  
  // grab clusters associated with PFParticles
  art::InputTag clus_tag(_producer);
  art::FindManyP<recob::Cluster> pfp_clus_assn_v(pfpHandle, *ev, clus_tag);
  // grab clusters themselves
  auto const& clusHandle = ev->getValidHandle<std::vector<recob::Cluster> >(clus_tag);
  // get hits associated to clusters
  art::FindMany<recob::Hit> clus_hit_assn_v(clusHandle, *ev, clus_tag);
  
  // grab showers associated with PFParticles
  art::FindMany<recob::Shower> pfp_shower_assn_v(pfpHandle, *ev, pfp_tag);

  // grab associated metadata
  //art::FindMany< larpandoraobj::PFParticleMetadata > pfPartToMetadataAssoc(pfpHandle, ev, pfp_tag);

  // create list of tracks and showers associated to neutrino candidate
  std::vector<recob::Track  > sliceTracks;
  std::vector<recob::Shower > sliceShowers;
  std::vector<unsigned int> sliceHitIdx;
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
	
	const auto daughter = pfpHandle->at( _pfpmap[daughterid] );

	// find cluster associated to this PFP
	auto const& ass_clus_v = pfp_clus_assn_v.at( _pfpmap[daughterid] );
	// for each cluster, get associated hits
	for (size_t c=0; c < ass_clus_v.size(); c++) {
	  auto const& key = ass_clus_v[c].key();
	  //auto clus_hits = 
	  sliceHitIdx.push_back( key );
	}

	// keep track of index for PFP for hit association
	//sliceHitIdx.push_back( _pfpmap[daughterid] );

	// if there is a track associated to the PFParticle, add it
	auto const& ass_trk_v = pfp_track_assn_v.at( _pfpmap[daughterid] );
	if (ass_trk_v.size() == 1) {
	  sliceTracks.push_back( *(ass_trk_v.at(0)) );
	}
	// if there is a shower associated to the PFParticle, add it
	auto const& ass_shr_v = pfp_shower_assn_v.at( _pfpmap[daughterid] );
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
    _dataByPlane.at(view).push_back(this->getNumuSelection2D(nuvtx, sliceTracks, sliceShowers, view));
  }

  // loop through slice hits
  for (auto const& assidx : sliceHitIdx) {
    // get associated hits
    auto const& ass_hits = clus_hit_assn_v.at(assidx);
    //auto const& ass_hits = pfp_hit_assn_v.at(assidx);
    for (size_t hitidx=0; hitidx < ass_hits.size(); hitidx++) {
      auto hit = *(ass_hits.at(hitidx));
      unsigned int view = hit.View();
      _dataByPlane.at(view).back()._hits.emplace_back(
						      Hit2D(hit.WireID().Wire,
							    hit.PeakTime(),
							    hit.Integral(),
							    hit.RMS(),
							    hit.StartTick(),
							    hit.PeakTime(),
							    hit.EndTick(),
							    hit.PeakAmplitude(),
							    view
							    ));
    }// for all hits associated to this PFP
  }// for all pfp -> hit association indices
  
  return true;
}

  evd::Shower2D DrawNumuSelection::getShower2d(recob::Shower shower, unsigned int plane) {
    
    Shower2D result;
    result._is_good = false;
    result._plane = plane;
    // Fill out the parameters of the 2d shower
    result._startPoint
      = geoHelper -> Point_3Dto2D(shower.ShowerStart(), plane);
    
    // Next get the direction:
    result._angleInPlane = geoHelper->Slope_3Dto2D(shower.Direction(), plane);
    
    // Get the opening Angle:
    // result._openingAngle = shower.OpeningAngle();
    result._openingAngle = 0.2;
    
    
    auto secondPoint = shower.ShowerStart() + shower.Length() * shower.Direction();
    
    
    result._endPoint
      = geoHelper -> Point_3Dto2D(secondPoint, plane);
    
    result._length = sqrt(pow(result.startPoint().w - result.endPoint().w, 2) +
			  pow(result.startPoint().t - result.endPoint().t, 2));
    
    if (shower.dEdx().size() > plane) {
      result._dedx = shower.dEdx()[plane];
    }
    else {
      result._dedx = 0.0;
    }
    
    if (shower.Energy().size() > plane) {
      result._energy = shower.Energy()[plane];
    }
    else {
      result._energy = 0.0;
    }
    
    result._is_good = true;
    
    return result;
  }
  

bool DrawNumuSelection::finalize() {

  return true;
}

DrawNumuSelection::~DrawNumuSelection() {}

} // larlite




#endif
