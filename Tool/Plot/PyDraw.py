#!/usr/bin/env python
# encoding: utf-8

# File        : PyDraw.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 24
#
# Description : A class used for drawing

import ROOT
import CMS_lumi
import tdrstyle
import math
from rootpy.plotting import HistStack
from rootpy.plotting import Canvas
from Config import PyColors, PyOutPut

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
        tdrstyle.setTDRStyle()
        self.canvas = self.CreatCanvas()

# ============================================================================#
# ------------------------------     Canvas     ------------------------------#
# ============================================================================#
    def CreatCanvas(self):
        H_ref = 600
        W_ref = 800
        W = W_ref
        H = H_ref
        T = 0.08*H_ref
        B = 0.12*H_ref
        L = 0.12*W_ref
        R = 0.04*W_ref
        # Set the tdr style

        canvas = Canvas(width=W, height=H)
        canvas.SetFillColor(0)
        canvas.SetBorderMode(0)
        canvas.SetFrameFillStyle(0)
        canvas.SetFrameBorderMode(0)
        canvas.SetLeftMargin(L/W)
        canvas.SetRightMargin(R/W)
        canvas.SetTopMargin(T/H)
        canvas.SetBottomMargin(B/H)
        canvas.SetTickx(0)
        canvas.SetTicky(0)
        return canvas

    def CanvasRedraw(self, outname="output", norm="None"):
        self.canvas.Update()
        self.canvas.Modified()
        self.canvas.RedrawAxis()
        if norm == "None" or norm == "Unit":
            CMS_lumi.CMS_lumi(self.canvas, 23, 11, self.lumi)
        else:
            CMS_lumi.CMS_lumi(self.canvas, 13, 11, self.lumi)

        for outformat in PyOutPut:
            self.canvas.SaveAs("%s.%s" % (outname, outformat))

# ============================================================================#
# ------------------------------     Legend     ------------------------------#
# ============================================================================#
    def SetLegend(self, hislegs, location):
        x1 = 0.0
        y1 = 0.0
        x2 = 1.0
        y2 = 1.0

        dy = 0.05 * len(hislegs)

        dx = 0.05 + 0.02 * max([len(lstr.strip()) for lstr in hislegs])

        # x3 --- x2
        #  |      |
        # x1 --- x4

        if location == "topright":
            x2 = 0.98
            y2 = 0.90
            x1 = x2 - dx
            y1 = y2 - dy

        if location == "bottomright":
            x4 = 0.98
            y4 = 0.16
            y2 = y4 + dy
            x2 = x4
            x1 = x4 - dx
            y1 = y4

        if location == "bottomcenter":
            x4 = 0.98
            y4 = 0.16
            y2 = y4 + dy
            x2 = 0.5 + dx / 2
            x1 = 0.5 - dx / 2
            y1 = y4

        if location == "bottomleft":
            x1 = 0.15
            y1 = 0.15
            x2 = x1 + dx
            y2 = y1 + dy

        if dy == 0:
            return None

        return ROOT.TLegend(x1, y1, x2, y2)

    def SetAutoLegend(self, hists):
        loclist = ["topright", "bottomright", "bottomcenter", "bottomleft"]
        locoverlap = {}
        for loc in loclist:
            leg = self.SetLegend([hist.title for hist in hists], loc)
            print "leg.GetBBox().fX, leg.GetBBox().fY,  leg.GetBBox().fWidth, leg.GetBBox().fHeight"
            print leg.GetBBox().fX, leg.GetBBox().fY,  leg.GetBBox().fWidth, leg.GetBBox().fHeight
            locoverlap[loc] = 0
            for hist in hists:
                print hist.title
                for ibin in range(1, hist.GetNbinsX()):
                    print "hist.title, ibin, hist.GetBinCenter(ibin), self.canvas.XtoPixel(hist.GetBinCenter(ibin)), hist.GetBinContent(ibin), self.canvas.YtoPixel(hist.GetBinContent(ibin))"
                    print hist.title, ibin, hist.GetBinCenter(ibin), self.canvas.XtoPixel(hist.GetBinCenter(ibin)), hist.GetBinContent(ibin), self.canvas.YtoPixel(hist.GetBinContent(ibin))
                    xNDC = float(self.canvas.XtoPixel(hist.GetBinCenter(ibin)))/self.canvas.GetWindowWidth()
                    yNDC = float(self.canvas.YtoPixel(hist.GetBinContent(ibin)))/self.canvas.GetWindowHeight()
                    if leg.IsInside(xNDC, yNDC) == 1:
                        print "hist.title, hist.GetBinCenter(ibin), hist.GetBinContent(ibin), xNDC, yNDC"
                        print hist.title, hist.GetBinCenter(ibin), hist.GetBinContent(ibin), xNDC, yNDC
                        if hist.GetBinContent(ibin) != 0.0:
                            print "hist.title, hist.GetBinError(ibin) , hist.GetBinContent(ibin)"
                            print hist.title, hist.GetBinError(ibin), hist.GetBinContent(ibin)
                            if hist.GetBinError(ibin) == 0.0:
                                locoverlap[loc] += 1.0
                            else:
                                locoverlap[loc] += hist.GetBinError(ibin)/hist.GetBinContent(ibin)
                            print "loc, locoverlap[loc]"
                            print loc, locoverlap[loc]

            if locoverlap[loc] == 0:
                print "loc, locoverlap[loc]"
                print loc, locoverlap[loc]
                return leg

        # Use the lest overlapping legend
        res = list(sorted(locoverlap, key=locoverlap.__getitem__, reverse=False))
        return self.SetLegend([hist.title for hist in hists], res[0])

    def LegendAddEntry(self, leg, hists):
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextFont(62)
        leg.SetTextSize(0.045)

        from collections import defaultdict
        typedict = defaultdict(list)
        histdict = {}
        for hist in hists:
            histdict[hist.title] = hist
            typedict[hist.ptype].append(hist.title)

        # DATA first
        for order in ["Data", "Signal", "Background"]:
            if order not in typedict:
                continue

            for htitle in sorted(typedict[order]):
                leg.AddEntry(histdict[htitle], htitle, "l")

