// ===========================================================================
// 
//       Filename:  TTZDiLep.cc
// 
//    Description:  An analysis for TTZ background in the 3 lepton channel
// 
//        Version:  1.0
//        Created:  10/07/2015 11:02:23
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#include "TTZDiLep.h"

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  TTZDiLep
// Description:  constructor
//----------------------------------------------------------------------------
TTZDiLep::TTZDiLep (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_)
: ComAna(name, tr_, OutFile, spec_)
{
  InitCutOrder(name);
  jetVecLabel = "prodJetsNoMu_jetsLVec";
  CSVVecLabel = "recoJetsBtag_0_MuCleaned";
}  // -----  end of method TTZDiLep::TTZDiLep  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  TTZDiLep
// Description:  copy constructor
//----------------------------------------------------------------------------
TTZDiLep::TTZDiLep ( const TTZDiLep &other ): ComAna(other)
{
}  // -----  end of method TTZDiLep::TTZDiLep  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  ~TTZDiLep
// Description:  destructor
//----------------------------------------------------------------------------
TTZDiLep::~TTZDiLep ()
{
}  // -----  end of method TTZDiLep::-TTZDiLep  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  TTZDiLep&
TTZDiLep::operator = ( const TTZDiLep &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method TTZDiLep::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::BookHistograms
//  Description:  
// ===========================================================================
bool TTZDiLep::BookHistograms()
{
  ComAna::BookHistograms();
  his->AddTH1C("JBT", "JBT", "JBT", "Events", 400, 0, 400);
  his->AddTH1C("bJetinTop", "bJetinTop", "bJetinTop", "Events", 5, -1, 4);
  return true;

}       // -----  end of function TTZDiLep::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::InitCutOrder
//  Description:  
// ===========================================================================
bool TTZDiLep::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("Filter");
  CutOrder.push_back("HasZ");
  CutOrder.push_back("nJets");
  CutOrder.push_back("BJets");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("MET70");
  CutOrder.push_back("BinTop");

  //Set the cutbit of each cut
  CutMap["NoCut"]  = "00000000000000000";
  CutMap["Filter"] = "00000000000000001";
  CutMap["HasZ"]   = "00000000000000011";
  CutMap["nJets"]  = "00000000000000111";
  CutMap["BJets"]  = "00000000000001111";
  CutMap["Tagger"] = "00000000000011111";
  CutMap["MET70"]  = "00000000000111111";
  CutMap["BinTop"] = "00000000001111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function TTZDiLep::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::CheckCut
//  Description:  
// ===========================================================================
bool TTZDiLep::CheckCut()
{
  
  cutbit.set(0 , tr->getVar<bool>("passNoiseEventFilterTTZ"));
  cutbit.set(1 , tr->getVar<bool>("passMuZinvSel"));
  cutbit.set(2 , tr->getVec<TLorentzVector>("jetsLVec_forTaggerTTZ").size() >= 4);
  cutbit.set(3 , tr->getVar<int>("cntCSVSTTZ") >= 1);
  cutbit.set(4 , tr->getVar<int>("nTopCandSortedCntTTZ") == 2);
  cutbit.set(5 , tr->getVar<double>(METLabel) < 70);

  std::vector<int> vbinTop = BJetTopAsso(i);
  cutbit.set(6 , vbinTop.size() > 0);

  return true;
}       // -----  end of function TTZDiLep::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::FillCut
//  Description:  
// ===========================================================================
bool TTZDiLep::FillCut()
{

//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  CheckCut();
  ComAna::RunEvent();
  bool passcuts = false;
  std::vector<int> vbinTop = BJetTopAsso(i);

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 
    ComAna::FillCut(i);
    //std::cout << ComAna::spec <<" _ "<< "jetsLVec_forTagger" + spec <<" "  << tr->getVec<TLorentzVector>("jetsLVec_forTagger" + spec).size()<< std::endl;
    int JBTcount = tr->getVar<int>(nTopLabel) * 100 + tr->getVar<int>(nCSVLabel) * 10 + tr->getVec<TLorentzVector>("jetsLVec_forTaggerTTZ").size();
    his->FillTH1(i, "JBT", JBTcount);

    if (vbinTop.empty()) 
      his->FillTH1(i, "bJetinTop", -1);
    else
      for(auto &it : vbinTop)
        his->FillTH1(i, "bJetinTop", it);


    if (i+1 == CutOrder.size()) 
    {
      passcuts = true;
    }
  }

  return passcuts;
}       // -----  end of function TTZDiLep::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::BJetTopAsso
//  Description:  
// ===========================================================================
std::vector<int> TTZDiLep::BJetTopAsso() const;
{
  const std::map<int, std::vector<TLorentzVector> > &mtopjets = tr->getMap<int, std::vector<TLorentzVector> >("mTopJetsTTZ");
  const std::vector<TLorentzVector> &jets = tr->getVec<TLorentzVector>(jetVecLabel);
  const std::vector<double> &bjets = tr->getVec<double>(CSVVecLabel);

  std::vector<int> vbinTop;

  for(unsigned int i=0; i < jets.size(); ++i)
  {
    if (bjets.at(i) < 0.890) continue;

    for(auto &topit  : mtopjets)
    {
      for(auto &topjet : topit.second)
      {
        if ( topjet.Pt() == jets.at(i).Pt()
            && topjet.Eta() == jets.at(i).Eta()
            && topjet.Phi() == jets.at(i).Phi() )
        {
          vbinTop.push_back(topit.first);
        }
      }
    }
  }
  return vbinTop;
}       // -----  end of function TTZDiLep::BJetTopAsso  -----


