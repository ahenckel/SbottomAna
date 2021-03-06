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
    //reVal[GetEventFilterList(keyString)] = -999.8;
    reVal[keyString] = -999.9;

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
void passBaselineTTZ(NTupleReader &tr, std::map<std::string, BaselineVessel*> &blvMap,
    std::string leps, int JEC)
{
  std::stringstream ss;
  ss <<"TTZ" << ChooseLepPath(leps);
  if (JEC > 0) ss <<"JECup";
  if (JEC < 0) ss <<"JECdn";


  if (blvMap.find(ss.str()) == blvMap.end())
  {
    
    std::cout << ss.str() << std::endl;
    blvMap[ss.str()] =  new BaselineVessel(tr, ss.str());
    
    if (JEC == 0)
    {
      blvMap[ss.str()]->jetVecLabel = "jetsLVecLepCleaned";
      blvMap[ss.str()]->CSVVecLabel = "recoJetsBtag_0_LepCleaned";
    }
    if (JEC > 0)
    {
      blvMap[ss.str()]->jetVecLabel = "jetLVecLepCleaned_jecUp";
      blvMap[ss.str()]->CSVVecLabel = "recoJetsBtagLepCleaned_jecUp";
    }
    if (JEC <0)
    {
      blvMap[ss.str()]->jetVecLabel = "jetLVecLepCleaned_jecDn";
      blvMap[ss.str()]->CSVVecLabel = "recoJetsBtagLepCleaned_jecDn";
    }
  }
 
  blvMap[ss.str()]->PassBaseline();
  blvMap[ss.str()]->GetMHT();
  blvMap[ss.str()]->GetLeptons();

  VarPerEvent var(&tr);
  var.GetRecoZ(ss.str(), leps);
  var.GetNoLepJEC(JEC);
}

// ===  FUNCTION  ============================================================
//         Name:  passBaselineMHT
//  Description:  /* cursor */
// ===========================================================================
void passBaselineMHT(NTupleReader &tr)
{
  // Run baseline
  BaselineVessel blv(tr, "MHT");
  blv.jetVecLabel = "jetsLVecLepCleaned";
  blv.CSVVecLabel = "recoJetsBtag_0_LepCleaned";
  blv.METLabel = "MHT";
  blv.METPhiLabel = "MHTPhi";

  blv.PassBaseline();
  blv.GetMHT();
}       // -----  end of function passBaselineMHT  -----

// ===  FUNCTION  ============================================================
//         Name:  passBaselineJECup
//  Description:  
// ===========================================================================
void passBaselineJECup(NTupleReader &tr)
{
  VarPerEvent var(&tr);
  var.GetJEC();

  // Run baseline
  BaselineVessel blv(tr, "JECup");
  blv.jetVecLabel = "jetLVec_jecUp";
  blv.CSVVecLabel = "recoJetsBtag_jecUp";

  blv.PassBaseline();
  blv.GetMHT();
}       // -----  end of function passBaselineJECup  -----

// ===  FUNCTION  ============================================================
//         Name:  passBaselineJECdn
//  Description:  
// ===========================================================================
void passBaselineJECdn(NTupleReader &tr)
{
  // Run baseline
  BaselineVessel blv(tr, "JECdn");
  blv.jetVecLabel = "jetLVec_jecDn";
  blv.CSVVecLabel = "recoJetsBtag_jecDn";
  blv.PassBaseline();
  blv.GetMHT();
  return ;
}       // -----  end of function passBaselineJECdn  -----


// ===  FUNCTION  ============================================================
//         Name:  passBaselineZinv
//  Description:  
// ===========================================================================
void passBaselineZinv(NTupleReader &tr, std::string leps)
{
  std::stringstream ss;
  ss <<"Zinv" << ChooseLepPath(leps);

  VarPerEvent var(&tr);
  var.GetRecoZ(ss.str(), leps);
  BaselineVessel blv(tr, ss.str());
  blv.jetVecLabel = "jetsLVecLepCleaned";
  blv.CSVVecLabel = "recoJetsBtag_0_LepCleaned";
  try
  {
    tr.getVar<double>("cleanMetPt" + ss.str());
  }
  catch (std::string &var)
  {
    std::cout << "Missing cleanMET! Need to run VarPerEvent::GetRecoZ() first!" << std::endl;
    throw std::runtime_error("Wrong configs of registerFunction");
  }
  blv.METLabel = "cleanMetPt" + ss.str();
  blv.METPhiLabel = "cleanMetPhi" + ss.str();
  blv.PassBaseline();
  blv.GetMHT();
}       // -----  end of function passBaselineZinv  -----