# ============================================================================#
# -------------------------------     Axis     -------------------------------#
# ============================================================================#
    def SetYaxis(self, yaxis, histItgl):
        yaxis.SetTitleOffset(1.25)
        # For normailzation
        UnitItgl = [False for itgl in histItgl if math.fabs(itgl - 1) > 0.01]
        orgTitle = yaxis.GetTitle()

        if len(orgTitle) == 0:
            orgTitle = "a.u."
        else:
            if len(UnitItgl) == 0:
                orgTitle += " (Norm. to Unity)"
        yaxis.SetTitle(orgTitle)

# ============================================================================#
# -------------------------------     Tools     ------------------------------#
# ============================================================================#

# ============================================================================#
# -------------------------------     Draw     -------------------------------#
# ============================================================================#
    def DrawLineComparison(self, hists, **kw):
        outname = kw.get('outname', "output")
        addlatex = kw.get('addlatex', ())
        options = kw.get('options', "hist")
        legloc = kw.get('legloc', "topright")
        self.canvas.cd()
        self.canvas.Clear()

        ymax = 0
        localColor = False
        if len(set(hist.GetLineColor() for hist in hists)) == 1:
            localColor = True

        for it in range(0, len(hists)):

            if localColor and it <= len(PyColors):
                hists[it].SetLineColor(PyColors[it])

            hists[it].SetFillColor(0)
            if it == 0:
                yaxis = hists[it].GetYaxis()
                hists[it].Draw(options)
            else:
                hists[it].Draw("%s same" % options)

            ymax = max(ymax, hists[it].GetMaximum())

        self.SetYaxis(yaxis, [hist.Integral() for hist in hists if hist.Integral() != 0.0])
        yaxis.SetRangeUser(0, 1.2*ymax)

        leg = self.SetLegend([hist.title for hist in hists], legloc)
        # leg = self.SetAutoLegend(hists)
        self.LegendAddEntry(leg, hists)
        if leg is not None:
            leg.Draw()
        if len(addlatex) != 0:
            latex = ROOT.TLatex()
            latex.SetNDC()
            print(addlatex)
            latex.DrawLatex(*addlatex)
        self.CanvasRedraw(outname=outname, norm=hists[0].norm)
        return self.canvas

    def DrawStackPlot(self, hists):
        self.canvas.cd()
        self.canvas.Clear()

        stack = HistStack(name="fdf")
        # leg = self.SetAutoLegend(hists)
        leg = self.SetLegend([hist.title for hist in hists], "topright")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextFont(62)
        leg.SetTextSize(0.045)

        yaxis = 0
        for hist in hists:
            if hist.ptype == "Background":
                print hist.title, hist.GetFillColor(), hist.GetFillStyle()
                stack.Add(hist)
                leg.AddEntry(hist, hist.title, "f")
                if yaxis == 0:
                    yaxis = hist.GetYaxis()
        stack.Draw()
        stack.GetXaxis().SetTitle(hists[0].GetXaxis().GetTitle())
        stack.GetYaxis().SetTitle(hists[0].GetYaxis().GetTitle())
        # yaxis = stack.GetYaxis()

        ymax = stack.GetMaximum()
        print(ymax)

        for hist in hists:
            if hist.ptype == "Data":
                hist.Draw("p same")
                leg.AddEntry(hist, hist.title, "m")
                ymax = max(ymax, hist.GetMaximum())
            if hist.ptype == "Signal":
                hist.Draw("hist same")
                leg.AddEntry(hist, hist.title, "l")
                ymax = max(ymax, hist.GetMaximum())

            print(ymax)
        # self.SetYaxis(yaxis, [hist.Integral() for hist in hists if hist.Integral() != 0.0])
        yaxis.SetRangeUser(0, 1.5*ymax)
        leg.Draw()

        self.CanvasRedraw(norm=hists[0].norm)
        return self.canvas

    def DrawSBPlot(self, hists):
        self.canvas.cd()
        self.canvas.Clear()

        # leg = self.SetAutoLegend(hists)
        leg = self.SetLegend([hist.title.replace('Top_', '') for hist in hists if hist.ptype == "Signal"], "topright")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextFont(62)
        leg.SetTextSize(0.045)

        yaxis = 0
        bkhist = 0
        for hist in hists:
            if hist.ptype == "Background":
                if bkhist == 0:
                    bkhist = hist.Clone("Background")
                else:
                    bkhist += hist
                # print hist.title, hist.GetFillColor(), hist.GetFillStyle()
                # stack.Add(hist)

        bkhist.GetXaxis().SetTitle(hists[0].GetXaxis().GetTitle())
        bkhist.GetYaxis().SetTitle(hists[0].GetYaxis().GetTitle())
        # yaxis = .GetYaxis()
        ymax = 0

        coloridx = 0

        for hist in hists:
            if hist.ptype == "Data":
                hist.Draw("p same")
                leg.AddEntry(hist, hist.title, "m")
                ymax = max(ymax, hist.GetMaximum())
            if hist.ptype == "Signal":
                hist.Divide(bkhist)
                for ibins in range(1, hist.GetNbinsX()):
                    print (hist.title, ibins, hist.GetBinCenter(ibins), hist.GetBinContent(ibins), hist.GetBinError(ibins))
                    if hist.GetBinCenter(ibins) < 3:
                        ymax = max(ymax, hist.GetBinContent(ibins))
                    # if hist.GetBinContent(ibins) != 0 and hist.GetBinError(ibins) / hist.GetBinContent(ibins) > 0.5:
                        # hist.SetBinContent(ibins, 0)
                hist.SetFillColor(0)
                hist.SetLineColor(PyColors[coloridx])
                coloridx += 1
                if yaxis == 0:
                    yaxis = hist.GetYaxis()
                    xaxis = hist.GetXaxis()
                    xaxis.SetRangeUser(0, 3)
                    hist.Draw("hist")
                else:
                    hist.Draw("hist same")
                leg.AddEntry(hist, hist.title.replace('Top_', ''), "l")
                print(ymax)

        # self.SetYaxis(yaxis, [hist.Integral() for hist in hists if hist.Integral() != 0.0])
        yaxis.SetRangeUser(0, 1.5*ymax)
        # xaxis.SetRangeUser(0, 3)
        leg.Draw()

        self.CanvasRedraw(norm=hists[0].norm)
        return self.canvas
