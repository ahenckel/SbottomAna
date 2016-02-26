// ===========================================================================
// 
//       Filename:  RootTools.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/15/2015 11:46:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#ifndef  MY_ROOTTOOLS_INC
#define  MY_ROOTTOOLS_INC

#include <unistd.h>
#include <ios>
#include <string>
#include <bitset>
#include <fstream>
#include <iostream>
#include <stdexcept>

// boost
#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"

// ROOT
#include "TLorentzVector.h"

// User
#include "TopTaggerAna.h"
#include "VarPerEvent.h"

//SusyAnatool
#include "SusyAnaTools/Tools/samples.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/baselineDef.h"

std::map<std::string, double> GetXS(std::string name);
std::string GetProcName(std::map<std::string, double> &SamplePro);
int GetCutBin( std::vector<std::string> &CutOrder , std::string name);
double CalMT(TLorentzVector J1, TLorentzVector J2);
double CalMCT(TLorentzVector J1, TLorentzVector J2);
bool PrintEvent(NTupleReader *tr);;
std::vector<TLorentzVector> GetGenParticles(const std::vector<int> pdgid, const NTupleReader *tr);
bool PrintTLorentz(int event, std::string name, std::vector<TLorentzVector> obj);
bool PrintTopEvent( NTupleReader &tr, std::vector<TopDecay> &vTops);

void process_mem_usage(double& vm_usage, double& resident_set);
bool LTVMatch(TLorentzVector &V1, TLorentzVector V2, double dR=0.3);

//template<typename T, typename V> int FindVectorIdX(const T &TeV, const V &t);
int FindVectorIdX(std::vector<std::string> &TeV, std::string t);
int HasTLV(TLorentzVector &obj, std::vector<TLorentzVector> &TLVs);

int GetGenChild( std::vector<int> &genDecayPdgIdVec, std::vector<int> &genDecayMomIdxVec , int parent, std::vector<int> pdgs);
bool GetGenTops(NTupleReader &tr, std::vector<TLorentzVector> &vGenTops, std::vector<int> &vGenTopCharge, std::vector<TLorentzVector> &vGenLeps);
//**************************************************************************//
//                            Register Functions                            //
//**************************************************************************//
bool TopWithoutBVeto(NTupleReader &tr, std::string spec);
std::string ChooseLepPath(std::string leps);
void passBaselineZinv(NTupleReader &tr, std::string leps);
void passBaselineTTZ(NTupleReader &tr, std::string leps, int JEC=0);
void passBaselineMHT(NTupleReader &tr);
void passBaselineJECdn(NTupleReader &tr);
void passBaselineJECup(NTupleReader &tr);
std::string GetEventFilterList(std::string dataset);
void GetTopPtReweight(NTupleReader &tr, std::map<std::string, double> &SamplePro);
// ===  FUNCTION  ============================================================
//         Name:  PassEventListFilter
//  Description:  Since EventFilter included both definition and declaration,
//  using template function to avoid double symbol :-(
//  Keeping function here for avoid linker error, according to 
//  http://stackoverflow.com/questions/1639797/template-issue-causes-linker-error-c
// ===========================================================================
  template<typename T>
void PassEventListFilter(NTupleReader &tr, T *filter)
{
  bool beamHaloFilter = true;
  if(filter->Initialized()) 
  {
    const unsigned int& run =   tr.getVar<unsigned int>("run");
    const unsigned int& lumi  = tr.getVar<unsigned int>("lumi");
    const unsigned int& event = tr.getVar<unsigned int>("event");
    beamHaloFilter = filter->CheckEvent(run, lumi, event);
    //if (!beamHaloFilter)
    //{
    //std::cout << " Run " << run <<" lumi " << lumi <<" event " << event <<" filter " << beamHaloFilter << std::endl;
    //}
  }
  tr.registerDerivedVar("CSCbeamHaloFilter", beamHaloFilter);

}       // -----  end of function PassEventListFilter  -----
#endif   // ----- #ifndef MY_ROOTTOOLS_INC  -----
