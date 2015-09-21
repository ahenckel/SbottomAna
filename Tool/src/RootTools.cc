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