// ===  FUNCTION  ============================================================
//         Name:  ChooseLepPath
//  Description:  /* cursor */
// ===========================================================================
std::string ChooseLepPath(std::string leps)
{
  std::bitset<3> lepbit(leps);
  std::stringstream ss;
  ss <<  (lepbit.test(0) ? "M":"") <<  (lepbit.test(1) ? "E":"") <<  (lepbit.test(2) ? "T":"");
  return ss.str();
}       // -----  end of function ChooseLepPath  -----

// ===  FUNCTION  ============================================================
//         Name:  GetEventFilterList
//  Description:  
// ===========================================================================
std::string GetEventFilterList(std::string dataset)
{
  assert(dataset != "");
  if (dataset.find("Data") == std::string::npos)
  {
    return "";
  }
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep("_");
  tokenizer tokens(dataset, sep);
  tokenizer::iterator tok_iter = tokens.begin();
  tok_iter ++; // Get keyword after Data_

  std::string cscfile = "";
  std::stringstream ss;
  ss << "FileList/" << *tok_iter <<"_csc2015.txt";
  std::ifstream ifile(ss.str());
  if (!ifile)
  {
    ss.str("");
    ss << *tok_iter <<"_csc2015.txt";
    std::ifstream ifile2(ss.str());
    if (!ifile2)
    {
      std::cout << " No BeamHole File for " << *tok_iter << std::endl;
      throw 0;
    } else
      cscfile = ss.str();
  } else cscfile = ss.str();
  assert(cscfile != "");

  return cscfile;
}       // -----  end of function GetEventFilterList  -----

// ===  FUNCTION  ============================================================
//         Name:  GetGenChild
//  Description:  
// ===========================================================================
int GetGenChild( std::vector<int> &genDecayPdgIdVec, std::vector<int> &genDecayMomIdxVec ,
    int parent, std::vector<int> pdgs)
{
  for (unsigned int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    if (abs(genDecayMomIdxVec[i]) == parent)
    {
      for(unsigned int j=0; j < pdgs.size(); ++j)
      {
        if (abs(genDecayPdgIdVec.at(i)) == pdgs.at(j))
        {
          return i;
        }
      }
    }
  }
  return -1;
}       // -----  end of function GetGenChild  -----

// ===  FUNCTION  ============================================================
//         Name:  GetGenChilds
//  Description:  
// ===========================================================================
std::vector<int>  GetGenChilds(std::vector<int> &genDecayPdgIdVec, std::vector<int> &genDecayMomIdxVec, int parent, std::vector<int> pdgs)
{
  
  std::vector<int> outs;
  
  for (unsigned int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    if (abs(genDecayMomIdxVec[i]) == parent)
    {
      for(unsigned int j=0; j < pdgs.size(); ++j)
      {
        if (abs(genDecayPdgIdVec.at(i)) == pdgs.at(j))
        {
          outs.push_back(i);
        }
      }
    }
  }

  return outs;
}       // -----  end of function GetGenChilds  -----


