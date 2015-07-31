Python Plot Tool
================

This an attempt to use Python for draw plots from ROOT

Dependant: PyRoot, rootpy

List of files
~~~~~~~~~~~~~

-  *tdrstyle.py* Official CMS TDR style
-  *CMS\_lumi.py* Official CMS Lumi setting
-  *PyHist.py* A simple class to get histogram from root file
-  *PyProcess.py* A class to add the process from different HT bin
   samples
-  *PyAna.py* A class for common handling of processes for analysis
-  *PyDraw.py* A class that handles the drawing process
-  *PyTable.py* A class to dump info into different table format
-  *PyTest.py* A class to dump the data card for Higgs combine tool at
   CMS
-  *StopPlot.py* Intend to be a dirty file for different drawing

Customization
~~~~~~~~~~~~~

Most of the customization can be set in the Config.py HIST\_Proper:
Setting attribute of the histogram, which will become the propetry of
the hist DIR\_Proper: Setting the attribute for histogram under certain
dir Prodmap: Define different process

All the attribute will be save to the histogram and propogate to the
PyDraw.py. The PyDraw calling function can take in argument as well. The
planned priority is PyDraw > Prodmap > HIST > DIR. (To be implemented.)

At the moment, only all common attribute of the input histograms will be
save to the PyDraw and applied during the plot making.

Label
~~~~~

*Label name is the key to control the package. They should be consistent
between Config.py and input arguments.*

+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
| Label       | Describtion                                                                     | Owner   | Default     | Applied Function         |
+=============+=================================================================================+=========+=============+==========================+
| RebinX      | rebin the x axis of the histogram, and updatet the y-axis label if applicable   | Owner   | 1           | PyHist.SetHistProperty   |
+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
| Logy        | Set canvas yaxis on log scale (Need to find a void to avoid 0)                  | Owner   | False       | TBI                      |
+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
| Logx        | Set canvas xaxis on log scale (Need to find a void to avoid 0)                  | Owner   | False       | TBI                      |
+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
| YMax        | Setting the maximum of the y-axis, always use the largest input                 | Owner   | None        | TBI                      |
+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
| YMaxScale   | Scale of the Maximum of the histogram                                           | Owner   | 1.2         | TBI                      |
+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
| HistLabel   | Legend Label corresponding to histname in file                                  | Owner   | histname    | PyDraw                   |
+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
| DirLabel    | Legend Label corresponding to direcotry in file                                 | Owner   | directory   | PyDraw                   |
+-------------+---------------------------------------------------------------------------------+---------+-------------+--------------------------+
