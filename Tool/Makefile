IDIR       = inc
ODIR       = obj
SDIR       = src

ifneq ("$(wildcard .clang_complete)","")
  CXX        = g++
else
  CXX       = ~/.vim/bundle/ClangComplete/bin/cc_args.py g++
endif

CXXFLAGS  += -Iinc -std=c++11
# Optimization flag
#CXXFLAGS += -g -O3 -fstack-protector-all 
## Enable the maximun warning
#CXXFLAGS += -Wall -Wextra -Weffc++ -g -Weverything

## Include ROOT
CXXFLAGS  += $(subst -I, -isystem , $(shell root-config --cflags))
#CXXDEPFLAGS = -MMD -MP

ifneq ($(shell echo $$CMSSW_BASE), )
  CXXFLAGS  += -isystem $(CMSSW_BASE)/src/
  SUSYObj = $(CMSSW_BASE)/src/SusyAnaTools/Tools/obj/
else
  CXXFLAGS  += -isystem ../..
  SUSYObj = ../../SusyAnaTools/Tools/obj/
endif

LD         = g++
LDFLAGS    =

LIBS       = $(shell root-config --glibs)   

ifneq ($(shell echo $$CMSSW_BASE), )
  LIBS += -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2
else
  LIBS += -L../../recipeAUX/OxbridgeMT2/.libs -loxbridgekinetics-1.0 
endif

#============================================================================#
#-----------------------------   Include Boost   ----------------------------#
#============================================================================#
ifneq ($(shell echo $$BOOSTROOT), )
  CXXFLAGS  += -isystem $(shell echo $(BOOSTROOT))
endif

SUSYTools = $(SUSYObj)/NTupleReader.o $(SUSYObj)/samples.o $(SUSYObj)/baselineDef.o $(SUSYObj)/customize.o $(SUSYObj)/searchBins.o

FILES = $(wildcard $(SDIR)/*.cc)
OBJS  := $(FILES:$(SDIR)/%.cc=$(ODIR)/%.o)

PROGRAM = testMain

TOOLS =  $(ODIR)/HistTool.o    $(ODIR)/RootTools.o   

all: $(PROGRAM)

$(ODIR)/%.o : $(SDIR)/%.C
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(IDIR)/%.C
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(IDIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

tupleTest: $(ODIR)/NTupleReader.o  $(ODIR)/tupleReadTest.o
	$(LD) $^ $(LIBS)  -o $@


testMain: $(TOOLS)  $(SUSYTools)  $(OBJS)
	@echo "Linking $(PROGRAM) ..."
	$(LD) $^ $(LIBS)  -o $@
	@echo "done"

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(PROGRAMS) core AutoDict_vector*
