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
import copy
import re
from rootpy.plotting import Canvas
from Config import PyColors, PyOutPut, Lumi

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

    def PadRedraw(self, pad, norm="None", addlatex=(), addline=()):
        pad.cd()
        pad.Update()
        if norm != "No":
            if norm == "None" or norm == "Unit":
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
        self.PadRedraw(canvas, norm=kw.get('norm', 'None'), addlatex=kw.get('addlatex', ()),
                       addline=kw.get('addline', ()))
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

        dx = 0.03 + 0.025 * max([len(lstr.strip()) for lstr in hislegs])

        # x3 --- x2
        #  |      |
        # x1 --- x4

        if location == "topright":
            x2 = 0.98
            y2 = 0.88
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
    def SetYaxisTitle(self, yaxis, hists):
        # For normailzation
        UnitItgl = [hist.norm for hist in hists]
        orgTitle = yaxis.GetTitle()

        if len(orgTitle) == 0:
            orgTitle = "a.u."
        else:
            if len(set(UnitItgl)) == 0 and UnitItgl[0] == "Unit":
                orgTitle += " (Norm. to Unity)"
        yaxis.SetTitle(orgTitle)

    def UpdateAxis(self, hists, kw, prefix="", loc="top"):
        if len(hists) == 0:
            return
        yaxis = hists[0].GetYaxis()

        # Set Axis properties
        if prefix == "Ratio":
            self.UpdateRatioAxis(hists[0], loc)
        if prefix == "":
            hists[0].GetYaxis().SetTitleSize(ROOT.gStyle.GetTitleSize("Y"))
            hists[0].GetYaxis().SetLabelSize(ROOT.gStyle.GetLabelSize("Y"))
            hists[0].GetXaxis().SetTitleSize(ROOT.gStyle.GetTitleSize("X"))
            hists[0].GetXaxis().SetLabelSize(ROOT.gStyle.GetLabelSize("X"))
            # hists[0].GetYaxis().SetTitleOffset(ROOT.gStyle.GetTitleOffset("Y"))

        # Set YAxis Title
        if prefix == "" or (prefix == "Ratio" and loc == "top"):
            self.SetYaxisTitle(yaxis, hists)
        if prefix == "Ratio" and loc == "bot":
            RatioYTitle = [hist.RatioYTitle for hist in hists if hasattr(hist, "RatioYTitle")]
            RatioYTitle = list(set(RatioYTitle))
            if len(RatioYTitle) == 1:
                yaxis.SetTitle(RatioYTitle[0])
            else:
                if "RatioOpt" in kw:
                    yaxis.SetTitle(kw["RatioOpt"])
                else:
                    yaxis.SetTitle("Ratio")

        # Set Yaxis range
        if prefix == "Ratio" and loc == "boc":
            self.SetAxisRange(hists, kw, prefix=prefix, axis="Y")
            self.SetAxisRange(hists, kw, prefix=prefix, axis="X")
        else:
            self.SetAxisRange(hists, kw, prefix="", axis="Y")
            self.SetAxisRange(hists, kw, prefix="", axis="X")

    def SetAxisRange(self, hists, kw, prefix="", axis="Y"):
        if len(hists) == 0:
            raise AssertionError()

        if axis == "Y":
            haxis = hists[0].GetYaxis()
        elif axis == "X":
            haxis = hists[0].GetXaxis()
        else:
            return None

        returnmin = None
        returnmax = None
        histmin = None
        histmax = None

        if axis == "Y":
            [hist.SetMaximum() for hist in hists]
            [hist.SetMinimum() for hist in hists]
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
            returnmin = histmin if histmin <= 0 else 0

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
            returnmax = 1.25 * histmax if histmax > 0 else 0.8 * histmax

        if hists[0].InheritsFrom("THStack") and axis == "Y":
            hists[0].SetMinimum(returnmin)
            hists[0].SetMaximum(returnmax)
        else:
            if haxis is not None:
                haxis.SetRangeUser(returnmin, returnmax)

    def UpdateRatioAxis(self, hist, position):
        """Setting the axis label and title in ratio plot. Not full optimized yet.
        Currently purely edit by hand."""
        if position == "top":
            hist.GetYaxis().SetTitleOffset(0.8)
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
            hist.GetYaxis().CenterLabels()

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
            canvas.toppad.SetBottomMargin(0)
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
            canvas.botpad.SetTopMargin(0)
            canvas.botpad.SetBottomMargin(3*canvas.GetBottomMargin())
            canvas.botpad.SetFrameFillStyle(0)
            canvas.botpad.SetFrameBorderMode(0)
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
            if len(hists) == 2:
                rhists.append(self.GetRatioPlot(formular, hists, hists))
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

        self.PadRedraw(canvas.botpad, norm="No",
                       addlatex=kw.get('Ratioaddlatex', ()),
                       addline=kw.get('Ratioaddline', ()))

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
                return ratioObj

        # Merge the hist expect the first element
        for i in range(1, len(rhists)):
            lhists = rhists[i]
            hist = copy.deepcopy(lhists[0])
            for subhist in lhists[1:]:
                hist += subhist
            rhists[i] = hist

        if len(rhists) == 0:
            return []

        if len(rhists) == 1:
            return list(rhists[0])

        # Now evalute the formula
        # Assert rhists is [[], hist, hist..]
        ratios = []
        for As in rhists[0]:
            histformular = copy.deepcopy(rhists)
            histformular[0] = As
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
        Ahist = rhists[0]
        Bhist = rhists[1]

        if len(rhists) > 2:
            Chist = rhists[2]

        if len(rhists) > 3:
            print Chist.title
            Dhist = rhists[3]

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
# ============================================================================#
# -------------------------------     Draw     -------------------------------#
# ============================================================================#
    def DrawLineComparison(self, hists, **kw):
        """ A funtion for comparing lines """
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
        # leg = self.SetAutoLegend(hists)
        if leg is not None:
            self.LegendAddEntry(leg, hists)
            leg.Draw()

        if 'RatioOpt' in kw or 'RatioHist' in kw:
            self.UpdateAxis(hists, kw, prefix="Ratio", loc="top")
            self.PadRedraw(canvas.toppad, norm=kw.get('norm', 'None'),
                           addlatex=kw.get('addlatex', ()),
                           addline=kw.get('addline', ()))
            self.DrawRatioPlot(hists, kw, canvas=canvas)
            self.CanvasSave(canvas=canvas,
                            outname=kw.get('outname', "output_%s" % time.strftime("%y%m%d_%H%M%S")))
        else:
            self.UpdateAxis(hists, kw)
            return self.CanvasRedrawSave(**kw)

    def DrawStackPlot(self, stackHists, overlayHists=None, ontopHists=None, **kw):
        """ A funtion for comparing lines """
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
        stack.GetXaxis().SetTitle(hists[0].GetXaxis().GetTitle())
        stack.GetYaxis().SetTitle(hists[0].GetYaxis().GetTitle())
        hists.insert(0, stack)
        totalStack = stack.GetStack().Last().Integral()

        # Draw overlayHists
        if overlayHists is not None:
            for hist in overlayHists:
                hist.Draw("l same")
                leg.AddEntry(hist, hist.title, "l")

        if ontopHists is not None:
            for hist in ontopHists:
                temphist = hist.Clone()
                temphist.Scale(totalStack.Integral() / temphist.Integral())
                temphist.Draw("l same")
                leg.AddEntry(temphist, temphist.title, "l")

        leg.Draw()

        #########################
        if 'RatioOpt' in kw or 'RatioHist' in kw:
            self.UpdateAxis(hists, kw, prefix="Ratio", loc="top")
            self.PadRedraw(canvas.toppad, norm=kw.get('norm', 'None'),
                           addlatex=kw.get('addlatex', ()),
                           addline=kw.get('addline', ()))
            self.DrawRatioPlot(hists, kw, canvas=canvas)
            return self.CanvasSave(canvas=canvas,
                            outname=kw.get('outname', "output_%s" % time.strftime("%y%m%d_%H%M%S")))
        else:
            self.UpdateAxis(hists, kw)
            return self.CanvasRedrawSave(**kw)

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
