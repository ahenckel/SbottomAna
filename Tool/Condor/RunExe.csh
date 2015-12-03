#!/bin/csh -v

set CMSSW = DELDIR
set EXE   = DELEXE
set OUTPUT = OUTDIR

#============================================================================#
#-----------------------------   Setup the env   ----------------------------#
#============================================================================#
echo "============  Running on" $HOST " ============"
cd ${CMSSW}/src
eval `scramv1 runtime -csh`
cd ${_CONDOR_SCRATCH_DIR}

foreach tarfile (`ls *gz FileList/*gz`)
  echo $tarfile
  tar -xzf $tarfile 
end

#============================================================================#
#--------------------------   To Run the Process   --------------------------#
#============================================================================#
echo $EXE $argv
./$EXE $argv

if ($? == 0) then
  foreach outfile (`ls *root`)
    echo "Copying ${outfile} to ${OUTPUT}"
    xrdcp $outfile "root://cmseos.fnal.gov/${OUTPUT}"
    if ($? == 0) then
      rm $outfile
    endif
  end
endif
