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

double GetXS(std::string name);
int GetCutBin( std::vector<std::string> &CutOrder , std::string name);
double CalMT(TLorentzVector J1, TLorentzVector J2);
double CalMCT(TLorentzVector J1, TLorentzVector J2);

#endif   // ----- #ifndef __ROOTTOOLS_INC__  -----

