// ===========================================================================
// 
//       Filename:  MatchingTaggerGen.h
// 
//    Description:  A class for studing TopTagger with GenParAna
// 
//        Version:  1.0
//        Created:  02/02/2015 11:37:58 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================


// UserCode 
#include "UserCode/SuperTopTagger/interface/GenParAna.h"
#include "UserCode/SuperTopTagger/interface/HistTool.h"
#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

// ===========================================================================
//        Class:  MatchingTaggerGen
//  Description:  
// ===========================================================================
class MatchingTaggerGen
{
  public:

    // ====================  LIFECYCLE     ===============================
    MatchingTaggerGen (HistTool* hist_, TLorentzVector *metLVec_,    // constructor
        std::vector<TLorentzVector> *oriJetsVec_, std::vector<double> *recoJetsBtagCSVS_);                             
    MatchingTaggerGen ( const MatchingTaggerGen &other );            // copy constructor
    ~MatchingTaggerGen ();                                           // destructor

    // ====================  ACCESSORS     ===============================
    bool BookHistogram() const;
    bool AnaType3WTag(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<WDecay> &Widx);
    bool AnaType3Gen(topTagger::type3TopTagger* type3TopTaggerPtr, GenParAna *AnaPtr);
    int MatchingWDijet(std::vector<TLorentzVector> pp, std::vector<TLorentzVector> jj) const;
    std::vector<TLorentzVector> GetType3WDijet(std::vector<int> T3TopComb) const;
    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================
    bool PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, int ic) const;

    MatchingTaggerGen& operator = ( const MatchingTaggerGen &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    HistTool* hist;
    TLorentzVector *metLVec; 
    std::vector<TLorentzVector> *oriJetsVec; 
    std::vector<double> *recoJetsBtagCSVS;

}; // -----  end of class MatchingTaggerGen  -----

