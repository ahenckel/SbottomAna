// ===========================================================================
// 
//       Filename:  Type3Bhad.h
// 
//    Description:  A complimentary class for b jet overlap with W daughter
// 
//        Version:  1.0
//        Created:  08/13/2015 11:22:22
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================



#include <iostream>
#include <vector>

#include "TLorentzVector.h"

// ===========================================================================
//        Class:  Type3Bhad
//  Description:  
// ===========================================================================
class Type3Bhad
{
  public:

    // ====================  LIFECYCLE     ===============================
    Type3Bhad ();                             // constructor
    Type3Bhad ( const Type3Bhad &other );   // copy constructor
    ~Type3Bhad ();                            // destructor

    // ====================  ACCESSORS     ===============================
    int RunTagger(std::vector<TLorentzVector> & jets, std::vector<double> & bjets );
    bool SetTopWindow(int lowWin_, int highWin_);

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    Type3Bhad& operator = ( const Type3Bhad &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================
    std::vector<TLorentzVector> bhadCans;
    std::vector<TLorentzVector> topCans;
    std::vector<std::pair<int, int> > toppair;

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    int lowWin;
    int highWin;


}; // -----  end of class Type3Bhad  -----

