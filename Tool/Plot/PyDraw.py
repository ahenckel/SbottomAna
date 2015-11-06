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
import time
import os
import math
import re
import copy
import rootpy
from rootpy.plotting import Canvas
from Config import PyOutPut, Lumi

# ### To Do list ########
# 1. Auto scale the y-axis for the CMS label
# 2. Auto set the y-axis offset
# 3. Auto set the TLegend location base on the input hists
# 4. Add ratio plot
# 5. Draw Stack etc...
# #######################


class PyDraw():
    def __init__(self, OutDir):
        self.OutDir = OutDir
        if not os.path.isdir(self.OutDir):
            try:
                os.makedirs(self.OutDir)
            except OSError:
                pass

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

    def CanvasSave(self, canvas=None, outname="output"):
        if canvas is None:
            canvas = self.canvas
        canvas.Update()

        for outformat in PyOutPut:
            canvas.SaveAs("%s/%s.%s" % (self.OutDir, outname, outformat))
        return canvas

    def PadRedraw(self, pad, kw):
        addlatex=kw.get('addlatex', ())
        addline=kw.get('addline', ())
        if hasattr(pad, "ratio"):
            addlatex=kw.get('Ratioaddlatex', ())
            addline=kw.get('Ratioaddline', ())
        norm=kw.get('norm', None)
        pad.cd()
        pad.Update()
        pad.SetLogx(kw.get("Logx", False))
        pad.SetLogy(kw.get("Logy", False))

        if norm != "No":
            if norm is None or norm == "None" or norm == "Unit":
                CMS_lumi.CMS_lumi(pad, 23, 11, self.lumi)
            else:
                CMS_lumi.CMS_lumi(pad, 13, 11, self.lumi)

        if len(addlatex) != 0:
            self.DrawTLatex(addlatex)
        if len(addline) != 0:
            self.DrawTLine(pad, addline)

        pad.Update()
        pad.Modified()
        pad.RedrawAxis()

    def CanvasRedrawSave(self, canvas=None, **kw):
        if canvas is None:
            canvas = self.canvas
        self.PadRedraw(canvas, kw)
        self.CanvasSave(canvas=canvas,
                        outname=kw.get('outname', "output_%s" % time.strftime("%y%m%d_%H%M%S")))

# ============================================================================#
# ------------------------------     Legend     ------------------------------#
# ============================================================================#
    def SetLegend(self, hislegs, location):
        if location == "None":
            return None
        x1 = 0.0
        y1 = 0.0
        x2 = 1.0
        y2 = 1.0

        dy = 0.05 * len(hislegs)

        # dx = 0.015 * max([len(lstr.strip()) for lstr in hislegs])
        dx = 0.03 + 0.025 * max([len(lstr.strip()) for lstr in hislegs])

        # x3 --- x2
        #  |      |
        # x1 --- x4

        if location == "topright":
            x2 = 0.98
            y2 = 0.88
            x1 = x2 - dx
            y1 = y2 - dy

        if location == "topleft":
            x3 = 0.1575
            # y3 = 0.85
            y3 = 0.75
            x1 = x3
            y1 = y3 -dy
            x2 = x1 + dx
            y2 = y3

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

        leg = ROOT.TLegend(x1, y1, x2, y2)
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextFont(62)
        leg.SetTextSize(0.045)
        return leg

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
        # For cut flow
        pronames = list(set([h.proname for h in hists]))
        dirnames = list(set([h.dirname for h in hists]))
        shistnames = list(set([h.striphistname for h in hists]))
        histnames = list(set([h.histname for h in hists]))
        if len(pronames) == 1 and len(dirnames) ==1 and len(shistnames) == 1 and len(histnames) != 1 :
            # hists = [x for x in hists if "_" in x.histname]
            hists.sort(key=lambda x: int(x.histname.split("_")[-1]))
            for h in hists:
                leg.AddEntry(h, h.title, "l")
            return True

        # DATA first
        from collections import defaultdict
        typedict = defaultdict(list)
        histdict = {}
        for hist in hists:
            histdict[hist.title] = hist
            typedict[hist.ptype].append(hist.title)

        for order in ["Data", "Signal", "Background"]:
            if order not in typedict:
                continue

            # By default, sort by title
            for htitle in sorted(typedict[order]):
                leg.AddEntry(histdict[htitle], htitle, "l")

