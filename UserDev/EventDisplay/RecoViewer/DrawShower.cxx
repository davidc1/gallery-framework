#ifndef EVD_DRAWSHOWER_CXX
#define EVD_DRAWSHOWER_CXX

#include "DrawShower.h"

namespace evd {


DrawShower::DrawShower()
  : RecoBase<Shower2D>()
{
  _name = "DrawShower";
  _fout = 0;
  // showerVectorByPlane = new std::vector<std::vector<Shower2D> >;
}

bool DrawShower::initialize() {

  // // Resize data holder to accommodate planes and wires:
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;

}

bool DrawShower::analyze(gallery::Event * ev) {

  // get a handle to the showers
  art::InputTag shower_tag(_producer);
  auto const & showerHandle
    = ev -> getValidHandle<std::vector <recob::Shower> >(shower_tag);

  // draw associated hits too
  art::InputTag assn_tag(_producer);
  art::FindMany<recob::Hit> hits_for_shower(showerHandle, *ev, assn_tag);


  if (showerHandle -> size() == 0) {
    std::cout << "No showers available to draw by producer "
              << _producer
              << std::endl;
    return true;
  }



  // Clear out the hit data but reserve some space for the showers
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(showerHandle -> size());
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }


  // Populate the shower vector:
  for (size_t s = 0; s < showerHandle->size(); s++) {

    auto const& shower = showerHandle->at(s);

    std::vector<recob::Hit const*> hits;
    hits_for_shower.get(s, hits);
    std::cout << "There are " << hits.size() << " associated with this shower" << std::endl;

    for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
      // get the reconstructed shower for this plane
      auto shr2D = getShower2d(shower, view);

      shr2D._showerCluster_v.resize(3);

      // create empty cluster
      evd::Cluster2D clus;
      clus._is_good = true;

      // loop through hits and figure out if any are from this plane
      for (auto const& hit : hits) {
	
	if (hit->WireID().Plane != view) continue;
	
	Hit2D hit2d(hit->WireID().Wire,
		  hit->PeakTime(),
		  hit->Integral(),
		  hit->RMS(),
		  hit->StartTick(),
		  hit->PeakTime(),
		  hit->EndTick(),
		  hit->PeakAmplitude(),
		  view);

	clus.push_back(hit2d);
	
      }// for all hits

      if (clus.size())
	shr2D._showerCluster_v[view] = clus;


      _dataByPlane.at(view).push_back( shr2D );

    }// for all planes
  }// for all showers


  return true;
}

bool DrawShower::finalize() {

  return true;
}



Shower2D DrawShower::getShower2d(recob::Shower shower, unsigned int plane) {



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



} // larlite

#endif
