#!/usr/bin/env python
# encoding: utf-8

# File        : StopPlot.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 23
#
# Description : A main code for making plots for the stop analysis


# class StopPlot():
    # def __init__(self):
        # pass


from rootpy.interactive import wait

from PyAna import PyAna
from PyDraw import PyDraw

Lumi = 10 * 1000

if __name__ == "__main__":
    test = PyAna("./Files/", Lumi)
    Draw = PyDraw(Lumi)

    test.FormProcesses()

    out = Draw.DrawLineComparison(test.GetHist([], "Top_G3Top20", "CutFlow"))
    # out = Draw.DrawLineComparison(test.GetHist("TTbar", "Top_G3Top20", "MissTopTagPT"))
    # out = Draw.DrawLineComparison(test.GetHist("Top_G3Top20", "MissTopTagEta"))
    out.SaveAs("test.root")
    out.SaveAs("test.png")
    wait()