# ============================================================================#
# -------------------------------     Axis     -------------------------------#
# ============================================================================#
    def SetYaxisTitle(self, yaxis, hists):
        # For normailzation
        UnitItgl = [hist.norm for hist in hists]
        orgTitle = yaxis.GetTitle()

        if len(orgTitle) == 0:
            orgTitle = "a.u."
        else:
            if len(set(UnitItgl)) == 1 and UnitItgl[0] == "Unit":
                orgTitle += " (Norm. to Unity)"
        yaxis.SetTitle(orgTitle)

    def UpdateAxis(self, hists, kw, prefix="", loc="top"):
        if len(hists) == 0:
            return
        yaxis = hists[0].GetYaxis()
        xaxis = hists[0].GetXaxis()

        # Set Axis properties
        if prefix == "Ratio":
            self.UpdateRatioAxis(hists[0], loc)
        if prefix == "":
            hists[0].GetYaxis().SetTitleSize(ROOT.gStyle.GetTitleSize("Y"))
            hists[0].GetYaxis().SetLabelSize(ROOT.gStyle.GetLabelSize("Y"))
            hists[0].GetXaxis().SetTitleSize(ROOT.gStyle.GetTitleSize("X"))
            hists[0].GetXaxis().SetLabelSize(ROOT.gStyle.GetLabelSize("X"))
            # hists[0].GetYaxis().SetTitleOffset(ROOT.gStyle.GetTitleOffset("Y"))

        # Set Title
        if "XTitle" in kw:
            xaxis.SetTitle(kw["XTitle"])
        if prefix == "" or (prefix == "Ratio" and loc == "top"):
            if "YTitle" in kw:
                yaxis.SetTitle(kw["YTitle"])
            else:
                self.SetYaxisTitle(yaxis, hists)
        if prefix == "Ratio" and loc == "bot":
            if "RatioYTitle" in kw:
                yaxis.SetTitle(kw["RatioYTitle"])
            else:
                RatioYTitle = [hist.RatioYTitle for hist in hists if hasattr(hist, "RatioYTitle")]
                RatioYTitle = list(set(RatioYTitle))
                if len(RatioYTitle) == 1:
                    yaxis.SetTitle(RatioYTitle[0])
                else:
                    if "RatioOpt" in kw:
                        yaxis.SetTitle(kw["RatioOpt"])
                    else:
                        yaxis.SetTitle("Ratio")

        # Set Range
        if prefix == "Ratio" and loc == "bot":
            self.SetAxisRange(hists, kw, prefix=prefix, axis="X")
            self.SetAxisRange(hists, kw, prefix=prefix, axis="Y")
        else:
            self.SetAxisRange(hists, kw, prefix="", axis="X")
            self.SetAxisRange(hists, kw, prefix="", axis="Y")

    def SetAxisRange(self, hists, kw, prefix="", axis="Y"):
        if len(hists) == 0:
            raise AssertionError()

        if axis == "Y":
            haxis = hists[0].GetYaxis()
            haxis.SetMoreLogLabels(kw.get("Logy", False))
        elif axis == "X":
            haxis = hists[0].GetXaxis()
            haxis.SetMoreLogLabels(kw.get("Logx", False))
        else:
            return None

        returnmin = None
        returnmax = None
        histmin = None
        histmax = None

        if axis == "Y":
            [hist.GetMaximum() for hist in hists]
            [hist.GetMinimum() for hist in hists]
            histmax = max([hist.GetMaximum() for hist in hists])
            histmin = min([hist.GetMinimum() for hist in hists])
        elif axis == "X":
            histmax = max([hist.GetXaxis().GetXmax() for hist in hists])
            histmin = min([hist.GetXaxis().GetXmin() for hist in hists])

        # For Min
        if prefix + axis + "Min" in kw:
            axisMin = kw[prefix+axis+"Min"]
            returnmin = axisMin
        if prefix + axis + "MinScale" in kw:
            axisScaleMin = kw[prefix + axis + "MinScale"] * histmin if histmin is not None else None
            templist = [v for v in [returnmin, axisScaleMin] if v is not None]
            returnmin = None if len(templist) == 0 else min(templist)
        # Not getting set from Input, try hists
        if returnmin is None:
            attrname = prefix + axis + "Min"
            fromhistmin = [getattr(hist, attrname) for hist in hists if hasattr(hist, attrname)]
            attrname = prefix + axis + "MinScale"
            fromhistminScale = [getattr(hist, attrname) for hist in hists if hasattr(hist, attrname)]
            templist = fromhistmin + [v * histmin for v in fromhistminScale] if histmin is not None else []
            returnmin = None if len(templist) == 0 else min(templist)
        # Still None? Set to default
        if returnmin is None:
            if axis == "Y" and "Logy" in kw:
                returnmin = None
            else:
                returnmin = histmin

        # For Max
        if prefix + axis + "Max" in kw:
            axisMax = kw[prefix+axis+"Max"]
            returnmax = axisMax
        if prefix + axis + "MaxScale" in kw:
            axisScaleMax = kw[prefix + axis + "MaxScale"] * histmax if histmax is not None else None
            templist = [v for v in [returnmax, axisScaleMax] if v is not None]
            returnmax = None if len(templist) == 0 else max(templist)
        # Not getting set from Input, try hists
        if returnmax is None:
            attrname = prefix + axis + "Max"
            fromhistMax = [getattr(hist, attrname) for hist in hists if hasattr(hist, attrname)]
            attrname = prefix + axis + "MaxScale"
            fromhistMaxScale = [getattr(hist, attrname) for hist in hists if hasattr(hist, attrname)]
            templist = fromhistMax + [v * histmax for v in fromhistMaxScale] if histmax is not None else []
            returnmax = None if len(templist) == 0 else max(templist)
        # Still None? Set to default
        if returnmax is None:
            if axis == "Y":
                returnmax = 1.25 * histmax if histmax > 0 else 0.8 * histmax
            else:
                returnmax = histmax

        if hists[0].InheritsFrom("THStack") and axis == "Y":
            hists[0].SetMinimum(returnmin)
            hists[0].SetMaximum(returnmax)
        else:
            if haxis is not None and returnmin is not None:
                # this is  problematic for logY and X axis
                haxis.SetRangeUser(returnmin, returnmax)

    def UpdateRatioAxis(self, hist, position):
        """Setting the axis label and title in ratio plot. Not full optimized yet.
        Currently purely edit by hand."""
        if position == "top":
            hist.GetYaxis().SetTitleOffset(1.0)
            hist.GetYaxis().SetTitleSize(ROOT.gStyle.GetTitleSize('Y'))
            hist.GetYaxis().SetLabelSize(ROOT.gStyle.GetLabelSize("Y"))
            hist.GetYaxis().SetTickLength(ROOT.gStyle.GetTickLength("Y"))

        if position == "bot":
            hist.GetXaxis().SetTitleSize(ROOT.gStyle.GetTitleSize('X')*2.5)
            hist.GetXaxis().SetLabelSize(ROOT.gStyle.GetLabelSize("X")*3)
            hist.GetXaxis().SetTickLength(ROOT.gStyle.GetTickLength("X"))

            hist.GetYaxis().SetTitleSize(ROOT.gStyle.GetTitleSize('Y')*2)
            hist.GetYaxis().SetLabelSize(ROOT.gStyle.GetLabelSize("Y")*2)
            hist.GetYaxis().SetTickLength(ROOT.gStyle.GetTickLength("Y"))
            hist.GetYaxis().SetTitleOffset(ROOT.gStyle.GetTitleOffset('Y')/4)
            hist.GetYaxis().SetNdivisions(110)
            hist.GetYaxis().CenterTitle()
            # hist.GetYaxis().CenterLabels()

