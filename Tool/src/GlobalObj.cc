// ===========================================================================
// 
//       Filename:  GlobalObj.cc
// 
//    Description:  Collection of global ojects, which are better to define
//    within main for speeding
// 
//        Version:  1.0
//        Created:  12/10/2015 10:40:13
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================


#include "GlobalObj.h"

//----------------------------------------------------------------------------
//       Class:  ExternObj
//      Method:  ExternObj
// Description:  constructor
//----------------------------------------------------------------------------
ExternObj::ExternObj ()
{
  FileNames.push_back("STZinv15.root");

  GetAllFiles();
  GetAllObjects();
}  // -----  end of method ExternObj::ExternObj  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  ExternObj
//      Method:  ExternObj
// Description:  copy constructor
//----------------------------------------------------------------------------
ExternObj::ExternObj ( const ExternObj &other )
{
}  // -----  end of method ExternObj::ExternObj  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  ExternObj
//      Method:  ~ExternObj
// Description:  destructor
//----------------------------------------------------------------------------
ExternObj::~ExternObj ()
{
}  // -----  end of method ExternObj::-ExternObj  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  ExternObj
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  ExternObj&
ExternObj::operator = ( const ExternObj &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method ExternObj::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  ExternObj::GetAllFiles
//  Description:  
// ===========================================================================
bool ExternObj::GetAllFiles()
{
  for(auto &filename : FileNames)
  {
    FileMap[filename] = new TFile(SearchFilePath(filename).c_str(), "READ");
  }
  return true;
}       // -----  end of function ExternObj::GetAllFiles  -----


// ===  FUNCTION  ============================================================
//         Name:  ExternObj::SearchFilePath
//  Description:  
// ===========================================================================
std::string ExternObj::SearchFilePath(std::string name_) const
{
  std::vector<std::string> SearchPath = {"./", "../", "FileList"};
  TString name(name_);
  std::string repath="";

  for(auto &a : SearchPath)
  {
    const char* out = gSystem->Which(a.c_str(), name);
    if (out != 0)
    {
      repath = out;
      delete out;
      break;
    }
    delete out;
  }

  return repath;
}       // -----  end of function ExternObj::SearchFilePath  -----

// ===  FUNCTION  ============================================================
//         Name:  ExternObj::GetAllObjects
//  Description:  
// ===========================================================================
bool ExternObj::GetAllObjects()
{
  for(auto &it : FileMap)
  {
    TIter next(it.second->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
      std::stringstream ss;
      ss << it.first<<":" << key->GetName();
      ObjMap[ss.str()] = key->ReadObj();
    }
  }
  return true;
}       // -----  end of function ExternObj::GetAllObjects  -----

// ===  FUNCTION  ============================================================
//         Name:  ExternObj::Get
//  Description:  /* cursor */
// ===========================================================================
TObject* ExternObj::Get(std::string name)
{
  if (ObjMap.find(name) == ObjMap.end())
  {
    std::cout << "Missing " << name << std::endl;
    throw name;
  }
  return ObjMap[name];
}       // -----  end of function ExternObj::Get  -----


//**************************************************************************//
//                   Function for deriving from GlobalObj                   //
//**************************************************************************//
// ===  FUNCTION  ============================================================
//         Name:  GetNbNjReweighting
//  Description:  
// ===========================================================================
void GetNbNjReweighting(NTupleReader &tr, std::string spec, TH2* Hist)
{

  if (tr.getVar<int>("run") != 1) // not for data!
  {
    tr.registerDerivedVar("NbNjReweight" + spec, 1.0);
    return;
  }

  const int nBjets = tr.getVar<int>("cntCSVS" + spec);
  const int njets = tr.getVar<int>("cntNJetsPt30Eta24" + spec);
  double bjweight = 1.0;

  TAxis* baxis = Hist->GetXaxis();
  TAxis* jaxis = Hist->GetYaxis();
  int bbin = baxis->FindBin(nBjets);
  int jbin = jaxis->FindBin(njets);

  // for lower bound, no reweighting
  if (bbin == 0 || jbin == 0)
  {
    bjweight = 1.0;
  }

  // for upper bound 
  if (bbin > baxis->GetNbins())
  {
    bbin = baxis->GetNbins();
  }
  if (jbin > jaxis->GetNbins())
  {
    jbin = jaxis->GetNbins();
  }

  bjweight = Hist->GetBinContent(bbin, jbin);
  if (bjweight == 0.0) // if no weight in the TH2
  {
    bjweight = 1.0;
  }

  tr.registerDerivedVar("NbNjReweight" + spec, bjweight);

}       // -----  end of function GetNbNjReweighting  -----

