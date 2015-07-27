#!/usr/bin/env python
# encoding: utf-8

# File        : PyDraw.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 24
#
# Description : A class used for drawing

import ROOT
import rootpy
import CMS_lumi, tdrstyle
import math


PyColors = [
    ROOT.kBlack,
    ROOT.kRed,
    ROOT.kBlue,
    ROOT.kGreen-2,
    ROOT.kOrange,
    ROOT.kCyan,
    ROOT.kMagenta,
    ROOT.kYellow,
    ROOT.kGreen
]


# ### To Do list ########
# 1. Auto scale the y-axis for the CMS label
# 2. Auto set the y-axis offset
# 3. Auto set the TLegend location base on the input hists
# 4. Add ratio plot
# 5. Draw Stack etc...
# #######################
class PyDraw():
    def __init__(self, Lumi):
        self.lumi = Lumi
        H_ref = 600
        W_ref = 800
        W = W_ref
        H = H_ref
        T = 0.08*H_ref
        B = 0.12*H_ref
        L = 0.12*W_ref
        R = 0.04*W_ref
        #set the tdr style
        tdrstyle.setTDRStyle()

        self.canvas = ROOT.TCanvas("c2", "c2", 50, 50, W, H)
        self.canvas.SetFillColor(0)
        self.canvas.SetBorderMode(0)
        self.canvas.SetFrameFillStyle(0)
        self.canvas.SetFrameBorderMode(0)
        self.canvas.SetLeftMargin(L/W)
        self.canvas.SetRightMargin(R/W)
        self.canvas.SetTopMargin(T/H)
        self.canvas.SetBottomMargin(B/H)
        self.canvas.SetTickx(0)
        self.canvas.SetTicky(0)

    def CanvasRedraw(self):
        self.canvas.Update()
        self.canvas.RedrawAxis()
        CMS_lumi.CMS_lumi(self.canvas, 13, 11, self.lumi )

    def DrawLineComparison(self, hists, options ="hist", legloc = "topright"):
        self.canvas.cd()
        self.canvas.Clear()

        leg = self.SetLegend([hist.title for hist in hists], legloc)
        if leg is not None:
            leg.SetBorderSize(0)
            leg.SetFillStyle(0)
            leg.SetTextFont(62)
            leg.SetTextSize(0.045)

        ymax = 0

        localColor = False
        if len(set(hist.GetLineColor() for hist in hists)) == 1:
            localColor = True

        for it in range(0, len(hists)):
            if localColor and it <= len(PyColors):
                hists[it].SetLineColor(PyColors[it])

            if it == 0:
                yaxis = hists[it].GetYaxis()
                hists[it].Draw(options)
            else:
                hists[it].Draw("%s same" % options)

            ymax = max(ymax, hists[it].GetMaximum())
            print hists[it].title
            if leg is not None:
                leg.AddEntry(hists[it], hists[it].title, "l")

        self.SetYaxis(yaxis, [hist.Integral() for hist in hists if hist.Integral() != 0.0])
        yaxis.SetRangeUser(0, 1.2*ymax)

        if leg is not None:
            leg.Draw()

        self.CanvasRedraw()
        return self.canvas

    def SetLegend(self, hislegs, location):
        x1 = 0.0
        y1 = 0.0
        x2 = 1.0
        y2 = 1.0

        dy = 0.05 * len(hislegs)

        dx = 0.05 + 0.018 * max([len(lstr.strip()) for lstr in hislegs])

        if location == "topright":
            x2 = 0.90
            y2 = 0.90
            x1 = x2 - dx
            y1 = y2 - dy

        if dy == 0:
            return None

        return ROOT.TLegend(x1, y1, x2, y2)

    def SetYaxis(self, yaxis, histItgl):
        ## For normailzation
        UnitItgl = [False for itgl in histItgl if math.fabs(itgl - 1)  > 0.01 ]
        orgTitle = yaxis.GetTitle()

        if len(orgTitle) == 0:
            orgTitle = "a.u."
        else:
            if len(UnitItgl) == 0:
                orgTitle += " (Norm. to Unity)"
        yaxis.SetTitle(orgTitle)
