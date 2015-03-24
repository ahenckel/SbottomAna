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
  if (WPar->numberOfDaughters() == 1 && fabs( WPar->daughter(0)->pdgId()) == 24)
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

//----------------------------------------------------------------------------
//  If it is an intermedia top , nest the function
//  NOTE: Sometime, top can branch to 3 particles:
//  tau daughters? 3
//  top dau -6 mass 170.058
//  top dau 523 mass 5.3252
//  top dau 1 mass 1.13833
//  So we need to test all the possible
//----------------------------------------------------------------------------
  if (TopPar->numberOfDaughters() != 2)
  {
  //std::cout << " tau daughters? " << TopPar->numberOfDaughters()<< std::endl;
    for(unsigned int i=0; i < TopPar->numberOfDaughters(); ++i)
    {
      //std::cout << " top dau " << dau->pdgId() << " mass " << dau->mass()<< std::endl;
      if ( fabs(TopPar->daughter(i)->pdgId()) == 6)
      {
        return GetTopDecay(TopPar->daughter(i));
      }
    }
  }

//----------------------------------------------------------------------------
//  If this W decays
//----------------------------------------------------------------------------
  TopDecay temp;
  temp.topidx_ = GetPackedPars(TopPar);
  bool hasProducts = false; //only store the top quarks has daughters

  assert( TopPar->numberOfDaughters() == 2);
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
  Tauidx.clear();

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

}       // -----  end of function GenParAna::GetLepCount  -----


// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetTopCount
//  Description:  
// ===========================================================================
int GenParAna::GetTopCount() const
{
  return Topidx.size();
}       // -----  end of function GenParAna::GetTopCount  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::BookHistogram
//  Description:  
// ===========================================================================
bool GenParAna::BookHistogram()
{
  hist->AddTH1( "WPT", " WPT",  400, 0, 400);
  hist->AddTH1( "MergedWPT", "Merged WPT",  400, 0, 400);
  hist->AddTH2( "WDR", " Decay dR vs expected dR",  70, 0, 7, 70, 0, 7);
  hist->AddTH2( "WDR2", " Decay dR vs expected dR with quark pt cut ",  70, 0, 7, 70, 0, 7);
  hist->AddTH2( "WDRPT", " Decay dR vs expected dR Vs WPT",  400, 0, 400, 140, -7, 7);

  //hist->AddTH1( "WPT", " WPT", "Gen W PT", "Number of W",  400, 0, 400);
  //hist->AddTH2( "WDR", " Decay dR vs expected dR", " 2 * M / PT", "diquark DeltaR",  70, 0, 7, 70, 0, 7);
  //hist->AddTH2( "WDR2", " Decay dR vs expected dR with quark pt cut ", " 2 *M / PT", "diquark DeltaR (PT>20)",  70, 0, 7, 70, 0, 7);
  //hist->AddTH2( "WDRPT", " Decay dR vs expected dR Vs WPT", "WPT", "2*M/PT - diquark DeltaR",  400, 0, 400, 140, -7, 7);


  hist->AddTH2( "WDRPT1", " Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "WDRPT2", " Expected dR Vs WPT", 400, 0, 400, 140, -7, 7);

  hist->AddTH2( "dPTWPT", " delta PT Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "SumPTWPT", "Sum PT Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "SumPTWPT-WPT", "Sum PT/WPT  Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "dPTdR", " delta PT Vs deltaR", 400, -200, 200, 140, -7, 7);
  hist->AddTH2( "dPTWPTdR", " delta PT Vs WPT vs delta R", 400, 0, 400, 200, 0, 200);

  hist->AddTH2( "dPTWPTVsWPT", " delta PT/WPT  Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "dRdijetW", " delta R dijet and W  Vs WPT", 400, 0, 400, 140, -7, 7);

  hist->AddTH2( "WPT-TopPT", "WPT Vs Top PT", 400, 0, 400, 400, 0, 400);
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
    if (p1p2DeltaR < 0.8 && p1p2DeltaR > 0.4)
      hist->FillTH1("MergedWPT", w->pt());

    hist->FillTH1("WPT", w->pt());
    hist->FillTH2("WDR", 2*w->mass() / w->pt(),  p1p2DeltaR);
    if (p1->pt() > 20 && p2->pt() > 20)
    {
      hist->FillTH2("WDR2", 2*w->mass() / w->pt(), p1p2DeltaR);
    }
    hist->FillTH2("dPTWPT", w->pt(), p1->pt() - p2->pt());
    hist->FillTH2("SumPTWPT", w->pt(), p1->pt() + p2->pt());
    hist->FillTH2("SumPTWPT-WPT", w->pt(), (p1->pt() + p2->pt())/w->pt());
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


// ===  FUNCTION  ============================================================
//         Name:  GenParAna::AnaTopandW
//  Description:  
// ===========================================================================
bool GenParAna::AnaTopandW() const
{
  for (size_t i = 0; i < Topidx.size(); ++i)
  {
    const reco::Candidate* t = Topidx.at(i).topidx_.Pruned;
    //const reco::Candidate* b = Topidx.at(i).bidx_.Pruned;
    const reco::Candidate* w = Topidx.at(i).widx_.Widx_.Pruned;
    assert(t != NULL);
    assert(w != NULL);
    hist->FillTH2("WPT-TopPT", w->pt(), t->pt());
    //if (Topidx.at(i).widx_.isLeptonic_) continue;
    //const reco::Candidate* p1 = Topidx.at(i).widx_.had1idx_.Pruned;
    //const reco::Candidate* p2 = Topidx.at(i).widx_.had2idx_.Pruned;

    //double p1p2DeltaR = reco::deltaR<reco::Candidate::LorentzVector>(p1->p4(), p2->p4());
    //double bp1DeltaR = reco::deltaR<reco::Candidate::LorentzVector>(p1->p4(), b->p4());
    //double bp2DeltaR = reco::deltaR<reco::Candidate::LorentzVector>(p2->p4(), b->p4());
    //double bwDeltaR = reco::deltaR<reco::Candidate::LorentzVector>(w->p4(), b->p4());

    //// is top merged in one jet
    //if (p1p2DeltaR < 0.4 && bp2DeltaR < 0.4 && bp2DeltaR < 0.4) continue;

    // if 

  }
  return true;
}       // -----  end of function GenParAna::AnaTopandW  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetTop
//  Description:  
// ===========================================================================
bool GenParAna::GetTop(edm::Handle<reco::GenParticleCollection> PrunedGenHdl)
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

  return hasTop;
}       // -----  end of function GenParAna::GetTop  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetW
//  Description:  
// ===========================================================================
bool GenParAna::GetW(edm::Handle<reco::GenParticleCollection> PrunedGenHdl)
{
  bool hasW = false;
  for (size_t i = 0; i < PrunedGenHdl->size(); ++i)
  {
    if (fabs(PrunedGenHdl->at(i).pdgId()) == 24)
    {
      GetWDecay(&PrunedGenHdl->at(i));
      hasW = true;
    }
  }

  return hasW;
}       // -----  end of function GenParAna::GetW  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetTau
//  Description:  
// ===========================================================================
bool GenParAna::GetTau(edm::Handle<reco::GenParticleCollection> PrunedGenHdl)
{
  bool hastau = false;
  for (size_t i = 0; i < PrunedGenHdl->size(); ++i)
  {
    if (fabs(PrunedGenHdl->at(i).pdgId()) == 15)
    {
      GetTauDecay(&PrunedGenHdl->at(i));
      hastau = true;
    }
  }
  return hastau;
}       // -----  end of function GenParAna::GetTau  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::GetTauDecay
//  Description:  
// ===========================================================================
bool GenParAna::GetTauDecay(const reco::Candidate* TauPar)
{
   // Tau will radia photons, looping over until the final tau decay 
  for(unsigned int i=0; i < TauPar->numberOfDaughters(); ++i)
  {
    //std::cout << " top dau " << dau->pdgId() << " mass " << dau->mass()<< std::endl;
    if ( fabs(TauPar->daughter(i)->pdgId()) == 15)
    {
      return GetTauDecay(TauPar->daughter(i));
    }
  }

  //----------------------------------------------------------------------------
  //  If this is the final tau
  //----------------------------------------------------------------------------
  TauDecay temp;
  temp.tauidx_  = GetPackedPars(TauPar);

  //int lepcount = 0;
  for (unsigned int i = 0; i < TauPar->numberOfDaughters(); ++i)
  {
    const reco::Candidate* dau = TauPar->daughter(i);
    assert(dau->pdgId() != 15);

    //Get leptonic decay tau
    if ( fabs(dau->pdgId()) == 11 || fabs(dau->pdgId()) == 13 )
    {
      temp.Lepidx_ = GetPackedPars(dau);
      temp.isLeptonic_ = true;
    }

    //Get tau neutrino, multiple neutrino from tau?
    if ( fabs(dau->pdgId()) == 12 || fabs(dau->pdgId()) == 14 ||fabs(dau->pdgId()) == 16 )
      temp.PrunedNeus.push_back(dau);
    // Get prongs?
  }

  Tauidx.push_back(temp);
  return true;
}       // -----  end of function GenParAna::GetTauDecay  -----


// ===  FUNCTION  ============================================================
//         Name:  GenParAna::ProdGenJets
//  Description:  /* cursor */
// ===========================================================================
std::vector<fastjet::PseudoJet>  GenParAna::ProdGenJets(const std::vector<const reco::Candidate*> &PackedGens, const double jetPtMin, const double rParam_ )
{
  // fastjet inputs
  std::vector<fastjet::PseudoJet> fjInputs;        
  for(unsigned int i=0; i < PackedGens.size(); ++i)
  {
    const reco::Candidate* gen = PackedGens.at(i);
    fjInputs.push_back(fastjet::PseudoJet(gen->px(),  gen->py(), gen->pz(), gen->energy())); 
  }

  // fastjet jet definition: default AntiKT
  fastjet::JetDefinition fjJetDefinition = fastjet::JetDefinition(fastjet::antikt_algorithm,rParam_);

  // do fasjet area => accept corresponding parameters
  // default Ghost_EtaMax should be 5
  double ghostEtaMax = 5.0;
  // default Active_Area_Repeats 1
  int    activeAreaRepeats = 1;
  // default GhostArea 0.01
  double ghostArea = 0.01;
  fastjet::GhostedAreaSpec fjActiveArea = fastjet::GhostedAreaSpec(ghostEtaMax,activeAreaRepeats,ghostArea);
  fjActiveArea.set_fj2_placement(true);
  fastjet::AreaDefinition fjAreaDefinition = fastjet::AreaDefinition(fastjet::active_area_explicit_ghosts, fjActiveArea);

    // fastjet cluster sequence
  fastjet::ClusterSequence fjClusterSeq =  fastjet::ClusterSequenceArea( fjInputs, fjJetDefinition, fjAreaDefinition );
  return fastjet::sorted_by_pt(fjClusterSeq.inclusive_jets(jetPtMin)) ;
}       // -----  end of function GenParAna::ProdGenJets  -----

// ===  FUNCTION  ============================================================
//         Name:  GenParAna::ProdGenLVs
//  Description:  
// ===========================================================================
std::vector<TLorentzVector> GenParAna::ProdGenLVs(const std::vector<const reco::Candidate*> &PackedGens, const double jetPtMin, const double rParam)
{
  
  std::vector<TLorentzVector> LVs;

  std::vector<fastjet::PseudoJet> tempJets = ProdGenJets(PackedGens, jetPtMin, rParam);
  for(unsigned int i=0; i < tempJets.size(); ++i)
  {
    fastjet::PseudoJet jet = tempJets.at(i);
    TLorentzVector tempLV(jet.px(), jet.py(), jet.pz(), jet.e());
    LVs.push_back(tempLV);
  }
  return LVs;
}       // -----  end of function GenParAna::ProdGenLVs  -----

//#include "FWCore/Framework/interface/MakerMacros.h"
////define this as a plug-in
//DEFINE_FWK_MODULE(GenParAna)
//
