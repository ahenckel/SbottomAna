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

#ifndef  __ROOTTOOLS_INC__
#define  __ROOTTOOLS_INC__

#include <fstream>
#include <iostream>

// boost
#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"

// ROOT
#include "TLorentzVector.h"

// User
#include "TopTaggerAna.h"

//SusyAnatool
#include "SusyAnaTools/Tools/samples.h"
#include "SusyAnaTools/Tools/NTupleReader.h"

double GetXS(std::string name);
int GetCutBin( std::vector<std::string> &CutOrder , std::string name);
double CalMT(TLorentzVector J1, TLorentzVector J2);
double CalMCT(TLorentzVector J1, TLorentzVector J2);
bool PrintEvent(NTupleReader *tr);;
std::vector<TLorentzVector> GetGenParticles(const std::vector<int> pdgid, const NTupleReader *tr);
bool PrintTLorentz(int event, std::string name, std::vector<TLorentzVector> obj);
bool PrintTopEvent( NTupleReader &tr, std::vector<TopDecay> &vTops);

bool LTVMatch(TLorentzVector &V1, TLorentzVector V2, double dR=0.3);

//template<typename T, typename V> int FindVectorIdX(const T &TeV, const V &t);
int FindVectorIdX(std::vector<std::string> &TeV, std::string t);
int HasTLV(TLorentzVector &obj, std::vector<TLorentzVector> &TLVs);
#endif   // ----- #ifndef __ROOTTOOLS_INC__  -----

