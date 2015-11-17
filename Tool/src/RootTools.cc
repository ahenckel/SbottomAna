// ===========================================================================
// 
//       Filename:  RootTools.cc
// 
//    Description:  Some tools for ROOT
// 
//        Version:  1.0
//        Created:  06/16/2015 03:29:18 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================


#include "RootTools.h"

// ===  FUNCTION  ============================================================
//         Name:  GetXS
//  Description:  /* cursor */
// ===========================================================================
std::map<std::string, double> GetXS(std::string name)
{
  AnaSamples::SampleSet        allSamples;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep("/.-");
  tokenizer tokens(name, sep);

  //double scaleMC = allSamples[keyString].getWeight();
  std::string keyString("");
  std::map<std::string, double> reVal;

  std::cout << " input " << name << std::endl;
  for (tokenizer::iterator tok_iter = tokens.begin();
      tok_iter != tokens.end(); ++tok_iter)
  {
    std::cout << "<" << *tok_iter << "> ";

    if (allSamples[*tok_iter] != allSamples.null())
    {
      keyString = *tok_iter;
      break;
    }
  }
  std::cout << "\n";

  if (keyString != "")
  {
    reVal["key"] = 0;
    reVal["xsec"] = allSamples[keyString].xsec;
    reVal["lumi"] = allSamples[keyString].lumi;
    reVal["kfactor"] = allSamples[keyString].kfactor;
    reVal["nEvts"] = allSamples[keyString].nEvts;
    reVal["color"] = allSamples[keyString].color;

    std::cout << " Found " << keyString << " with XS " <<  allSamples[keyString].xsec  << " with kFactor " 
      << allSamples[keyString].kfactor << std::endl;
  }

  return  reVal;
}       // -----  end of function GetXS  -----

// ===  FUNCTION  ============================================================
//         Name:  GetCutBin
//  Description:  /* cursor */
// ===========================================================================
int GetCutBin( std::vector<std::string> &CutOrder , std::string name) 
{
  std::vector<std::string>::iterator  it = std::find(CutOrder.begin(), CutOrder.end(), name);
  if (it == CutOrder.end())
  {
    std::cout << "Warning: wrong cut name!! " << std::endl;
    return -1;
  } else
  {
    return std::distance(CutOrder.begin(), it);
  } 
  
  return -1;
}       // -----  end of function GetCutBin  -----


// ===  FUNCTION  ============================================================
//         Name:  CalMT
//  Description:  /* cursor */
// ===========================================================================
double CalMT(TLorentzVector J1, TLorentzVector J2)
{
  return sqrt(2 * J1.Pt() * J2.Pt() * (1 - cos(J1.DeltaPhi(J2))));
}       // -----  end of function CalMT  -----

// ===  FUNCTION  ============================================================
//         Name:  CalMCT
//  Description:  /* cursor */
// ===========================================================================
double CalMCT(TLorentzVector J1, TLorentzVector J2)
{
  return sqrt(2 * J1.Pt() * J2.Pt() * (1 + cos(J1.DeltaPhi(J2))));
}       // -----  end of function CalMCT  -----

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

