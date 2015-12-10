// ===========================================================================
// 
//       Filename:  GlobalObj.h
// 
//    Description:  Collection of global ojects, which are better to define
//    within main for speeding
// 
//        Version:  1.0
//        Created:  12/10/2015 10:40:54
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_GLOBALOBJ_INC
#define  MY_GLOBALOBJ_INC

#include <iostream>
#include <sstream>
#include <map>
#include <type_traits>

#include "TFile.h"
#include "TKey.h"
#include "TH2.h"
#include "TSystem.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
// ===========================================================================
//        Class:  ExternObj
//  Description:  
// ===========================================================================
class ExternObj
{
  public:

    // ====================  LIFECYCLE     ===============================
    ExternObj ();                             // constructor
    ExternObj ( const ExternObj &other );   // copy constructor
    ~ExternObj ();                            // destructor

    // ====================  ACCESSORS     ===============================
    TObject* Get(std::string name);

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    ExternObj& operator = ( const ExternObj &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool GetAllObjects();
    bool GetAllFiles();
    std::string SearchFilePath(std::string name) const;

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    std::vector<std::string> FileNames;
    std::map<std::string, TFile*> FileMap;
    std::map<std::string, TObject* > ObjMap;

}; // -----  end of class ExternObj  -----


void GetNbNjReweighting(NTupleReader &tr, std::string spec, TH2* Hist);

// ===  FUNCTION  ============================================================
//         Name:  RegisterDefaultAllSpecs
//  Description:  
// ===========================================================================
  template <typename T>
bool RegisterDefaultAllSpecs(NTupleReader &tr, std::string name, T deVar)
{
  const std::vector<std::string> &members = tr.GetTupleMembers();
  const std::vector<std::string> &specs  = tr.GetTupleSpecs();

  for(auto &s : specs)
  {
    if (std::find(members.begin(), members.end(), name+s) ==  members.end())
    {
      tr.registerDerivedVar(name+s, deVar);
    }
  }

  return true;
}       // -----  end of function RegisterDefaultAllSpecs  -----

#endif   // ----- #ifndef MY_GLOBALOBJ_INC  -----
