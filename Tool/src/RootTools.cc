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
#include "samples2.h"

// ===  FUNCTION  ============================================================
//         Name:  GetXS
//  Description:  /* cursor */
// ===========================================================================
double GetXS(std::string name)
{

  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep("/.");
  tokenizer tokens(name, sep);

  int totalsize = sizeof(mcStr) /sizeof(mcStr[0]);
  int foundidx = -1;
  int idx = 0;

  std::cout << " input " << name << std::endl;
  for (tokenizer::iterator tok_iter = tokens.begin();
      tok_iter != tokens.end(); ++tok_iter)
  {
    std::cout << "<" << *tok_iter << "> ";

    idx = 0;
    while (idx < totalsize)
    {
      if (*tok_iter == std::string(mcStr[idx]))
      {
        foundidx = idx;
        break;
      }
      idx++;
    }
  }
  std::cout << "\n";

  if (foundidx != -1)
  {
    std::cout << " Found " << mcStr[foundidx] << " with XS " <<  xSecArr[foundidx]<< std::endl;
  }
  else
    return -1;


  return xSecArr[foundidx];
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
//         Name:  PrintTLorentz
//  Description:  /* cursor */
// ===========================================================================
bool PrintTLorentz(int event, std::string name, std::vector<TLorentzVector> obj) 
{
  for (int i = 0; i < obj.size(); ++i)
  {
    TLorentzVector temp = obj.at(i);
    if (temp.Pt()<3) continue;
    std::cout << event <<"," << name <<"," << temp.Px() <<"," << temp.Py() <<"," <<
      temp.Pz()<<"," << temp.E()<< std::endl;
  }
  return true;
}       // -----  end of function PrintTLorentz  -----

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
  for (int i = 0; i < tr->getVec<int>("genDecayPdgIdVec").size(); ++i)
  {
    for (int j = 0; j < pdgid.size(); ++j)
    {
      if (abs(tr->getVec<int>("genDecayPdgIdVec").at(i)) == abs(pdgid.at(j)))
      {
        temp.push_back( tr->getVec<TLorentzVector> ("genDecayLVec").at(i));
      }
    }
  }
  return temp;
}       // -----  end of function GetGenParticles  -----