// ===  FUNCTION  ============================================================
//         Name:  PrintEvent
//  Description:  
// ===========================================================================
bool PrintEvent(NTupleReader *tr)
{
  int event = tr->getEvtNum();
  //std::cout << " GenParticles : ";
  //for (int i = 0; i < tr->getVec<int>("genDecayPdgIdVec").size(); ++i)
  //{
    //std::cout <<  tr->getVec<int>("genDecayPdgIdVec").at(i) <<" ";
  //}
  //std::cout << " " << std::endl;

  std::vector<TLorentzVector> tempJet;

  for(unsigned int i=0; i < tr->getVec<TLorentzVector>("jetsLVec").size(); ++i)
  {
    if (tr->getVec<TLorentzVector>("jetsLVec").at(i).Pt() > 30)
    {
      tempJet.push_back(tr->getVec<TLorentzVector>("jetsLVec").at(i));
    }
  } 
  PrintTLorentz(event, "AK4Jet", tempJet);
  //PrintTLorentz(event, "GEN4Jet", tr->getVec<TLorentzVector>("Gen4LVec"));

  PrintTLorentz(event, "Top", GetGenParticles({6}, tr));
  PrintTLorentz(event, "W", GetGenParticles({24}, tr));
  PrintTLorentz(event, "Lep", GetGenParticles({11, 13, 15}, tr));
  PrintTLorentz(event, "B", GetGenParticles({5}, tr));
  PrintTLorentz(event, "Had", GetGenParticles({1, 2, 3, 4, 21}, tr));

  TLorentzVector METLV(0, 0, 0, 0);
  METLV.SetPtEtaPhiE(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
  std::vector<TLorentzVector> tempMET;
  tempMET.push_back(METLV);
  PrintTLorentz(event, "MET", tempMET);

  return true;
}       // -----  end of function PrintEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  GetGenParticles
//  Description:  /* cursor */
// ===========================================================================
std::vector<TLorentzVector> GetGenParticles(const std::vector<int> pdgid, const NTupleReader *tr)
{
  std::vector<TLorentzVector> temp;
  for (unsigned int i = 0; i < tr->getVec<int>("genDecayPdgIdVec").size(); ++i)
  {
    for (unsigned int j = 0; j < pdgid.size(); ++j)
    {
      if (abs(tr->getVec<int>("genDecayPdgIdVec").at(i)) == abs(pdgid.at(j)))
      {
        temp.push_back( tr->getVec<TLorentzVector> ("genDecayLVec").at(i));
      }
    }
  }
  return temp;
}       // -----  end of function GetGenParticles  -----


// ===  FUNCTION  ============================================================
//         Name:  PrintTLorentz
//  Description:  /* cursor */
// ===========================================================================
bool PrintTLorentz(int event, std::string name, std::vector<TLorentzVector> obj) 
{
  for (unsigned int i = 0; i < obj.size(); ++i)
  {
    TLorentzVector temp = obj.at(i);
    if (temp.Pt()<3) continue;
    std::cout << event <<"," << name <<"," << temp.Px() <<"," << temp.Py() <<"," <<
      temp.Pz()<<"," << temp.E()<< std::endl;
  }

  return true;
}       // -----  end of function PrintTLorentz  -----

// ===  FUNCTION  ============================================================
//         Name:  PrintTopEvent
//  Description:  /* cursor */
// ===========================================================================
bool PrintTopEvent( NTupleReader &tr, std::vector<TopDecay> &vTops) 
{
  int event = tr.getEvtNum();
  //PrintTLorentz(event, "HEPTop", TopMap["HEPTop"]->RecoTops);
  //PrintTLorentz(event, "T3Top", TopMap["T3Top"]->RecoTops);
  PrintTLorentz(event, "AK4Jet", tr.getVec<TLorentzVector>("jetsLVec"));
  PrintTLorentz(event, "GEN4Jet", tr.getVec<TLorentzVector>("Gen4LVec"));


  std::vector<TLorentzVector> genDecayLVec     = tr.getVec<TLorentzVector> ("genDecayLVec");

  std::vector<TLorentzVector> tempTop;
  std::vector<TLorentzVector> tempW;
  std::vector<TLorentzVector> tempb;
  std::vector<TLorentzVector> tempLep;
  std::vector<TLorentzVector> temphad;
  for(unsigned int i=0; i < vTops.size(); ++i)
  {
    TopDecay gentop = vTops.at(i);
    if (gentop.topidx_ != -1) tempTop.push_back(genDecayLVec[gentop.topidx_]);
    if (gentop.Widx_ != -1) tempW.push_back(genDecayLVec[gentop.Widx_]);
    if (gentop.bidx_ != -1) tempb.push_back(genDecayLVec[gentop.bidx_]);
    if (gentop.Lepidx_ != -1) tempLep.push_back(genDecayLVec[gentop.Lepidx_]);
    if (gentop.had1idx_ != -1 && static_cast<unsigned int>(gentop.had1idx_) <= genDecayLVec.size())
    {
      temphad.push_back(genDecayLVec[gentop.had1idx_]);
    }
    if (gentop.had2idx_ != -1 && static_cast<unsigned int>(gentop.had2idx_) <= genDecayLVec.size())
    {
      temphad.push_back(genDecayLVec[gentop.had2idx_]);
    }
  }
  PrintTLorentz(event, "Top", tempTop);
  PrintTLorentz(event, "W", tempW);
  PrintTLorentz(event, "Lep", tempLep);
  PrintTLorentz(event, "B", tempb);
  PrintTLorentz(event, "Had", temphad);

  // Type3 Jets
  std::vector<TLorentzVector> jetsforTT;

  for(unsigned int i=0; i < tr.getVec<TLorentzVector>("jetsLVec").size(); ++i)
  {
    if (tr.getVec<TLorentzVector>("jetsLVec").at(i).Pt() > 30) 
      jetsforTT.push_back(tr.getVec<TLorentzVector>("jetsLVec").at(i));
  }

  //std::vector<TLorentzVector> jetsforTTplt;
  //for(unsigned int i=0; i < TopMap["T3Top"]->Type3Jets.size(); ++i)
  //{
    //jetsforTTplt.push_back(jetsforTT.at(TopMap["T3Top"]->Type3Jets.at(i)));
  //}
  //PrintTLorentz(event, "T3Jet", jetsforTTplt);

  return true;
}       // -----  end of function PrintTopEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  LTVMatch
//  Description:  
// ===========================================================================
bool LTVMatch(TLorentzVector &V1, TLorentzVector V2, double dR) 
{
  return V1.DeltaR(V2) <= dR;
}       // -----  end of function LTVMatch  -----

// ===  FUNCTION  ============================================================
//         Name:  FindVectorIdX
//  Description:  
// ===========================================================================
  //template<typename T, typename V> 
//int FindVectorIdX(const T &TeV, const V &t)
//{
   //auto it = find(TeV.begin(), TeV.end(), t);
   //if (it == TeV.end()) return -1;
   //else return it-TeV.begin();
//}       // -----  end of function FindVectorIdX  -----

// ===  FUNCTION  ============================================================
//         Name:  FindVectorIdX
//  Description:  
// ===========================================================================
int FindVectorIdX(std::vector<std::string> &TeV, std::string t)
{
  auto it = find(TeV.begin(), TeV.end(), t);
  if (it == TeV.end()) return -1;
  else return it-TeV.begin();
}       // -----  end of function FindVectorIdX  -----


// ===  FUNCTION  ============================================================
//         Name:  HasTLV
//  Description:  /* cursor */
// ===========================================================================
int HasTLV(TLorentzVector &obj, std::vector<TLorentzVector> &TLVs)
{
  for(std::vector<TLorentzVector>::const_iterator it=TLVs.begin();
    it!=TLVs.end(); ++it)
  {
    if (LTVMatch(obj, *it))
    {
      return it-TLVs.begin();
    }
  }
  return -1;
}       // -----  end of function HasTLV  -----

//**************************************************************************//
//                            Register Functions                            //
//**************************************************************************//
void passBaselineTTZ(NTupleReader &tr)
{
  VarPerEvent var(&tr);
  var.GetRecoZ("TTZ");
  BaselineVessel blv("TTZ");
  blv.jetVecLabel = "jetsLVecLepCleaned";
  blv.CSVVecLabel = "recoJetsBtag_0_LepCleaned";
  blv.prepareTopTagger();
  blv.passBaseline(tr);
  blv.GetnTops(&tr);
}

// ===  FUNCTION  ============================================================
//         Name:  passBaselineZinv
//  Description:  
// ===========================================================================
void passBaselineZinv(NTupleReader &tr)
{
  VarPerEvent var(&tr);
  var.GetRecoZ("Zinv");
  BaselineVessel blv("Zinv");
  blv.jetVecLabel = "jetsLVecLepCleaned";
  blv.CSVVecLabel = "recoJetsBtag_0_LepCleaned";
  try
  {
    tr.getVar<double>("cleanMetPtZinv");
  }
  catch (std::string &var)
  {
    std::cout << "Missing cleanMET! Need to run VarPerEvent::GetRecoZ() first!" << std::endl;
    throw std::runtime_error("Wrong configs of registerFunction");
  }
  blv.METLabel = "cleanMetPtZinv";
  blv.METPhiLabel = "cleanMetPhiZinv";
  blv.prepareTopTagger();
  blv.passBaseline(tr);
  blv.GetnTops(&tr);
}       // -----  end of function passBaselineZinv  -----
