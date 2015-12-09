if (  $SHORT_HOST =~ nbay04 ) then 
setenv LD_LIBRARY_PATH ./:../../recipeAUX/OxbridgeMT2/.libs/:$LD_LIBRARY_PATH
setenv BOOSTROOT /usr/include/
else if (  $SHORT_HOST =~ macbook ) then 
setenv DYLD_LIBRARY_PATH /Users/benwu/Work/Stop/SUSY2015/src/SbottomAna/Tool/Plot/theta/lib/libtheta.dylib:./:/Users/benwu/Work/Stop/SUSY2015/src/recipeAUX/OxbridgeMT2/.libs/:$DYLD_LIBRARY_PATH
setenv BOOSTROOT /Users/benwu/BenSys/package/boost_1_57_0/
cd ../../recipeAUX/
source LHAPDF.csh
cd -
endif

setenv MAKEFLAGS "-j 4"
