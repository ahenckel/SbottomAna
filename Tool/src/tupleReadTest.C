
















#include "NTupleReader.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>

#include <boost/algorithm/string.hpp>

// UserTools
//#include "RootTools.h"
#include "NTupleReader.h"
#include "HistTool.hh"


#include "baselineDef.h"
namespace joeFunctions
{
    void calcJoe(NTupleReader& tr)
    {
        double retval = tr.getVar<double>("met") + tr.getVar<double>("mht");

        int five = 5;

        std::vector<double> *vec = new std::vector<double>(4, tr.getVar<double>("mht"));

        tr.registerDerivedVar("joe", retval);
        tr.registerDerivedVar("five", five);
        tr.registerDerivedVec("threeNum", vec);
    }
}

bool FillChain(TChain *chain, const TString &inputFileList);

int main()
{
    //char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";
    //char nBase[] = "/eos/uscms/store/user/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";
    char nBase[] = "/Users/benwu/Data/Dataset/PHY14_Ntuple/PHYS14_720_Mar14_2014_v2//DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150328_003328/0000/stopFlatNtuples_%d.root";

    //char nBase[] = "/eos/uscms/store/user/lpcsusyhad/PHYS14_720_Mar14_2014_v2/pastika/DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/150328_003540/0000/stopFlatNtuples_%d.root";
    //char nBase[] = "root://cmsxrootd-site.fnal.gov//store/user/lpcsusyhad/PHYS14_720_Dec23_2014/pastika/DYJetsToLL_M-50_13TeV-madgraph-pythia8/PHYS14_PU20bx25_PHYS14_25_V1-FLAT/141227_223539/0000/stopFlatNtuples_%d.root";

    TChain *f = new TChain("stopTreeMaker/AUX");

    size_t t0 = clock();
  //----------------------------------------------------------------------------
  //  Input and output files
  //----------------------------------------------------------------------------
    const char *inputFileList = "FileList/DYJetsToLL_HT_600toInf.list";
    std::fstream input(inputFileList);
    if(!input.is_open())
    {
      std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
      return false;
    }
    for(std::string line; getline(input, line);)
    {
      if (line[0] == '#') continue;
      boost::algorithm::trim_right(line);
      std::cout << "Add File: " << line << std::endl;
      f->Add(line.c_str());
    }
    std::cout << "No. of Entries in this tree : " << f->GetEntries() << std::endl;
  //if(!FillChain(f, inputFileList))
  //{
    //std::cerr << "Cannot get the tree " << std::endl;
  //}


  ////std::cout << " XS " << GetXS(inputFileList) << std::endl;
    //char fname[512];
    //for(int i = 1; i <= 5; ++i)
    //{
        //sprintf(fname, nBase, i);
        //std::cout << " adding files " << fname << std::endl;
        //f->Add(fname);
    //}
    //std::cout << " files " << f->GetNtrees() << std::endl;
    
    NTupleReader tr(f);
    tr.registerFunction(&joeFunctions::calcJoe);
    AnaFunctions::prepareTopTagger();
    tr.registerFunction(&passBaselineFunc);

    std::cout << "NEVTS: " << tr.getNEntries() << std::endl;

    while(tr.getNextEvent())
    {
        if(tr.getEvtNum() == 1)
        {
            tr.printTupleMembers();
            FILE * fout = fopen("NTupleTypes.txt", "w");
            tr.printTupleMembers(fout);
            fclose(fout);
        }
        //if(tr.getEvtNum()%100000 == 0) std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;
        //std::cout << tr.getVar<double>("met") << "\t" << tr.getVar<double>("mht") << "\t" << tr.getVar<double>("joe") << "\t" << tr.getVar<int>("five") << "\t" << tr.getVec<double>("muonsMtw").size() << "\t" << tr.getVec<double>("threeNum")[2] << std::endl;

    if(tr.getEvtNum()%20000 == 0)
      std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;
    if(tr.getEvtNum() == 60000 )
      break;
    }

    //const unsigned int& run = tr.getVar<unsigned int>("run");

  return 0;
}


//Fill chain from txt file
bool FillChain(TChain *chain, const TString &inputFileList)
{

  return true;
}

