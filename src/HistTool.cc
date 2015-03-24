// ===========================================================================
// 
//       Filename:  HistTool.hh
// 
//    Description:  A helpful class to handle the histograms
// 
//        Version:  1.0
//        Created:  05/29/2013 02:19:25 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "UserCode/SuperTopTagger/interface/HistTool.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HistTool
//      Method:  HistTool
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HistTool::HistTool (std::string name)
{ 
  prefix = name;
  HWeight = -999.;
}  // ~~~~~  end of method HistTool::HistTool  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HistTool
//      Method:  HistTool
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HistTool::HistTool ( const HistTool &other )
{
}  // ~~~~~  end of method HistTool::HistTool  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HistTool
//      Method:  ~HistTool
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HistTool::~HistTool ()
{
  OutFile->Close();
  delete OutFile;
}  // ~~~~~  end of method HistTool::~HistTool  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HistTool
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  HistTool&
HistTool::operator = ( const HistTool &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method HistTool::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  HistTool::Cutorder
//  Description:  
// ===========================================================================
std::vector<std::string> HistTool::Cutorder()
{
    order.push_back("noCut");
    order.push_back("CTVBF");
    order.push_back("CTLepV");
    order.push_back("CTMet50");
    order.push_back("CTMjj");
    order.push_back("CTMet200");
    order.push_back("AllCut");
    CutSize = order.size();
    return order;
}       // -----  end of function HistTool::Cutorder  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::Cutorder
//  Description:  
// ===========================================================================
int HistTool::Cutorder(std::vector<std::string> Order)
{
  order = Order;
  CutSize = order.size();
  return CutSize;
}       // -----  end of function HistTool::Cutorder  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1C
//  Description:  
// ===========================================================================
int HistTool::AddTH1C (const std::string name, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t logx, Int_t logy)
{
  for (Long_t i = 0; i < CutSize; ++i)
  {
    TString mapname = name+"_"+i;
    TString maptitle = title+" ("+order.at(i)+")";
    AddTH1(mapname.Data(), maptitle.Data(), xlabel, ylabel, nxbins, xmin, xmax, logx, logy);
  }

  return 1;
}       // -----  end of function HistTool::AddTH1C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1C
//  Description:  
// ===========================================================================
int HistTool::AddTH1C (const std::string name, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax)
{
  for (Long_t i = 0; i < CutSize; ++i)
  {
    TString mapname = name+"_"+i;
    TString maptitle = title+" ("+order.at(i)+")";
    AddTH1(mapname.Data(), maptitle.Data(), nxbins, xmin, xmax);
  }
  return 1;
}       // -----  end of function HistTool::AddTH1C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1
//  Description:  degenrous! What if the external th gets destroy? Need to
//  figure out a better way for this!
// ===========================================================================
TH1D* HistTool::AddTH1(TH1D* th)
{
  HisMap[th->GetName()] = th;
  return HisMap[th->GetName()];
}       // -----  end of function HistTool::AddTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1
//  Description:  
// ===========================================================================
TH1D* HistTool::AddTH1 (const std::string name, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t logx, Int_t logy)
{
  TString xlb, ylb;
  if (logx) xlb = "log_"+xlabel;
  else xlb = xlabel;
  if (logy) ylb = "log_"+ylabel;
  else ylb = ylabel;
  TString Title = title +";"+xlb+";"+ylb;
  HisMap[name.c_str()] = fs->make<TH1D>(name.c_str(), Title, nxbins, xmin, xmax);
  return HisMap[name.c_str()];
}       // -----  end of function HistTool::AddTH1C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1
//  Description:  
// ===========================================================================
TH1D* HistTool::AddTH1 (const std::string name, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax)
{
  HisMap[name.c_str()] = fs->make<TH1D>(name.c_str(), title.c_str(), nxbins, xmin, xmax);
  return HisMap[name.c_str()];
}       // -----  end of function HistTool::AddTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::::FillTH1
//  Description:  
// ===========================================================================
int HistTool::FillTH1(int Ncut, std::string HisName, double value, double weight)
{
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  if (weight != -999.)
    HisMap[mapname.Data()]->Fill(value, weight);
  else if (HWeight != -999.)
    HisMap[mapname.Data()]->Fill(value, HWeight);
  else
    HisMap[mapname.Data()]->Fill(value);
  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

int HistTool::FillTH1(int Ncut, std::string HisName, int value, double weight)
{
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  if (weight != -999.)
    HisMap[mapname.Data()]->Fill(value, weight);
  else if (HWeight != -999.)
    HisMap[mapname.Data()]->Fill(value, HWeight);
  else
    HisMap[mapname.Data()]->Fill(value);
  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

int HistTool::FillTH1(std::string HisName, int value, double weight)
{
  if (HisMap.find(HisName) == HisMap.end())
    return 0;
  if (weight != -999.)
    HisMap[HisName]->Fill(value, weight);
  else if (HWeight != -999.)
    HisMap[HisName]->Fill(value, HWeight);
  else
    HisMap[HisName]->Fill(value);
  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

int HistTool::FillTH1(std::string HisName, double value, double weight)
{
  if (HisMap.find(HisName) == HisMap.end())
    return 0;
  if (weight != -999.)
    HisMap[HisName]->Fill(value, weight);
  else if (HWeight != -999.)
    HisMap[HisName]->Fill(value, HWeight);
  else
    HisMap[HisName]->Fill(value);
  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::WriteTH1
//  Description:  Save the TH1
// ===========================================================================
int HistTool::WriteTH1()
{
  OutFile->cd();
  for(std::map<std::string, TH1D*>::iterator it=HisMap.begin();
    it!=HisMap.end(); it++)
  {
    it->second->Write();
  }
  return 1;
}       // -----  end of function HistTool::WriteTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::DrawTH1
//  Description:  Save the TH1
// ===========================================================================
int HistTool::DrawTH1()
{
  TCanvas *c1 = new TCanvas("TH1", "Canvas for TH1", 600, 500);
  for(std::map<std::string, TH1D*>::iterator it=HisMap.begin();
    it!=HisMap.end(); it++)
  {
    c1->cd();
    c1->Clear();
    it->second->Draw();
    TString picname = prefix + "_" + it->second->GetName() + ".png";
    c1->Print(picname);
  }
  delete c1;
  return 1;
}       // -----  end of function HistTool::DrawTH1  -----


// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTProC
//  Description:  
// ===========================================================================
int HistTool::AddTProC (const std::string name, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t logx, Int_t logy)
{
  for (Long_t i = 0; i < CutSize; ++i)
  {
    TString mapname = name+"_"+i;
    TString maptitle = title+" ("+order.at(i)+")";

    AddTPro(mapname.Data(), maptitle.Data(), xlabel, ylabel, nxbins, xmin, xmax, logx, logy);
  }

  return 1;
}       // -----  end of function HistTool::AddTProC  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTProC
//  Description:  
// ===========================================================================
int HistTool::AddTProC (const std::string name, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax)
{
  for (Long_t i = 0; i < CutSize; ++i)
  {
    TString mapname = name+"_"+i;
    TString maptitle = title+" ("+order.at(i)+")";

    AddTPro(mapname.Data(), maptitle.Data(), nxbins, xmin, xmax);
  }

  return 1;
}       // -----  end of function HistTool::AddTProC  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTPro
//  Description:  
// ===========================================================================
TProfile* HistTool::AddTPro (const std::string name, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t logx, Int_t logy)
{
  TString xlb, ylb;
  if (logx) xlb = "log_"+xlabel;
  else xlb = xlabel;
  if (logy) ylb = "log_"+ylabel;
  else ylb = ylabel;
  TString Title = title +";"+xlb+";"+ylb;
  ProMap[name.c_str()] = fs->make<TProfile>(name.c_str(), Title, nxbins, xmin, xmax, "s");
  return ProMap[name.c_str()];
}       // -----  end of function HistTool::AddTProC  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTPro
//  Description:  
// ===========================================================================
TProfile* HistTool::AddTPro(TProfile* pro)
{
  ProMap[pro->GetName()] = pro;
  return ProMap[pro->GetName()];
}       // -----  end of function HistTool::AddTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTPro
//  Description:  
// ===========================================================================
TProfile* HistTool::AddTPro (const std::string name, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax)
{
  
  ProMap[name.c_str()] = fs->make<TProfile>(name.c_str(), title.c_str(), nxbins, xmin, xmax, "s");
  return ProMap[name.c_str()];
}       // -----  end of function HistTool::AddTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::::FillTPro
//  Description:  
// ===========================================================================
int HistTool::FillTPro(int Ncut, std::string HisName, double xvalue, double yvalue, double weight)
{
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (ProMap.find(mapname.Data()) == ProMap.end())
    return 0;
  if (weight != -999.)
    ProMap[mapname.Data()]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    ProMap[mapname.Data()]->Fill(xvalue, yvalue,  HWeight);
  else
    ProMap[mapname.Data()]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTPro  -----

int HistTool::FillTPro(int Ncut, std::string HisName, int xvalue, double yvalue,  double weight)
{
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (ProMap.find(mapname.Data()) == ProMap.end())
    return 0;
  if (weight != -999.)
    ProMap[mapname.Data()]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    ProMap[mapname.Data()]->Fill(xvalue, yvalue,  HWeight);
  else
    ProMap[mapname.Data()]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTPro  -----

int HistTool::FillTPro(std::string HisName, double xvalue, double yvalue, double weight)
{
  if (ProMap.find(HisName) == ProMap.end())
    return 0;
  if (weight != -999.)
    ProMap[HisName]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    ProMap[HisName]->Fill(xvalue, yvalue,  HWeight);
  else
    ProMap[HisName]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTPro  -----

int HistTool::FillTPro(std::string HisName, int xvalue, double yvalue, double weight)
{
  if (ProMap.find(HisName) == ProMap.end())
    return 0;
  if (weight != -999.)
    ProMap[HisName]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    ProMap[HisName]->Fill(xvalue, yvalue,  HWeight);
  else
    ProMap[HisName]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTPro  -----


// ===  FUNCTION  ============================================================
//         Name:  HistTool::WriteTPro
//  Description:  Save the TPro
// ===========================================================================
int HistTool::WriteTPro()
{
  OutFile->cd();
  for(std::map<std::string, TProfile*>::iterator it=ProMap.begin();
    it!=ProMap.end(); it++)
  {
    it->second->Write();
  }
  return 1;
}       // -----  end of function HistTool::WriteTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::DrawTPro
//  Description:  Save the TPro
// ===========================================================================
int HistTool::DrawTPro()
{
  TCanvas *c1 = new TCanvas("TPro", "Canvas for TPro", 600, 500);
  for(std::map<std::string, TProfile*>::iterator it=ProMap.begin();
    it!=ProMap.end(); it++)
  {
    c1->cd();
    c1->Clear();
    it->second->Draw();
    TString picname = prefix + "_" + it->second->GetName() + ".png";
    c1->Print(picname);
  }
  delete c1;
  return 1;
}       // -----  end of function HistTool::DrawTPro  -----


// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH2C
//  Description:  
// ===========================================================================
int HistTool::AddTH2C (const std::string name, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t nybins, Axis_t ymin, Axis_t ymax,
    Int_t logx, Int_t logy)
{
  for (Long_t i = 0; i < CutSize; ++i)
  {
    TString mapname = name+"_"+i;
    TString xlb, ylb;
    if (logx) xlb = "log_"+xlabel;
    else xlb = xlabel;
    if (logy) ylb = "log_"+ylabel;
    else ylb = ylabel;
    TString maptitle = title+" ("+order.at(i)+")" + ";" + xlb + ";" + ylb;
    HisMap2D[mapname.Data()] = fs->make<TH2D>(mapname.Data(), maptitle.Data(), nxbins,
        xmin, xmax, nybins, ymin, ymax);
  }

  return 1;
}       // -----  end of function HistTool::AddTH2C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH2C
//  Description:  
// ===========================================================================
int HistTool::AddTH2C (const std::string name, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax, Int_t nybins, Axis_t ymin, Axis_t ymax)
{
  for (Long_t i = 0; i < CutSize; ++i)
  {
    TString mapname = name+"_"+i;
    TString maptitle = title+" ("+order.at(i)+")";

    HisMap2D[mapname.Data()] = fs->make<TH2D>(mapname.Data(), maptitle.Data(), 
        nxbins, xmin, xmax, nybins, ymin, ymax);
    //HisMap2D[mapname.Data()] = result.AddHist1D(mapname.Data(), 
        //maptitle.Data(), xlabel.c_str(), ylabel.c_str(), nxbins, xmin, xmax);
  }

  return 1;
}       // -----  end of function HistTool::AddTH2C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH2
//  Description:  
// ===========================================================================
TH2D* HistTool::AddTH2 (const std::string name, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t nybins, Axis_t ymin, Axis_t ymax,
    Int_t logx, Int_t logy)
{

  TString xlb, ylb;
  if (logx) xlb = "log_"+xlabel;
  else xlb = xlabel;
  if (logy) ylb = "log_"+ylabel;
  else ylb = ylabel;
  TString Title = title +";"+xlb+";"+ylb;
  HisMap2D[name.c_str()] = fs->make<TH2D>(name.c_str(), Title.Data(), 
      nxbins, xmin, xmax, nybins, ymin, ymax);
  return HisMap2D[name.c_str()];
}       // -----  end of function HistTool::AddTH2C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH2
//  Description:  
// ===========================================================================
TH2D* HistTool::AddTH2 (const std::string name, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax, Int_t nybins, Axis_t ymin, Axis_t ymax)
{
  
  HisMap2D[name.c_str()] = fs->make<TH2D>(name.c_str(), title.c_str(), nxbins, 
      xmin, xmax , nybins, ymin, ymax);
  return HisMap2D[name.c_str()];
}       // -----  end of function HistTool::AddTH2  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::FillTH2
//  Description:  
// ===========================================================================
int HistTool::FillTH2(int Ncut, std::string HisName, double xvalue, double yvalue, double weight)
{
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (HisMap2D.find(mapname.Data()) == HisMap2D.end())
    return 0;
  if (weight != -999.)
    HisMap2D[mapname.Data()]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    HisMap2D[mapname.Data()]->Fill(xvalue, yvalue,  HWeight);
  else
    HisMap2D[mapname.Data()]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTH2  -----

int HistTool::FillTH2(int Ncut, std::string HisName, int xvalue, double yvalue, double weight)
{
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (HisMap2D.find(mapname.Data()) == HisMap2D.end())
    return 0;
  if (weight != -999.)
    HisMap2D[mapname.Data()]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    HisMap2D[mapname.Data()]->Fill(xvalue, yvalue,  HWeight);
  else
    HisMap2D[mapname.Data()]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTH2  -----

int HistTool::FillTH2(std::string HisName, int xvalue, double yvalue, double weight)
{
  if (HisMap2D.find(HisName) == HisMap2D.end())
    return 0;
  if (weight != -999.)
    HisMap2D[HisName]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    HisMap2D[HisName]->Fill(xvalue, yvalue,  HWeight);
  else
    HisMap2D[HisName]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTH2  -----

int HistTool::FillTH2(std::string HisName, double xvalue, double yvalue, double weight)
{
  if (HisMap2D.find(HisName) == HisMap2D.end())
    return 0;
  if (weight != -999.)
    HisMap2D[HisName]->Fill(xvalue, yvalue,  weight);
  else if (HWeight != -999.)
    HisMap2D[HisName]->Fill(xvalue, yvalue,  HWeight);
  else
    HisMap2D[HisName]->Fill(xvalue, yvalue);
  return 1;
}       // -----  end of function HistTool::::FillTH2  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::WriteTH2
//  Description:  Save the TH2
// ===========================================================================
int HistTool::WriteTH2()
{
  OutFile->cd();
  for(std::map<std::string, TH2D*>::iterator it=HisMap2D.begin();
    it!=HisMap2D.end(); it++)
  {
    it->second->Write();
  }
  return 1;
}       // -----  end of function HistTool::WriteTH2  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::DrawTH2
//  Description:  Save the TH2
// ===========================================================================
int HistTool::DrawTH2()
{
  TCanvas *c1 = new TCanvas("TH2", "Canvas for TH2", 600, 500);
  for(std::map<std::string, TH2D*>::iterator it=HisMap2D.begin();
    it!=HisMap2D.end(); it++)
  {
    c1->cd();
    c1->Clear();
    //it->second->SetMarkerStyle(7);
    //it->second->SetMarkerSize(1.5);
    it->second->Draw();
    TString picname = prefix + "_" + it->second->GetName() + ".png";
    c1->Print(picname);
  }
  delete c1;
  return 1;
}       // -----  end of function HistTool::DrawTH2  -----


// ===  FUNCTION  ============================================================
//         Name:  HistTool::SetWeight
//  Description:  
// ===========================================================================
bool HistTool::SetWeight(double weight)
{
  HWeight = weight;
  return true;
}       // -----  end of function HistTool::SetWeight  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::CalEfficiency
//  Description:  /* cursor */
// ===========================================================================
bool HistTool::CalEfficiency(std::string output, std::string Numerator, std::string DeNumerator)
{
  HisMap[output]->Divide(HisMap[Numerator], HisMap[DeNumerator]);
  return true;
}       // -----  end of function HistTool::CalEfficiency  -----
