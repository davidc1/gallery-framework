#ifndef DRAWMCTRUTH3D_CXX
#define DRAWMCTRUTH3D_CXX

#include "DrawMCTruth3D.h"

namespace evd {


DrawMCTruth3D::DrawMCTruth3D() {
  _name = "DrawMCTruth3D";
  _fout = 0;
}

bool DrawMCTruth3D::initialize() {

  return true;
}

bool DrawMCTruth3D::analyze(gallery::Event * ev) {

  //
  // Do your event-by-event analysis here. This function is called for
  // each event in the loop. You have "storage" pointer which contains
  // event-wise data. To see what is available, check the "Manual.pdf":
  //
  // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
  //
  // Or you can refer to Base/DataFormatConstants.hh for available data type
  // enum values. Here is one example of getting PMT waveform collection.
  //
  // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
  //
  // if( event_fifo )
  //
  //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
  //





  // get a handle to the vertexes
  art::InputTag mctruth_tag(_producer);
  auto const & mctruthHandle
        = ev -> getValidHandle<std::vector <simb::MCTruth> >(mctruth_tag);


  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(mctruthHandle -> size());


  // Populate the mctruth vector:
  for (auto & mct : *mctruthHandle) {

    MCTruth3D temp;

    temp._origin = mct.Origin();

    if (temp._origin == simb::kBeamNeutrino) {
      auto const & mc_nu = mct.GetNeutrino().Nu();
      temp._nux   = mc_nu.Vx();
      temp._nuy   = mc_nu.Vy();
      temp._nuz   = mc_nu.Vz();
      temp._nupdg = mc_nu.PdgCode();
    } else {
      temp._nux   = -9999.;
      temp._nuy   = -9999.;
      temp._nuz   = -9999.;
      temp._nupdg = -9999;
    }

    _data.push_back(temp);
  }


  return true;
}

bool DrawMCTruth3D::finalize() {

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
  //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
  //
  return true;
}

DrawMCTruth3D::~DrawMCTruth3D() {}


} // evd

#endif