# ============================================================================#
# ----------------------------     Tex & Line     ----------------------------#
# ============================================================================#
    def DrawTLatex(self, addlatex):
        """Getting list of tuple as (x, y, latex, color, size, align, front)."""
        if not isinstance(addlatex, list):
            addlatex = [addlatex]
        latex = ROOT.TLatex()
        latex.SetNDC()
        for entry in addlatex:
            if len(entry) > 3:
                latex.SetTextColor(entry[3])
            if len(entry) > 4:
                latex.SetTextSize(entry[4])
            if len(entry) > 5:
                latex.SetTextAlign(entry[5])
            if len(entry) > 6:
                latex.SetTextFont(entry[6])
            latex.DrawLatex(entry[0], entry[1], entry[2])

    def DrawTLine(self, pad, addline):
        """Getting list of tuple as (x1, y1, x2, y2, color, style, width)."""
        if not isinstance(addline, list):
            addline = [addline]
        line = ROOT.TLine()
        for entry in addline:
            if len(entry) > 4:
                line.SetLineColor(entry[4])
            if len(entry) > 5:
                line.SetLineStyle(entry[5])
            if len(entry) > 6:
                line.SetLineWidth(entry[6])
            entry = self.SetTLineRange(pad, entry)
            line.DrawLine(entry[0], entry[1], entry[2], entry[3])

    def SetTLineRange(self, pad, entry):
        xmin = ROOT.Double(-999)
        xmax = ROOT.Double(-999)
        ymin = ROOT.Double(-999)
        ymax = ROOT.Double(-999)
        pad.GetRangeAxis(xmin, ymin, xmax, ymax)

        reentry = list(entry)
        if reentry[0] is None:
            reentry[0] = xmin
        if reentry[1] is None:
            reentry[1] = ymin
        if reentry[2] is None:
            reentry[2] = xmax
        if reentry[3] is None:
            reentry[3] = ymax
        return tuple(reentry)

