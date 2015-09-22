#!/usr/bin/env python
# encoding: utf-8

# File        : scatter.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Aug 05
#
# Description :

import csv
import ROOT
from plotly.graph_objs import *
from collections import defaultdict
import plotly.plotly as py


def PlotEvent(num, lines):
    outname = 'T2bb900_%s' % num
    tlzv = []
    for line in lines:
        tlzv.append([line[0].strip(), ROOT.TLorentzVector(
            ROOT.Double(line[1].strip()), ROOT.Double(line[2].strip()), ROOT.Double(line[3].strip()), ROOT.Double(line[4].strip()))])

    datalist = []
    shapelist = []
    objlist = list(set([l[0] for l in tlzv]))

    objlist = ['B', 'AK4Jet', 'HEPTop', 'MET']

    objdict = defaultdict(list)

    for lv in tlzv:
        if lv[0] in objlist:
            objdict[lv[0]].append(lv[1])

    objmaker = {
        'B'      : ['x', 20, 'red'],
        'Top'    : ['diamond-x', 20, 'blue'],
        'Had'    : ['circle', 15, 'black'],
        'HEPTop' : ['hexagram', 20, 'pink'],
        'W'      : ['cross', 20, 'green'],
        'T3Top'  : ['square-cross', 20, 'purple'],
        'AK4Jet' : ['square', 15, 'cyan'],
        'MET'    : ['square', 10, 'blue'],
    }

    # print (objdict['B'][1] +  objdict['Had'][3]).Pt()
    # print objdict['B'][-1].DeltaR(objdict['Had'][-1])

    for obj in objlist:
        # print obj, [ v.Eta() for v in objdict[obj]]
        datalist.append(
            Scatter(
                x=[v.Eta() for v in objdict[obj]],
                y=[v.Phi() for v in objdict[obj]],
                mode='markers',
                name=obj,
                text=["%.1f, %.1f" % (v.Pt(), v.M()) for v in objdict[obj]],
                marker=Marker(
                    symbol=objmaker[obj][0],
                    size=objmaker[obj][1],
                    color=objmaker[obj][2],
                )
            )
        )

    circlelist = ["AK4Jet", "HEPTop"]
    for obj in circlelist:
        for v in objdict[obj]:
            shapedict = {
                'type': 'circle',
                'xref': 'x',
                'yref': 'y',
                'x0': 1,
                'y0': 1,
                'x1': 3,
                'y1': 3,
                'line': {
                    'color': 'rgba(50, 171, 96, 1)',
                },
            }
            if obj in ["HEPTop", "T3Top"]:
                shapedict['x0'] = v.Eta()-1.5
                shapedict['y0'] = v.Phi()-1.5
                shapedict['x1'] = v.Eta()+1.5
                shapedict['y1'] = v.Phi()+1.5
            # elif obj in ['B', 'Top', 'Had', 'W']:
                # shapedict['x0'] = v.Eta()-0.2
                # shapedict['y0'] = v.Phi()-0.2
                # shapedict['x1'] = v.Eta()+0.2
                # shapedict['y1'] = v.Phi()+0.2
            elif obj in ['AK4Jet']:
                shapedict['x0'] = v.Eta()-0.4
                shapedict['y0'] = v.Phi()-0.4
                shapedict['x1'] = v.Eta()+0.4
                shapedict['y1'] = v.Phi()+0.4
            shapedict['line']['color'] = objmaker[obj][2]
            shapelist.append(shapedict)

    for v in objdict['MET']:
        shapedict2 = {
            'type': 'line',
            'xref': 'x',
            'yref': 'y',
            'x0': -5,
            'y0': v.Phi(),
            'x1': 5,
            'y1': v.Phi(),
            'line': {
                'color': 'blue',
                'width': 4,
                'dash': 'dashdot',
            },
        }
        shapelist.append(shapedict2)

    data = Data(datalist)
    # layout = Layout(
        # title=outname,
        # xaxis=XAxis(
            # range=[-5, 5],
            # autorange=True,
            # title="eta"
        # ),

        # yaxis=YAxis(
            # range=[0, 8],
            # autorange=True,
            # title="phi"
        # ),
    # )

    layout = {
        'xaxis': {
            'range': [-5, 5],
            'zeroline': False,
            'title': "eta",
            'autorange': True,
        },
        'yaxis': {
            'range': [-3.5, 3.5],
            'title': 'phi',
            'autorange': True,
        },
        'width': 600,
        'height': 600,
        'shapes': shapelist
    }

    # fig = plotly.graph_objs.Figure(data=data, layout=layout)
    fig = {
        'data': data,
        'layout': layout,
    }
    plot_url = py.plot(fig, validate=False,filename=outname)


def AnaEvent(num, lines):
    tlzv = []
    for line in lines:
        tlzv.append([line[0].strip(), ROOT.TLorentzVector(
            ROOT.Double(line[1].strip()), ROOT.Double(line[2].strip()), ROOT.Double(line[3].strip()), ROOT.Double(line[4].strip()))])

    objlist = ['B', 'AK4Jet', 'MET']
    objdict = defaultdict(list)
    for lv in tlzv:
        if lv[0] in objlist:
            objdict[lv[0]].append(lv[1])

    fatjetlist = []
    for v in objdict['AK4Jet']:
        # sumV = ROOT.TLorentzVector(0, 0, 0, 0)
        import copy
        sumV = copy.deepcopy(v)
        # print "++++" , v.Pt()
        jcount = 0
        for j in objdict['AK4Jet']:
            if v.DeltaR(j) < 1.5 and v.DeltaR(j) != 0:
                sumV += j
                jcount+=1
        # print "----" , jcount, sumV.Pt(), v.Pt(),  sumV.Pt()/v.Pt()
        if sumV.Pt()/v.Pt() > 1.0:
            fatjetlist.append(sumV)
    # print fatjetlist

    for t in fatjetlist:
        out = ",".join(["1", "HEPTop", str(t.Px()), str(t.Py()), str(t.Pz()), str(t.E())])
        print out

if __name__ == "__main__":
    infile = open('out', 'r')

    # define csv reader object, assuming delimiter is tab
    tsvfile = csv.reader(infile, delimiter=',')

    lines = []

    # iterate through lines in file
    for line in tsvfile:
        lines.append(line)

    # lines = lines[19:]
    eventnumber = list(set([line[0] for line in lines]))
    eventdict = defaultdict(list)
    for i, event in enumerate(eventnumber):
        eventdict[event] = [line[1:] for line in lines if line[0] == event]
        AnaEvent(event, eventdict[event])
        # PlotEvent(event, eventdict[event])
        if i == 10:
            break