// ===  FUNCTION  ============================================================
//         Name:  GetGenTops
//  Description:  
// ===========================================================================
bool GetGenTops(NTupleReader &tr, std::vector<TLorentzVector> &vGenTops,
    std::vector<int> &vGenTopCharge, std::vector<TLorentzVector> &vGenLeps)
{
  std::vector<TLorentzVector> genDecayLVec      ;
  std::vector<int>            genDecayIdxVec    ;
  std::vector<int>            genDecayPdgIdVec  ;
  std::vector<int>            genDecayMomIdxVec ;
  vGenTops.clear();
  vGenTopCharge.clear();
  vGenLeps.clear();

  try{
    genDecayLVec      = tr.getVec<TLorentzVector> ("genDecayLVec");
    genDecayIdxVec    = tr.getVec<int>            ("genDecayIdxVec");
    genDecayPdgIdVec  = tr.getVec<int>            ("genDecayPdgIdVec");
    genDecayMomIdxVec = tr.getVec<int>            ("genDecayMomIdxVec");
  }
  catch(std::string &var)
  {
    return false;
  }

  for (unsigned int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    if (abs(genDecayPdgIdVec[i]) == 6)
    {
      vGenTops.push_back(genDecayLVec.at(i));
      vGenTopCharge.push_back(genDecayPdgIdVec[i]/6);

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the leptons  ~~~~~
      int Widx_ = GetGenChild(genDecayPdgIdVec, genDecayMomIdxVec, genDecayIdxVec[i], {24});
      int Lepidx_ = GetGenChild(genDecayPdgIdVec, genDecayMomIdxVec, genDecayIdxVec[Widx_], {11, 13, 15});
      if (Lepidx_ != -1)
      {
        vGenLeps.push_back(genDecayLVec.at(Lepidx_));
      }
    }
  }

  return true;
}       // -----  end of function GetGenTops  -----

// ===  FUNCTION  ============================================================
//         Name:  GetTopPtReweight
//  Description:  Reweighting top pt from 8TeV recommendation
//  https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopPtReweighting
// ===========================================================================
void GetTopPtReweight(NTupleReader &tr, std::map<std::string, double> &SamplePro)
{
  double topPtWeight = 1;

  std::string proname = "";
  for(auto &it : SamplePro)
  {
    if (it.second == -999.9)
    {
      proname = it.first;
      break;
    }
  }

  // For data and not_TTbar MC, set the weight to 1.
  if (tr.getVar<int>("run") != 1  || proname.find("TTbar") == std::string::npos)
  {
    tr.registerDerivedVar("TopPtReweight", topPtWeight);
    return;
  }
  
  std::vector<TLorentzVector> vGenTops;
  std::vector<int> vGenTopCharge;
  std::vector<TLorentzVector> vGenLeps;
  GetGenTops(tr, vGenTops, vGenTopCharge, vGenLeps);
  if ( vGenTops.size() != 2)
  {
    tr.registerDerivedVar("TopPtReweight", topPtWeight);
    return;
  }

  // Getting the reweight factor SF(x)=exp(a+bx)
  double a = 0.0615; //13TeV
  double b = -0.0005; //13TeV
  /* 8TeV
   *if (vGenLeps.size() == 1)  // Semileptonic
   *{
   *  a = 0.159;
   *  b = -0.00141;
   *}
   *if (vGenLeps.size() == 2)  // Dilepton
   *{ 
   *  a = 0.148;
   *  b = -0.00129;
   *} 
   *if (vGenLeps.size() == 0)  // all combined, but use for all hadroinc case here
   *{
   *  a = 0.156;
   *  b = -0.00137;
   *}
   */

  // weight=sqrt(SF(top)SF(anti-top))
  double weight = 1.0;
  int weightcount = 0;
  for(unsigned int i=0; i < vGenTops.size(); ++i)
  {
    TLorentzVector gentop = vGenTops.at(i);
    if (gentop.Pt() <= 400)
    {
      weight = weight * TMath::Exp(a + b*gentop.Pt()); 
      weightcount ++;
    }
    //std::cout << "pt " << gentop.Pt() <<" weight " << weightcount << " a " << a <<" b " << b << " exp " << TMath::Exp(a + b*gentop.Pt()) << std::endl;
  }
  
  if (weightcount == 2)
  {
    topPtWeight = sqrt(weight);
  }
  else
    topPtWeight = weight;


  tr.registerDerivedVar("TopPtReweight", topPtWeight);

  return ;
}       // -----  end of function GetTopPtReweight  -----

// ===  FUNCTION  ============================================================
//         Name:  GetProcName
//  Description:  
// ===========================================================================
std::string GetProcName(std::map<std::string, double> &SamplePro)
{
  std::string proname = "";
  for(auto &it : SamplePro)
  {
    if (it.second == -999.9)
    {
      proname = it.first;
      break;
    }
  }
  return proname;
}       // -----  end of function GetProcName  -----