# ============================================================================#
# -------------------------------     Ratio     ------------------------------#
# ============================================================================#
    def PrepRatioPad(self, loc="top", canvas=None):
        if canvas is None:
            canvas = self.canvas
        # split canvas
        canvas.cd()
        canvas.Range(0, 0, 1, 1)
        if loc == "top":
            # setattr(canvas, "toppad", ROOT.TPad("top",  "toppad",  0.0, 1.0/PadFraction, 1.0, 1.0))
            setattr(canvas, "toppad", ROOT.TPad("top",  "toppad",  0.0, 0.25, 1.0, 1.0))
            canvas.toppad.Draw()
            canvas.toppad.cd()
            canvas.toppad.SetFillColor(0)
            canvas.toppad.SetBorderMode(0)
            canvas.toppad.SetBorderSize(0)
            canvas.toppad.SetTickx(1)
            canvas.toppad.SetTicky(1)
            canvas.toppad.SetLeftMargin(canvas.GetLeftMargin())
            canvas.toppad.SetRightMargin(canvas.GetRightMargin())
            canvas.toppad.SetTopMargin(canvas.GetTopMargin())
            canvas.toppad.SetBottomMargin(0.01)
            canvas.toppad.SetFrameFillStyle(0)
            canvas.toppad.SetFrameBorderMode(0)
            canvas.toppad.cd()

        if loc == "bot":
            # setattr(canvas, "botpad", ROOT.TPad("bot",  "botpad",  0.0, 0.0, 1.0, 1.0/PadFraction))
            setattr(canvas, "botpad", ROOT.TPad("bot",  "botpad",  0.0, 0.0, 1.0, 0.25))
            canvas.botpad.Draw()
            canvas.botpad.cd()
            canvas.botpad.SetFillColor(0)
            canvas.botpad.SetBorderMode(0)
            canvas.botpad.SetBorderSize(2)
            canvas.botpad.SetTickx(1)
            canvas.botpad.SetTicky(1)
            canvas.botpad.SetLeftMargin(canvas.GetLeftMargin())
            canvas.botpad.SetRightMargin(canvas.GetRightMargin())
            canvas.botpad.SetTopMargin(0.01)
            canvas.botpad.SetBottomMargin(3*canvas.GetBottomMargin())
            canvas.botpad.SetFrameFillStyle(0)
            canvas.botpad.SetFrameBorderMode(0)
            setattr(canvas.botpad, "ratio", True)
            canvas.botpad.cd()

        return canvas

    def DrawRatioPlot(self, hists, kw, canvas=None):
        if canvas is None:
            canvas = self.canvas
        self.PrepRatioPad(loc="bot", canvas=canvas)

        formular = kw.get("RatioOpt", "A/B")
        ratiohist = kw.get("RatioHist", None)

        rhists = []
        # Setting up the ratio hist
        # 1. No input hist, but only two hists in plot
        if ratiohist is None:
            rhists = self.GetRatioPlot(formular, hists, hists)
            # if len(hists) == 2:
                # rhists = self.GetRatioPlot(formular, hists, hists)
        else:
            if not isinstance(ratiohist, list):
                ratiohist = [ratiohist]
            rhists = self.GetRatioPlot(formular, ratiohist, hists)

        for rit in range(0, len(rhists)):
            rhists[rit].SetFillColor(0)
            if rit == 0:
                rhists[rit].Draw("hist")
            else:
                rhists[rit].Draw("hist same")

        # if yaxis != None:
            # yaxis.SetTitle(formular)
            # # yaxis.SetTitle("Ratio")

        if len(rhists) > 0:
            self.UpdateAxis(rhists, kw, prefix="Ratio", loc="bot")
        else:
            self.DrawTLatex((0.12, 0.2, "No Ratio Plot!", 2, 0.7))

        tempkw = copy.deepcopy(kw)
        tempkw["norm"] = "No"
        self.PadRedraw(canvas.botpad, tempkw)
        del tempkw

    def GetRatioPlot(self, formular, ratioObj, hists):
        rhists = []
        for obji in range(0, len(ratioObj)):
            obj = ratioObj[obji]
            if isinstance(obj, str):
                rhists.append(self.GetHistWithStr(hists, obj))
            elif isinstance(obj, list):
                temp = []
                for j in obj:
                    temp += self.GetHistWithStr(hists, j)
                rhists.append(temp)
            else:
                rhists.append(obj)

        # Merge the hist expect the first element
        for i in range(1, len(rhists)):
            lhists = rhists[i]
            if isinstance(lhists, list) and len(lhists) >= 1:
                hist = lhists[0].Clone()
                for subhist in lhists[1:]:
                    hist += subhist
            else:
                hist = lhists
            rhists[i] = hist

        if len(rhists) == 0:
            return []

        if len(rhists) == 1:
            return list(rhists[0])

        # Now evalute the formula
        # Assert rhists is [[], hist, hist..]
        ratios = []
        if not isinstance(rhists[0], list):
            rhists[0] = [rhists[0]]

        for As in rhists[0]:
            histformular = rhists[1:]
            histformular.insert(0, As)
            ratios.append(self.EvalRatioFormular(formular, histformular, len(rhists[0])))
        return ratios

    def GetHistWithStr(self, hists, objstr):
        lpro = [hist.proname for hist in hists]
        ldir = [hist.dirname for hist in hists]
        lhist = [hist.histname for hist in hists]

        npro = lpro.count(objstr)
        ndir = ldir.count(objstr)
        nhist = lhist.count(objstr)
        nobj = npro + ndir + nhist
        rehist = []
        if nobj == 0:
            print("No Obj found")
            raise AssertionError()
            return None
        if npro != 0:
            for hist in hists:
                if hist.proname == objstr:
                    rehist.append(hist)
        if ndir != 0:
            for hist in hists:
                if hist.dirname == objstr:
                    rehist.append(hist)
        if nhist != 0:
            for hist in hists:
                if hist.histname == objstr:
                    rehist.append(hist)
        if len(rehist) != nobj:
            raise AssertionError()
        return rehist

    def EvalRatioFormular(self, formular, rhists, Alen):
        Ahist = self.GetHistForEval(rhists[0])
        Bhist = self.GetHistForEval(rhists[1])

        if len(rhists) > 2:
            Chist = self.GetHistForEval(rhists[2])

        if len(rhists) > 3:
            Dhist = self.GetHistForEval(rhists[3])

        operator = formular
        ytitle = formular
        operator = operator.replace("A", "Ahist").replace("B", "Bhist").replace("C", "Chist").replace("D", "Dhist")
        rhist = eval(operator)

        ytitle = ytitle.replace("A", "%Ahist%").replace("B", "%Bhist%").replace("C", "%Chist%").replace("D", "%Dhist%")
        if len(rhists) > 0:
            ytitle = ytitle.replace("%Ahist%", Ahist.title) if Alen == 1 else ytitle.replace("%Ahist%", "X")
        if len(rhists) > 1:
            ytitle = ytitle.replace("%Bhist%", Bhist.title)
        if len(rhists) > 2:
            ytitle = ytitle.replace("%Chist%", Chist.title)
        if len(rhists) > 3:
            ytitle = ytitle.replace("%Dhist%", Dhist.title)

        while True:
            ma = re.match(r"(.*)/(.*)", ytitle)
            if ma is None:
                break
            if len(ma.groups()) == 2:
                ytitle = "#frac{%s}{%s}" % (ma.group(1), ma.group(2))
            else:
                break

        if not hasattr(rhist, "RatioYTitle"):
            setattr(rhist, "RatioYTitle", ytitle)

        return rhist

    def GetHistForEval(self, hist):
        rehist = None
        if hist.InheritsFrom("THStack"):
            rehist = hist.GetStack().Last().Clone()
        else:
            rehist = hist.Clone()
        setattr(rehist, "title", hist.title)
        return rehist

