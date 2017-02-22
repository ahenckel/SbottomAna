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

#include "HistTool.hh"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HistTool
//      Method:  HistTool
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HistTool::HistTool (std::shared_ptr<TFile> OutFile_, std::string name, std::string cut_):
 cutflag(cut_), prefix(name), OutFile(OutFile_)
{ 
  OutFile->cd();
  if (cutflag != "")
  {
    std::stringstream ss;
    if (prefix != "")
      ss << prefix <<"_"<< cutflag;
    else
      ss << cutflag;
    OutFile->mkdir(ss.str().c_str());
    cutflag = ss.str();
  }
  HWeight = -999.;
  CutSize = 0;
  SaveCutHists_=true;
  SaveStatHists_=true;
}  // ~~~~~  end of method HistTool::HistTool  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HistTool
//      Method:  ~HistTool
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HistTool::~HistTool ()
{
  std::cout << "File name " << OutFile->GetName() << std::endl;
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
//         Name:  HistTool::CreateFile
//  Description:  Create the output file for TH1 and TH2D
// ===========================================================================
int HistTool::CreateFile(std::string filname)
{
  (void)filname;
  //OutFile = new TFile(filname.c_str(), "RECREATE");
  return 1;
}       // -----  end of function HistTool::CreateFile  -----

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
    CutSize = static_cast<int>(order.size());
    return order;
}       // -----  end of function HistTool::Cutorder  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::Cutorder
//  Description:  
// ===========================================================================
int HistTool::Cutorder(std::string ana, std::vector<std::string>& CutOrder, unsigned int Nbits)
{
  order = CutOrder;
  CutSize = static_cast<int>(order.size());

  // Initial the cutflow
  TString title = ana == "DM" ? "SUSY VBF DM" : ana;

  TH1 *temp = AddTH1("CutFlow", title.Data(), static_cast<int>(CutOrder.size()), 0 , CutOrder.size());
  for (unsigned int i = 0; i < CutOrder.size(); ++i)
    temp->GetXaxis()->SetBinLabel(i+1, CutOrder.at(i).c_str());

  BookCutBits(Nbits);
  return CutSize;
}       // -----  end of function HistTool::Cutorder  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1
//  Description:  
// ===========================================================================
TH1F* HistTool::AddTH1(const std::string& name_, const std::string& title, std::vector<std::string>& BinLabels)
{
  std::string name = cutflag + "_" + name_;
  TH1F *temp = AddTH1(name, title, BinLabels.size(), 0, BinLabels.size());
  for (unsigned int i = 0; i < BinLabels.size(); ++i)
    temp->GetXaxis()->SetBinLabel(i+1, BinLabels.at(i).c_str());
  return temp;
}       // -----  end of function HistTool::AddTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1C
//  Description:  /* cursor */
// ===========================================================================
int HistTool::AddTH1C(const std::string& name, const std::string& title, std::vector<std::string>& BinLabels) 
{
  if (SaveCutHists_)
  {
    for (Long_t i = 0; i < CutSize; ++i)
    {
      TString mapname = name+"_"+i;
      TString maptitle = title+" ("+order.at(i)+")";
      AddTH1(mapname.Data(), maptitle.Data(), BinLabels);
    }
    if (SaveStatHists_)
    {
      for (Long_t i = 0; i < CutSize; ++i)
      {
        TString mapname = name+"_Stat_"+i;
        TString maptitle = title+" ("+order.at(i)+")";
        AddTH1(mapname.Data(), maptitle.Data(), BinLabels);
      }
    }
  }
  AddTH1(name.c_str(), title.c_str(), BinLabels);
  if (SaveStatHists_)
  {
    TString mapname = name+"_Stat";
    AddTH1(mapname.Data(), title.c_str(), BinLabels);
  }
  return 1;
}       // -----  end of function HistTool::AddTH1C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1C
//  Description:  /* cursor */
// ===========================================================================
int HistTool::AddTH1C(const std::string& name, const std::string& title, 
    const std::string& xlabel, const std::string& ylabel,
    std::vector<std::string>& BinLabels) 
{
  if (SaveCutHists_)
  {
    for (Long_t i = 0; i < CutSize; ++i)
    {
      TString mapname = name+"_"+i;
      TString maptitle = title+" ("+order.at(i)+");"+xlabel+";"+ylabel;
      AddTH1(mapname.Data(), maptitle.Data(), BinLabels);
    }
    if (SaveStatHists_)
    {
      for (Long_t i = 0; i < CutSize; ++i)
      {
        TString mapname = name+"_Stat_"+i;
        TString maptitle = title+" ("+order.at(i)+");"+xlabel+";"+ylabel;
        AddTH1(mapname.Data(), maptitle.Data(), BinLabels);
      }
    }
  }
  TString newtitle = title+";"+xlabel+";"+ylabel;
  AddTH1(name.c_str(), title.c_str(), BinLabels);
  if (SaveStatHists_)
  {
    TString mapname = name+"_Stat";
    AddTH1(mapname.Data(), title.c_str(), BinLabels);
  }
  return 1;
}       // -----  end of function HistTool::AddTH1C  -----


// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1C
//  Description:  
// ===========================================================================
int HistTool::AddTH1C (const std::string& name, const std::string& title,
    const std::string& xlabel, const std::string& ylabel,
    const Int_t& nxbins, const Axis_t& xmin, const Axis_t& xmax,
    const Int_t& logx, const Int_t& logy)
{
  if (SaveCutHists_)
  {
    for (Long_t i = 0; i < CutSize; ++i)
    {
      TString mapname = name+"_"+i;
      TString maptitle = title+" ("+order.at(i)+")";
      AddTH1(mapname.Data(), maptitle.Data(), xlabel, ylabel, nxbins, xmin, xmax, logx, logy);
    }
    if (SaveStatHists_)
    {
      for (Long_t i = 0; i < CutSize; ++i)
      {
        TString mapname = name+"_Stat_"+i;
        TString maptitle = title+" ("+order.at(i)+")";
      AddTH1(mapname.Data(), maptitle.Data(), xlabel, ylabel, nxbins, xmin, xmax, logx, logy);
      }
      
    }
  }
  AddTH1(name.c_str(), title.c_str(), xlabel, ylabel, nxbins, xmin, xmax, logx, logy);
  if (SaveStatHists_)
  {
    TString mapname = name+"_Stat";
    AddTH1(mapname.Data(), title.c_str(), xlabel, ylabel, nxbins, xmin, xmax, logx, logy);
  }

  return 1;
}       // -----  end of function HistTool::AddTH1C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1C
//  Description:  
// ===========================================================================
int HistTool::AddTH1C (const std::string& name, const std::string& title,
    const Int_t& nxbins, const Axis_t& xmin, const Axis_t& xmax)
{
  if (SaveCutHists_)
  {
    for (Long_t i = 0; i < CutSize; ++i)
    {
      TString mapname = name+"_"+i;
      TString maptitle = title+" ("+order.at(i)+")";
      AddTH1(mapname.Data(), maptitle.Data(), nxbins, xmin, xmax);
    }
    if (SaveStatHists_)
    {
      for (Long_t i = 0; i < CutSize; ++i)
      {
        TString mapname = name+"_Stat_"+i;
        TString maptitle = title+" ("+order.at(i)+")";
        AddTH1(mapname.Data(), maptitle.Data(), nxbins, xmin, xmax);
      }
      
    }
  }
  AddTH1(name.c_str(), title.c_str(), nxbins, xmin, xmax);

  if (SaveStatHists_)
  {
    TString mapname = name+"_Stat";
    AddTH1(mapname.Data(), title.c_str(), nxbins, xmin, xmax);
  }
  return 1;
}       // -----  end of function HistTool::AddTH1C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1
//  Description:  degenrous! What if the external th gets destroy? Need to
//  figure out a better way for this!
// ===========================================================================
TH1F* HistTool::AddTH1(TH1F* th)
{
  HisMap[th->GetName()] = std::unique_ptr<TH1F>(th);
  return HisMap[th->GetName()].get();
}       // -----  end of function HistTool::AddTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1
//  Description:  
// ===========================================================================
TH1F* HistTool::AddTH1 (const std::string& name_, const std::string& title,
    const std::string& xlabel, const std::string& ylabel,
    const Int_t& nxbins, const Axis_t& xmin, const Axis_t& xmax,
    const Int_t& logx, const Int_t& logy)
{
  std::string name = cutflag + "_" + name_;
  TString xlb, ylb;
  if (logx) xlb = "log_"+xlabel;
  else xlb = xlabel;
  if (logy) ylb = "log_"+ylabel;
  else ylb = ylabel;
  TString Title = title +";"+xlb+";"+ylb;
  HisMap[name.c_str()] = std::unique_ptr<TH1F>(new TH1F(name.c_str(), Title, nxbins, xmin, xmax));
  return HisMap[name.c_str()].get();
}       // -----  end of function HistTool::AddTH1C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH1
//  Description:  
// ===========================================================================
TH1F* HistTool::AddTH1 (const std::string& name_, const std::string& title,
    const Int_t& nxbins, const Axis_t& xmin, const Axis_t& xmax)
{
  std::string name = cutflag + "_" + name_;
  HisMap[name.c_str()] = std::unique_ptr<TH1F>(new TH1F(name.c_str(), title.c_str(), nxbins, xmin, xmax));
  return HisMap[name.c_str()].get();
}       // -----  end of function HistTool::AddTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::::FillTH1
//  Description:  
// ===========================================================================
int HistTool::FillTH1(int Ncut, std::string HisName_, double value, double weight)
{
  if (Ncut+1 == CutSize) FillTH1(HisName_, value, weight);
  std::string HisName = cutflag + "_" + HisName_;
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;

  double tempweight = 1;
  if (weight != -999.)
    tempweight = weight;
  else if (HWeight != -999.)
    tempweight = HWeight;
  HisMap[mapname.Data()]->Fill(value, tempweight);

  // Filling stat error
  mapname = HisName+"_Stat_"+static_cast<Long_t>(Ncut);
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  HisMap[mapname.Data()]->Fill(value, tempweight >= 0 ? 1 : -1);
  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

int HistTool::FillTH1(int Ncut, std::string HisName_, int value, double weight)
{
  if (Ncut + 1 == CutSize) 
    FillTH1(HisName_, value, weight);

  std::string HisName = cutflag + "_" + HisName_;
  TString mapname = HisName+"_"+static_cast<Long_t>(Ncut);
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  double tempweight = 1;
  if (weight != -999.)
    tempweight = weight;
  else if (HWeight != -999.)
    tempweight = HWeight;
  HisMap[mapname.Data()]->Fill(value, tempweight);

  // Filling stat error
  mapname = HisName+"_Stat_"+static_cast<Long_t>(Ncut);
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  HisMap[mapname.Data()]->Fill(value, tempweight >= 0 ? 1 : -1);

  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

int HistTool::FillTH1(std::string HisName_, int value, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
  TString mapname = HisName;
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  double tempweight = 1;
  if (weight != -999.)
    tempweight = weight;
  else if (HWeight != -999.)
    tempweight = HWeight;
  HisMap[mapname.Data()]->Fill(value, tempweight);

  // Filling stat error
  mapname = HisName+"_Stat";
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  HisMap[mapname.Data()]->Fill(value, tempweight >= 0 ? 1 : -1);

  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

int HistTool::FillTH1(std::string HisName_, double value, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
  if (HisMap.find(HisName) == HisMap.end())
    return 0;
  double tempweight = 1;
  if (weight != -999.)
    tempweight = weight;
  else if (HWeight != -999.)
    tempweight = HWeight;
  HisMap[HisName]->Fill(value, tempweight);

  // Filling stat error
  TString mapname = HisName+"_Stat";
  if (HisMap.find(mapname.Data()) == HisMap.end())
    return 0;
  HisMap[mapname.Data()]->Fill(value, tempweight >= 0 ? 1 : -1);

  return 1;
}       // -----  end of function HistTool::::FillTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::WriteTH1
//  Description:  Save the TH1
// ===========================================================================
int HistTool::WriteTH1()
{
  OutFile->cd();
  if (cutflag != "")
    OutFile->cd(cutflag.c_str());
  for(std::map<std::string, std::unique_ptr<TH1F> >::iterator it=HisMap.begin();
    it!=HisMap.end(); ++it)
  {
    std::string newname = it->second->GetName();
    newname.erase(0, cutflag.size()+1);
    it->second->Write(newname.c_str());
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
  for(std::map<std::string, std::unique_ptr<TH1F> >::iterator it=HisMap.begin();
    it!=HisMap.end(); ++it)
  {
    c1->cd();
    c1->Clear();
    it->second->Draw();
    TString picname = prefix  + "_"+ cutflag + "_" + it->second->GetName() + ".png";
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
TProfile* HistTool::AddTPro (const std::string name_, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t logx, Int_t logy)
{
  std::string name = cutflag + "_" + name_;
  TString xlb, ylb;
  if (logx) xlb = "log_"+xlabel;
  else xlb = xlabel;
  if (logy) ylb = "log_"+ylabel;
  else ylb = ylabel;
  TString Title = title +";"+xlb+";"+ylb;
  ProMap[name.c_str()] = std::unique_ptr<TProfile>(new TProfile(name.c_str(), Title, nxbins, xmin, xmax, "s"));
  return ProMap[name.c_str()].get();
}       // -----  end of function HistTool::AddTProC  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTPro
//  Description:  
// ===========================================================================
TProfile* HistTool::AddTPro(TProfile* pro)
{
  ProMap[pro->GetName()] = std::unique_ptr<TProfile>(pro);
  return ProMap[pro->GetName()].get();
}       // -----  end of function HistTool::AddTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTPro
//  Description:  
// ===========================================================================
TProfile* HistTool::AddTPro (const std::string name_, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax)
{
  
  std::string name = cutflag + "_" + name_;
  ProMap[name.c_str()] = std::unique_ptr<TProfile>(new TProfile(name.c_str(), title.c_str(), nxbins, xmin, xmax, "s"));
  return ProMap[name.c_str()].get();
}       // -----  end of function HistTool::AddTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTPro
//  Description:  
// ===========================================================================
TProfile* HistTool::AddTPro(const std::string& name, const std::string& title, std::vector<std::string>& BinLabels)
{
  TProfile *temp = AddTPro(name, title, BinLabels.size(), 0, BinLabels.size());
  for (unsigned int i = 0; i < BinLabels.size(); ++i)
    temp->GetXaxis()->SetBinLabel(i+1, BinLabels.at(i).c_str());
  return temp;
}       // -----  end of function HistTool::AddTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::::FillTPro
//  Description:  
// ===========================================================================
int HistTool::FillTPro(int Ncut, std::string HisName_, double xvalue, double yvalue, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
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

int HistTool::FillTPro(int Ncut, std::string HisName_, int xvalue, double yvalue,  double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
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

int HistTool::FillTPro(std::string HisName_, double xvalue, double yvalue, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
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

int HistTool::FillTPro(std::string HisName_, int xvalue, double yvalue, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
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
  if (cutflag != "")
    OutFile->cd(cutflag.c_str());
  for(std::map<std::string, std::unique_ptr<TProfile> >::iterator it=ProMap.begin();
    it!=ProMap.end(); ++it)
  {
    std::string newname = it->second->GetName();
    newname.erase(0, cutflag.size()+1);
    it->second->Write(newname.c_str());
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
  for(std::map<std::string, std::unique_ptr<TProfile> >::iterator it=ProMap.begin();
    it!=ProMap.end(); ++it)
  {
    c1->cd();
    c1->Clear();
    it->second->Draw();
    TString picname = prefix + "_"+ cutflag + "_" + it->second->GetName() + ".png";
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
  if (SaveCutHists_)
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
      HisMap2D[mapname.Data()] = std::unique_ptr<TH2D>(new TH2D(mapname.Data(), maptitle.Data(), nxbins,
            xmin, xmax, nybins, ymin, ymax));
    }
  }
  AddTH2(name, title, xlabel, ylabel, nxbins, xmin, xmax, nybins, ymin, ymax, logx, logy);

  return 1;
}       // -----  end of function HistTool::AddTH2C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH2C
//  Description:  
// ===========================================================================
int HistTool::AddTH2C (const std::string name, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax, Int_t nybins, Axis_t ymin, Axis_t ymax)
{
  if (SaveCutHists_)
  {
    for (Long_t i = 0; i < CutSize; ++i)
    {
      TString mapname = name+"_"+i;
      TString maptitle = title+" ("+order.at(i)+")";

      HisMap2D[mapname.Data()] = std::unique_ptr<TH2D>(new TH2D(mapname.Data(), maptitle.Data(), 
            nxbins, xmin, xmax, nybins, ymin, ymax));
    }
  }
  AddTH2(name, title, nxbins, xmin, xmax, nybins, ymin, ymax);

  return 1;
}       // -----  end of function HistTool::AddTH2C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH2
//  Description:  
// ===========================================================================
TH2D* HistTool::AddTH2 (const std::string name_, const std::string title,
    const std::string xlabel, const std::string ylabel,
    Int_t nxbins, Axis_t xmin, Axis_t xmax,
    Int_t nybins, Axis_t ymin, Axis_t ymax,
    Int_t logx, Int_t logy)
{
  std::string name = cutflag + "_" + name_;
  TString xlb, ylb;
  if (logx) xlb = "log_"+xlabel;
  else xlb = xlabel;
  if (logy) ylb = "log_"+ylabel;
  else ylb = ylabel;
  TString Title = title +";"+xlb+";"+ylb;
  HisMap2D[name.c_str()] = std::unique_ptr<TH2D>(new TH2D(name.c_str(), Title.Data(), 
      nxbins, xmin, xmax, nybins, ymin, ymax));
  return HisMap2D[name.c_str()].get();
}       // -----  end of function HistTool::AddTH2C  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::AddTH2
//  Description:  
// ===========================================================================
TH2D* HistTool::AddTH2 (const std::string name_, const std::string title,
    Int_t nxbins, Axis_t xmin, Axis_t xmax, Int_t nybins, Axis_t ymin, Axis_t ymax)
{
  std::string name = cutflag + "_" + name_;
  HisMap2D[name.c_str()] = std::unique_ptr<TH2D>(new TH2D(name.c_str(), title.c_str(), nxbins, 
      xmin, xmax , nybins, ymin, ymax));
  return HisMap2D[name.c_str()].get();
}       // -----  end of function HistTool::AddTH2  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::FillTH2
//  Description:  
// ===========================================================================
int HistTool::FillTH2(int Ncut, std::string HisName_, double xvalue, double yvalue, double weight)
{
  if (Ncut+1 == CutSize) FillTH2(HisName_, xvalue, yvalue, weight);
  std::string HisName = cutflag + "_" + HisName_;
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

int HistTool::FillTH2(int Ncut, std::string HisName_, int xvalue, double yvalue, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
  if (Ncut+1 == CutSize) FillTH2(HisName, xvalue, yvalue, weight);
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

int HistTool::FillTH2(std::string HisName_, int xvalue, double yvalue, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
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

int HistTool::FillTH2(std::string HisName_, double xvalue, double yvalue, double weight)
{
  std::string HisName = cutflag + "_" + HisName_;
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
  if (cutflag != "")
    OutFile->cd(cutflag.c_str());
  for(std::map<std::string, std::unique_ptr<TH2D> >::iterator it=HisMap2D.begin();
    it!=HisMap2D.end(); ++it)
  {
    std::string newname = it->second->GetName();
    newname.erase(0, cutflag.size()+1);
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
  for(std::map<std::string, std::unique_ptr<TH2D> >::iterator it=HisMap2D.begin();
    it!=HisMap2D.end(); ++it)
  {
    c1->cd();
    c1->Clear();
    //it->second->SetMarkerStyle(7);
    //it->second->SetMarkerSize(1.5);
    it->second->Draw();
    TString picname = prefix + "_"+ cutflag + "_" + it->second->GetName() + ".png";
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
//         Name:  HistTool::BookCutBits
//  Description:  
// ===========================================================================
bool HistTool::BookCutBits(const unsigned int& Nbits)
{
  AddTH1("CutBits", "Bits information of the Cut Flow", Nbits+1, 0, Nbits);
  return true;
}       // -----  end of function HistTool::BookCutBits  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::FillCutBits
//  Description:  bitset in cpp is fixed length in compile time. Need to use
//  vector<bool> instead
// ===========================================================================
bool HistTool::FillCutBits(const std::vector<bool>  bits)
{
  //Set the 0 as the total number of event count
  FillTH1("CutBits", -1, 1);

  for (std::size_t i = 0; i < bits.size(); ++i)
    FillTH1("CutBits", static_cast<int>(i), static_cast<int>(bits.at(i)));
  
  return true;
}       // -----  end of function HistTool::FillCutBits  -----


// ===  FUNCTION  ============================================================
//         Name:  HistTool::CalEfficiency
//  Description:  /* cursor */
// ===========================================================================
bool HistTool::CalEfficiency(std::string output, std::string Numerator_, std::string DeNumerator_)
{
  std::string Numerator = cutflag + "_" + Numerator_;
  std::string DeNumerator = cutflag + "_" + DeNumerator_;
  HisMap[output]->Divide(HisMap[Numerator].get(), HisMap[DeNumerator].get());
  return true;
}       // -----  end of function HistTool::CalEfficiency  -----

// ===  FUNCTION  ============================================================
//         Name:  HistTool::SaveCutHists
//  Description:  
// ===========================================================================
bool HistTool::SaveCutHists(bool choice) 
{
  SaveCutHists_ = choice;
  return SaveCutHists_;
}       // -----  end of function HistTool::SaveCutHists  -----
