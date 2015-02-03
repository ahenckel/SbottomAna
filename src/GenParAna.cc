// ===========================================================================
// 
//       Filename:  GenParAna.cc
// 
//    Description:  Analyzer for Gen Particles
// 
//        Version:  1.0
//        Created:  01/29/2015 12:10:16 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#include "UserCode/SuperTopTagger/interface/GenParAna.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/Math/interface/deltaR.h"

//----------------------------------------------------------------------------
//       Class:  GenParAna
//      Method:  GenParAna
// Description:  constructor
//----------------------------------------------------------------------------
GenParAna::GenParAna (HistTool* hist_)
{
  hist = hist_;
}  // -----  end of method GenParAna::GenParAna  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  GenParAna
//      Method:  GenParAna
// Description:  copy constructor
//----------------------------------------------------------------------------
GenParAna::GenParAna ( const GenParAna &other )
{
}  // -----  end of method GenParAna::GenParAna  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  GenParAna
//      Method:  ~GenParAna
// Description:  destructor
//----------------------------------------------------------------------------
GenParAna::~GenParAna ()
{
}  // -----  end of method GenParAna::-GenParAna  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  GenParAna
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  GenParAna&
GenParAna::operator = ( const GenParAna &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method GenParAna::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetGenParticles
//  Description:  /* cursor */
// ===========================================================================
bool GenParAna::GetGenParticles(edm::Handle<reco::GenParticleCollection> PrunedGenHdl, 
    edm::Handle<std::vector<pat::PackedGenParticle> >  PackedGenHdl) 
{
  CleanPerEvent();
  LocalPackedGenHdl = PackedGenHdl;
  //PrintGenPars(PrunedGenHdl);

  return true;
}       // -----  end of function GenParAna::GetGenParticles  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::PrintGenPars
//  Description:  
// ===========================================================================
bool GenParAna::PrintGenPars(edm::Handle<reco::GenParticleCollection> PrunedGenHdl) const
{
  for (size_t i = 0; i < PrunedGenHdl->size(); ++i)
  {
    reco::GenParticle par = PrunedGenHdl->at(i);
    std::cout <<  i <<" Status " << par.status() <<" ID " << par.pdgId() <<" dauter " 
      << par.numberOfDaughters() << " mass " << par.mass() <<" pt " << par.pt()<< std::endl;
  }
  return true;
}       // -----  end of function GenParAna::PrintGenPars  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetWDecay
//  Description:  Once we get W decay product, fill in the decay inforamtion
// ===========================================================================
WDecay GenParAna::GetWDecay(const reco::Candidate* WPar) 
{
//----------------------------------------------------------------------------
//  If it is an intermedia W , nest the function
//----------------------------------------------------------------------------
  if (WPar->numberOfMothers() == 1 && fabs( WPar->daughter(0)->pdgId()) == 24)
  {
    return GetWDecay(WPar->daughter(0));
  }

//----------------------------------------------------------------------------
//  If this W decays
//----------------------------------------------------------------------------
  WDecay temp;
  temp.Widx_ = GetPackedPars(WPar);

  int hadcount = 0;
  assert( WPar->numberOfDaughters()  == 2);
  for (unsigned int i = 0; i < WPar->numberOfDaughters(); ++i)
  {
    const reco::Candidate* dau = WPar->daughter(i);
    if ( fabs(dau->pdgId()) == 11 || fabs(dau->pdgId()) == 13 ||fabs(dau->pdgId()) == 15 )
    {
      temp.Lepidx_ = GetPackedPars(dau);
      temp.isLeptonic_ = true;
    }
    else if ( fabs(dau->pdgId()) == 12 || fabs(dau->pdgId()) == 14 ||fabs(dau->pdgId()) == 16 )
      temp.neuidx_ = GetPackedPars(dau);
    else if ( ( fabs(dau->pdgId()) > 0 && fabs(dau->pdgId())  <= 5 ) || fabs(dau->pdgId()) == 22)
    {
      if (hadcount == 0)
      {
        temp.had1idx_ = GetPackedPars(dau);
        hadcount++;
      } else {
        temp.had2idx_ = GetPackedPars(dau);
        hadcount++;
      }
    }
  }
  assert(hadcount == 0 || hadcount ==2);

  Widx.push_back(temp);
  return temp;
}       // -----  end of function GenParAna::GetWDecay  -----


// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetTopDecay
//  Description:  
// ===========================================================================
TopDecay GenParAna::GetTopDecay(const reco::Candidate* TopPar)
{
  TopDecay temp;
  temp.topidx_ = GetPackedPars(TopPar);
  bool hasProducts = false; //only store the top quarks has daughters

  for(unsigned int i=0; i < TopPar->numberOfDaughters(); ++i)
  {
    const reco::Candidate *dau = TopPar->daughter(i);
    if ( fabs(dau->pdgId()) == 5 ) 
    {
      hasProducts = true;
      temp.bidx_ = GetPackedPars(dau);
    }
    if ( fabs(dau->pdgId()) == 24 ) temp.widx_ = GetWDecay(dau);
  }

  if (hasProducts)
    Topidx.push_back(temp);
  return temp;
}       // -----  end of function GenParAna::GetTopDecay  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetTopandW
//  Description:  
// ===========================================================================
bool GenParAna::GetTopandW(edm::Handle<reco::GenParticleCollection> PrunedGenHdl)
{
  bool hasTop = false;
  for (size_t i = 0; i < PrunedGenHdl->size(); ++i)
  {
    if (fabs(PrunedGenHdl->at(i).pdgId()) == 6)
    {
      GetTopDecay(&PrunedGenHdl->at(i));
      hasTop = true;
    }
  }


//----------------------------------------------------------------------------
//  For W + jets event
//----------------------------------------------------------------------------
  if (! hasTop)
  {
    for (size_t i = 0; i < PrunedGenHdl->size(); ++i)
    {
      if (fabs(PrunedGenHdl->at(i).pdgId()) == 24)
      {
        GetWDecay(&PrunedGenHdl->at(i));
      }
    }
  }

  return hasTop;
}       // -----  end of function GenParAna::GetTopandW  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::Test
//  Description:  
// ===========================================================================
bool GenParAna::Test() const
{

  std::cout << "Test whether this is OK" << std::endl;
  return true;
}       // -----  end of function GenParAna::Test  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::isAncestor
//  Description:  Check recursively if any ancestor of particle is the given
//  one, taken from the MINIAOD twiki page
//  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#MC_Truth
// ===========================================================================
bool GenParAna::isAncestor(const reco::Candidate* ancestor, const reco::Candidate * particle) const
{
  //particle is already the ancestor
  if(ancestor == particle ) return true;

  //otherwise loop on mothers, if any and return true if the ancestor is found
  for(size_t i=0;i< particle->numberOfMothers();i++)
  {
    if(isAncestor(ancestor,particle->mother(i))) return true;
  }
  //if we did not return yet, then particle and ancestor are not relatives
  return false;

}       // -----  end of function GenParAna::isAncestor  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetPackedPars
//  Description:  /* cursor */
// ===========================================================================
GenPrunePacked GenParAna::GetPackedPars(const reco::Candidate* GenPar) 
{
  GenPrunePacked temp;
  temp.Pruned = GenPar;
  for(size_t j=0; j<LocalPackedGenHdl->size();j++){
    //get the pointer to the first survied ancestor of a given packed GenParticle in the prunedCollection 
    const reco::Candidate * motherInPrunedCollection = (*LocalPackedGenHdl)[j].mother(0) ;
    if(motherInPrunedCollection != nullptr && isAncestor( temp.Pruned , motherInPrunedCollection)){
      temp.Packed.push_back(&LocalPackedGenHdl->at(j));
    }
  }
  return temp;
}       // -----  end of function GenParAna::GetPackedPars  -----


// ===  FUNCTION  ============================================================
//         Name:  GenParAna::CleanPerEvent
//  Description:  
// ===========================================================================
bool GenParAna::CleanPerEvent()
{
  Topidx.clear();
  Widx.clear();
  return true;
}       // -----  end of function GenParAna::CleanPerEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::PrintTopDecay
//  Description:  
// ===========================================================================
bool GenParAna::PrintTopDecay() const
{
  std::cout << "size " << Topidx.size() << std::endl;
  for(unsigned int i=0; i < Topidx.size(); ++i)
  {
    std::cout << " Top " << Topidx.at(i).topidx_.Pruned->pdgId() 
    << " dau: " ;
    BOOST_FOREACH(const reco::Candidate* dau, Topidx.at(i).topidx_.Packed)
    {

      std::cout << dau->pdgId() << " " ;
    }
    std::cout << "" << std::endl;

    std::cout << " b " << Topidx.at(i).bidx_.Pruned->pdgId() 
    << " dau: " ;
    BOOST_FOREACH(const reco::Candidate* dau, Topidx.at(i).bidx_.Packed)
    {
      std::cout << dau->pdgId() << " " ;
    }
    std::cout << "" << std::endl;
  }
  
  return true;
}       // -----  end of function GenParAna::PrintTopDecay  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::PrintWDecay
//  Description:  
// ===========================================================================
bool GenParAna::PrintWDecay(WDecay wboson) const
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ W ~~~~~
  
    std::cout << " W " << wboson.Widx_.Pruned->pdgId() 
    << " dau: " ;
    BOOST_FOREACH(const reco::Candidate* dau, wboson.Widx_.Packed)
    {

      std::cout << dau->pdgId() << " " ;
    }
    std::cout << "" << std::endl;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Lepton ~~~~~
    if (wboson.Lepidx_.Pruned != NULL)
    {
      std::cout << " lepton " << wboson.Lepidx_.Pruned->pdgId() 
        << " dau: " ;
      BOOST_FOREACH(const reco::Candidate* dau, wboson.Lepidx_.Packed)
      {
        std::cout << dau->pdgId() << " " ;
      }
      std::cout << "" << std::endl;
    }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Had 1 ~~~~~
  if (!wboson.isLeptonic_ && wboson.had1idx_.Pruned != NULL)
  {
      std::cout << " Had1 " << wboson.had1idx_.Pruned->pdgId() 
        << " dau: " ;
      BOOST_FOREACH(const reco::Candidate* dau, wboson.had1idx_.Packed)
      {
        std::cout << dau->pdgId() << " " ;
      }
      std::cout << "" << std::endl;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Had 2 ~~~~~
  if (!wboson.isLeptonic_ && wboson.had2idx_.Pruned != NULL)
  {
      std::cout << " Had2 " << wboson.had2idx_.Pruned->pdgId() 
        << " dau: " ;
      BOOST_FOREACH(const reco::Candidate* dau, wboson.had2idx_.Packed)
      {
        std::cout << dau->pdgId() << " " ;
      }
      std::cout << "" << std::endl;
  }

  return true;
}       // -----  end of function GenParAna::PrintWDecay  -----


// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetGenCount
//  Description:  
// ===========================================================================
bool GenParAna::GetGenCount() 
{
  GenCounts.clear();
  GenCounts["top"] = 0;
  GenCounts["b"] = 0;
  GenCounts["W"] = 0;
  GenCounts["lep"] = 0;
  GenCounts["ele"] = 0;
  GenCounts["muon"] = 0;
  GenCounts["tau"] = 0;

  
  return true;
}       // -----  end of function GenParAna::GetGenCount  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetLepCount
//  Description:  
// ===========================================================================
int GenParAna::GetLepCount() const
{
  
  int lepcount = 0;
  for (size_t i = 0; i < Widx.size(); ++i)
  {
    if (Widx.at(i).isLeptonic_)
    {
      lepcount++;
    }
    
  }
  return lepcount;

  return true;
}       // -----  end of function GenParAna::GetLepCount  -----


// ===  FUNCTION  ============================================================
//         Name:  GenParAna::BookHistogram
//  Description:  
// ===========================================================================
bool GenParAna::BookHistogram()
{
  hist->AddTH1( "WPT", " WPT", 400, 0, 400);
  hist->AddTH2( "WDR", " Decay dR vs expected dR ", 70, 0, 7, 70, 0, 7);
  hist->AddTH2( "WDR2", " Decay dR vs expected dR with quark pt cut ", 70, 0, 7, 70, 0, 7);
  hist->AddTH2( "WDRPT", " Decay dR vs expected dR Vs WPT", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "WDRPT1", " Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "WDRPT2", " Expected dR Vs WPT", 400, 0, 400, 140, -7, 7);

  hist->AddTH2( "dPTWPT", " delta PT Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "dPTdR", " delta PT Vs deltaR", 400, -200, 200, 140, -7, 7);
  hist->AddTH2( "dPTWPTdR", " delta PT Vs WPT vs delta R", 400, 0, 400, 200, 0, 200);

  hist->AddTH2( "dPTWPTVsWPT", " delta PT/WPT  Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "dRdijetW", " delta R dijet and W  Vs WPT", 400, 0, 400, 140, -7, 7);

  return true;
}       // -----  end of function GenParAna::BookHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::AnaWdiJets
//  Description:  
// ===========================================================================
bool GenParAna::AnaWdiJets() const
{
  for(unsigned int i=0; i < Widx.size(); ++i)
  {
    WDecay wbos = Widx.at(i);
    //PrintWDecay(wbos);
    if (wbos.isLeptonic_) continue;
    const reco::Candidate* w = wbos.Widx_.Pruned;
    const reco::Candidate* p1 = wbos.had1idx_.Pruned;
    const reco::Candidate* p2 = wbos.had2idx_.Pruned;
    assert(p1->pt() > 0);
    assert(p2->pt() > 0);
    double p1p2DeltaR = reco::deltaR<reco::Candidate::LorentzVector>(p1->p4(), p2->p4());

    hist->FillTH1("WPT", w->pt());
    hist->FillTH2("WDR", 2*w->mass() / w->pt(),  p1p2DeltaR);
    if (p1->pt() > 20 && p2->pt() > 20)
    {
      hist->FillTH2("WDR2", 2*w->mass() / w->pt(), p1p2DeltaR);
      hist->FillTH2("dPTWPT", w->pt(), p1->pt() - p2->pt());
    }
    hist->FillTH2("dPTWPTVsWPT", w->pt(), fabs(p1->pt() - p2->pt())/w->pt());
    hist->FillTH2("dPTWPTdR", w->pt(), fabs(p1->pt() - p2->pt()), p1p2DeltaR);
    hist->FillTH2("dPTdR", p1->pt() - p2->pt(), p1p2DeltaR);
    hist->FillTH2("WDRPT", w->pt(), 2*w->mass() / w->pt() - p1p2DeltaR);
    hist->FillTH2("WDRPT1", w->pt(), 2*w->mass() / w->pt());
    hist->FillTH2("WDRPT2", w->pt(), p1p2DeltaR);
    reco::Candidate::LorentzVector dijetW = p1->p4()+ p2->p4();
    double dijetWDeltaR = reco::deltaR<reco::Candidate::LorentzVector>(dijetW, w->p4());
    hist->FillTH2("dRdijetW", w->pt(), dijetWDeltaR);

    
  }
  return true;
}       // -----  end of function GenParAna::AnaWdiJets  -----


//#include "FWCore/Framework/interface/MakerMacros.h"
////define this as a plug-in
//DEFINE_FWK_MODULE(GenParAna)
//