# ============================================================================#
# -------------------------------     Tools     ------------------------------#
# ============================================================================#
    def GetSmartOutName(self, hists, kw):
        if "outname" in kw:
            return kw

        kw["outname"] = ""
        histoutname = list(set(hist.outname for hist in hists))
        if len(histoutname) == 0:
            return
        if len(histoutname) == 1 and histoutname[0] != "":
            kw["outname"] = histoutname[0]
        else:
            kw["outname"] = "_VS_".join(histoutname)

        return kw

    def GetCommonOpt(self, hists, kw):
        attrs = {}

        for hist in hists:
            histvar = vars(hist)
            for k, v in histvar.iteritems():
                if k[0] == "_":
                    continue
                if k not in attrs:
                    attrs[k] = set()
                    attrs[k].add(v)
                else:
                    attrs[k].add(v)

        for k, v in attrs.iteritems():
            if k in kw:
                pass
            else:
                if len(v) == 1:
                    kw[k] = list(v)[0]

    def UpdateKWargs(self, hists, kw):
        self.GetSmartOutName(hists, kw)
        self.GetCommonOpt(hists, kw)

    def UpdateLineColor(self, hists):
        from Config import PyColors
        if len(set(hist.GetLineColor() for hist in hists)) == 1:
            for it in range(0, len(hists)):
                if it <= len(PyColors):
                    hists[it].SetLineColor(PyColors[it])

    def CopyLocalHists(self, hists):
        if hists is None:
            return None
        if len(hists) == 0:
            return []

        rehist = []
        for i, hist in enumerate(hists):
            rehist.append(hist.Clone())
            for k, v in vars(hist).iteritems():
                setattr(rehist[i], k, v)

        pronames = list(set([h.proname for h in hists]))
        dirnames = list(set([h.dirname for h in hists]))
        shistnames = list(set([h.striphistname for h in hists]))
        histnames = list(set([h.histname for h in hists]))
        if len(pronames) == 1 and len(dirnames) ==1 and len(shistnames) == 1 and len(histnames) != 1 :
            # hists = [x for x in hists if "_" in x.histname]
            rehist.sort(key=lambda x: int(x.histname.split("_")[-1]))

        return rehist

    def MergeHistCate(self, hists):
        bkhist = 0
        rehist = {}
        from collections import defaultdict
        rehist = defaultdict(lambda: defaultdict(None))
        for hist in hists:
            if hist.ptype == "Background":
                rehist["Background"][hist.proname] = hist
                if bkhist == 0:
                    bkhist = hist.Clone("Background")
                else:
                    bkhist += hist
            if hist.ptype == "Data":
                rehist["Data"][hist.proname] = hist
                setattr(hist, "legOpt", "m")
                rehist.append(hist)
            if hist.ptype == "Signal":
                rehist["Signal"][hist.proname] = hist
                setattr(hist, "legOpt", "l")
        rehist["Background"]["Total"] = bkhist
        return rehist

    def CalSignificance(self, hists, formular=None, direction=0):
        rehists = []
        for k, v in hists["Signal"].iteritems():
            temphist = v.Clone(k)
            for k1, v1 in vars(v).iteritems():
                setattr(temphist, k1, v1)
            if formular == "S/B":
                rehists.append(self.SigSBRatio(temphist, hists["Background"]["Total"], direction))
            if formular is None:
                rehists.append(self.SigFormular15(temphist, hists["Background"]["Total"], direction))
        return rehists

    def SigSBRatio(self, sighist, bkhist, direction):
        # print bkhist.Integral()
        for ibins in range(1, sighist.GetNbinsX()):
            newcontent = 0
            newerror = 0
            if bkhist.GetBinContent(ibins) == 0:
                newcontent = 0
                newerror = 0
            else:
                newcontent = sighist.GetBinContent(ibins) / bkhist.GetBinContent(ibins)
                newerror = 0
            sighist.SetBinContent(ibins, newcontent)
            sighist.GetBinError(ibins, newerror)
        return sighist

    def SigFormular15(self, sighist, bkhist, sigunc=0.10, bkunc=0.15, **kw):
        # print bkhist.Integral()
        for ibins in range(1, sighist.GetNbinsX()):
            newcontent = 0
            newerror = 0
            if bkhist.GetBinContent(ibins) == 0:
                newcontent = 0
                newerror = 0
            else:
                # S/sqrt(S + B + sigma_S^2 + sigma_B^2 + 1.5)
                denominator = sighist.GetBinContent(ibins) + bkhist.GetBinContent(ibins)
                + (sigunc * sighist.GetBinContent(ibins))**2 + (bkunc * bkhist.GetBinContent(ibins))**2 + 1.5
                newcontent = sighist.GetBinContent(ibins) / math.sqrt(denominator)
                # print sighist.GetBinContent(ibins), bkhist.GetBinContent(ibins), (sigunc * sighist.GetBinContent(ibins))**2 , (bkunc * bkhist.GetBinContent(ibins))**2, denominator, newcontent
                newerror = 0
            sighist.SetBinContent(ibins, newcontent)
            sighist.GetBinError(ibins, newerror)
            if newcontent > 100:
                sighist.SetBinContent(ibins, 0)
                sighist.GetBinError(ibins, 0)
        sighist.SetBinContent(0, 0)
        sighist.GetBinError(0, 0)
        sighist.SetBinContent(sighist.GetNbinsX()+1, 0)
        sighist.GetBinError(sighist.GetNbinsX()+1, 0)
        # print sighist.dirname
        # sighist.title = sighist.dirname
        # sighist.title = sighist.title.replace("Top_", "")
        return sighist


