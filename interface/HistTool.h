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

#ifndef  __HISTTOOL__
#define  __HISTTOOL__

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TH2D.h"
#include "TLegend.h"
#include <map>
#include <iostream>



// ===========================================================================
//        Class:  HistTool
//  Description:  A class that should be able to generate TH1D or TH2D for the
//  plots. It automatically generate number of plots for the cutflow.
// ===========================================================================
class HistTool
{
  public:

    // ====================  LIFECYCLE     ===============================
    HistTool (std::string name);
    HistTool ( const HistTool &other );   // copy constructor
    ~HistTool ();                            // destructor

    // ====================  ACCESSORS     ===============================
    std::map<std::string, TH1D*> HisMap; //Saving the 1D Hist 

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    HistTool& operator = ( const HistTool &other ); // assignment operator

    int Cutorder(std::vector<std::string> order);
    std::vector<std::string> Cutorder();
    int CreateFile(std::string filname);
    bool SetWeight(double weight=1.0);

//----------------------------------------------------------------------------
//  TH1
//----------------------------------------------------------------------------
    // Book the His TH1
    TH1D* AddTH1(TH1D* th);
    TH1D* AddTH1 (const std::string name, const std::string title, 
        const std::string xlabel, const std::string ylabel, 
        Int_t nxbins, Axis_t xmin, Axis_t xmax, Int_t logx=0, Int_t logy=0);
    TH1D* AddTH1 (const std::string name, const std::string title, 
        Int_t nxbins, Axis_t xmin, Axis_t xmax);
    int AddTH1C (const std::string name, const std::string title, 
        Int_t nxbins, double xmin, double  xmax);
    int AddTH1C (const std::string name, const std::string title, 
        const std::string xlabel, const std::string ylabel, 
        Int_t nxbins, Axis_t xmin, Axis_t xmax, 
        Int_t logx=0, Int_t logy=0);

    // Filling the his TH1
    int FillTH1(int Ncut, std::string HisName, int value, double weight=-999.);
    int FillTH1(int Ncut, std::string HisName, double value, double weight=-999.);
    int FillTH1(std::string HisName, double value, double weight=-999.);
    int FillTH1(std::string HisName, int value, double weight=-999.);

    // Output the His TH1
    int WriteTH1();
    int DrawTH1();

//----------------------------------------------------------------------------
//  TProfile
//----------------------------------------------------------------------------
    // Book the His TPro
    TProfile* AddTPro(TProfile* pro);
    TProfile* AddTPro (const std::string name, const std::string title, 
        const std::string xlabel, const std::string ylabel, 
        Int_t nxbins, Axis_t xmin, Axis_t xmax, Int_t logx=0, Int_t logy=0);
    TProfile* AddTPro (const std::string name, const std::string title, 
        Int_t nxbins, Axis_t xmin, Axis_t xmax);
    int AddTProC (const std::string name, const std::string title, 
        Int_t nxbins, double xmin, double  xmax);
    int AddTProC (const std::string name, const std::string title, 
        const std::string xlabel, const std::string ylabel, 
        Int_t nxbins, Axis_t xmin, Axis_t xmax, 
        Int_t logx=0, Int_t logy=0);

    // Filling the his TPro
    int FillTPro(int Ncut, std::string HisName, int xvalue, double yvalue, double weight=-999.);
    int FillTPro(int Ncut, std::string HisName, double xvalue, double yvalue, double weight=-999.);
    int FillTPro(std::string HisName, double xvalue, double yvalue, double weight=-999.);
    int FillTPro(std::string HisName, int xvalue, double yvalue, double weight=-999.);

    // Output the His TPro
    int WriteTPro();
    int DrawTPro();

//----------------------------------------------------------------------------
//  TH2D
//----------------------------------------------------------------------------
    
    // Book the His TH2
    int AddTH2C (const std::string name, const std::string title, 
        const std::string xlabel, const std::string ylabel, Int_t nxbins, 
        Axis_t xmin, Axis_t xmax, Int_t nybins, Axis_t ymin, Axis_t ymax, 
        Int_t logx=0, Int_t logy=0);
    int AddTH2C (const std::string name, const std::string title, 
        Int_t nxbins, Axis_t xmin, Axis_t xmax, 
        Int_t nybins, Axis_t ymin, Axis_t ymax);
    TH2D* AddTH2 (const std::string name, const std::string title, 
        const std::string xlabel, const std::string ylabel, Int_t nxbins, 
        Axis_t xmin, Axis_t xmax, Int_t nybins, Axis_t ymin, 
        Axis_t ymax, Int_t logx=0, Int_t logy=0);
    TH2D* AddTH2 (const std::string name, const std::string title, Int_t nxbins, 
        Axis_t xmin, Axis_t xmax, Int_t nybins, Axis_t ymin, Axis_t ymax);

    // Filling the his TH2
    int FillTH2(int Ncut, std::string HisName, double xvalue, double yvalue, double weight=-999.);
    int FillTH2(int Ncut, std::string HisName, int xvalue, double yvalue, double weight=-999.);
    int FillTH2(std::string HisName, int xvalue, double yvalue, double weight=-999.);
    int FillTH2(std::string HisName, double xvalue, double yvalue, double weight=-999.);

    // Output the His TH2
    int WriteTH2();
    int DrawTH2();
    bool CalEfficiency(std::string output, std::string Numerator, std::string DeNumerator);

  protected:
    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  DATA MEMBERS  ===============================
    edm::Service<TFileService> fs;
    int CutSize; // The size of the cutbit
    double HWeight; // The weight of current event
    TFile* OutFile;
    std::string prefix;

    std::vector<std::string> order;
    std::map<std::string, TProfile*> ProMap; //Saving the 1D Hist 
    std::map<std::string, TH2D*> HisMap2D; //Saving the 2D Hist 

}; // -----  end of class HistTool  -----

#endif   // ----- #ifndef __HISTTOOL__  -----