# ============================================================================#
# -------------------------------     Draw     -------------------------------#
# ============================================================================#
    def DrawLineComparison(self, hists_, **kw):
        """ A funtion for comparing lines """
        hists = self.CopyLocalHists(hists_)
        self.UpdateKWargs(hists, kw)
        legloc = kw.get('legloc', "topright")
        options = kw.get('DrawOpt', "hist")

        self.canvas.cd()
        self.canvas.Clear()

        if 'RatioOpt' in kw or 'RatioHist' in kw:
            canvas = self.PrepRatioPad(loc="top")

        for it in range(0, len(hists)):
            hists[it].SetLineColor(hists[it].Linecolor)
            hists[it].SetLineStyle(hists[it].Linestyle)
            hists[it].SetFillColor(0)
            if it == 0:
                hists[it].Draw(options)
            else:
                hists[it].Draw("%s same" % options)

        self.UpdateLineColor(hists)
        leg = self.SetLegend([hist.title for hist in hists], legloc)
        if leg is not None:
            self.LegendAddEntry(leg, hists)
            leg.Draw()

        if 'RatioOpt' in kw or 'RatioHist' in kw:
            self.UpdateAxis(hists, kw, prefix="Ratio", loc="top")
            self.PadRedraw(canvas.toppad, kw)
            self.DrawRatioPlot(hists, kw, canvas=canvas)
            self.CanvasSave(canvas=canvas,
                            outname=kw.get('outname', "output_%s" % time.strftime("%y%m%d_%H%M%S")))
        else:
            self.UpdateAxis(hists, kw)
            return self.CanvasRedrawSave(**kw)

    def DrawStackPlot(self, stackHists, overlayHists=None, ontopHists=None, **kw):
        """ A funtion for comparing lines """
        stackHists = self.CopyLocalHists(stackHists)
        overlayHists = self.CopyLocalHists(overlayHists)
        ontopHists = self.CopyLocalHists(ontopHists)
        self.canvas.cd()
        self.canvas.Clear()

        hists = []
        hists += stackHists
        if overlayHists is not None:
            self.UpdateLineColor(overlayHists)
            hists += overlayHists
        if ontopHists is not None:
            self.UpdateLineColor(ontopHists)
            hists += ontopHists
        self.UpdateKWargs(hists, kw)
        legloc = kw.get('legloc', "topright")
        options = kw.get('DrawOpt', '')

        if 'RatioOpt' in kw or 'RatioHist' in kw:
            canvas = self.PrepRatioPad(loc="top")

        leg = self.SetLegend([hist.title for hist in hists], legloc)
        ##########################
        # Draw Stack
        stack = ROOT.THStack("stack", "stack")
        ROOT.SetOwnership(stack, False)  # Trick to fix the crashing at the end of run
        for hist in stackHists:
            hist.SetFillColor(hist.Fillcolor)
            hist.SetFillStyle(hist.Fillstyle)
            hist.SetLineColor(hist.Linecolor)
            stack.Add(hist)
            leg.AddEntry(hist, hist.title, "f")
        stack.Draw(options)

        setattr(stack, "norm", hists[0].norm)
        setattr(stack, "proname", "Stack")
        setattr(stack, "title", "Stack")
        setattr(stack, "dirname", hists[0].dirname)
        setattr(stack, "histname", hists[0].histname)
        stack.GetXaxis().SetTitle(hists[0].GetXaxis().GetTitle())
        stack.GetYaxis().SetTitle(hists[0].GetYaxis().GetTitle())
        hists.insert(0, stack)
        totalStack = stack.GetStack().Last().Integral()

        # Draw overlayHists
        if overlayHists is not None:
            for hist in overlayHists:
                hist.SetLineColor(hist.Linecolor)
                hist.SetLineStyle(hist.Linestyle)
                hist.Draw("Hist same")
                leg.AddEntry(hist, hist.title, "l")

        if ontopHists is not None:
            for hist in ontopHists:
                temphist = hist.Clone()
                temphist.SetLineColor(hist.Linecolor)
                temphist.SetLineStyle(hist.Linestyle)
                if temphist.Integral() != 0:
                    temphist.Scale(totalStack / temphist.Integral())
                temphist.Draw("Hist same")
                temphist.title += "#scale[0.8]{#color[2]{x Norm}}"
                leg.AddEntry(temphist, temphist.title, "l")

        leg.Draw()

        #########################
        if 'RatioOpt' in kw or 'RatioHist' in kw:
            self.UpdateAxis(hists, kw, prefix="Ratio", loc="top")
            self.PadRedraw(canvas.toppad, kw)
            self.DrawRatioPlot(hists, kw, canvas=canvas)
            return self.CanvasSave(canvas=canvas,
                                   outname=kw.get('outname', "output_%s" % time.strftime("%y%m%d_%H%M%S")))
        else:
            self.UpdateAxis(hists, kw)
            return self.CanvasRedrawSave(**kw)

    def DrawSBPlot(self, hists_, **kw):
        """A function for calculating the significance."""
        inhists = self.CopyLocalHists(hists_)
        self.UpdateKWargs(inhists, kw)
        linehists = []

        # Getting pro, dir and hist set
        # proSet = set([hist.proname for hist in inhists])
        dirSet = set([hist.dirname for hist in inhists])
        histSet = set([hist.histname for hist in inhists])

        if len(histSet) > 1:
            raise AssertionError()

        for hdir in list(dirSet):
            dirhists = [hist for hist in inhists if hist.dirname == hdir]
            mhists = self.MergeHistCate(dirhists)
            linehists += self.CalSignificance(mhists, formular=kw.get("SigOpt", None),
                                              direction=kw.get("Direction", 0))

        return self.DrawLineComparison(linehists, **kw)

    def Draw2DComparison(self, hists_, **kw):
        """ A funtion for comparing lines """
        hists = self.CopyLocalHists(hists_)
        self.UpdateKWargs(hists, kw)
        options = kw.get('DrawOpt', "COLZ")

        self.canvas.cd()
        self.canvas.Clear()
        if len(hists) > 1:
            self.canvas.DivideSquare(len(hists), 0.0001, 0.0001)

        for it in range(0, len(hists)):
            curpad = self.canvas.cd(it+1)
            hists[it].Draw(options)
            if isinstance(hists[it], rootpy.plotting.hist._Hist2D):
                self.DrawTLatex((0.5, 0.96, hists[it].title, 2, 0.06))
            else:
                self.PadRedraw(curpad, kw)

        return self.CanvasSave(canvas=self.canvas,
                               outname=kw.get('outname', "output_%s" % time.strftime("%y%m%d_%H%M%S")))

    def DrawComparison(self, hists_, **kw):
        htype = list(set([isinstance(h, rootpy.plotting.hist._Hist) for h in hists_]))

        if len(htype) == 1 and htype[0]:
            return self.DrawLineComparison(hists_, **kw)
        else:
            return self.Draw2DComparison(hists_, **kw)
