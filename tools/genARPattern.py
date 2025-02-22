#!/usr/bin/python

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import sys, getopt, os, svgwrite

pageFormat = { 'a4' : ('210mm','297mm'), 'a4l' : ('280mm','297mm'), 'a3' : ('297mm','420mm'), 'a3l' : ('420mm','297mm')}

def help(name):
    """ print the usage for the program
    """
    print('Usage:\n\t'+name+' -i <directory containing the markers> -o <output file>')
    print("\nSupported languages:\nLanguage\t\ttoken")
    for i in suppLang:
        print(i.name+"\t\t\t"+i.token)

# use this instead
#https://docs.python.org/2/library/optparse.html#optparse-standard-option-actions

if __name__ == "__main__":  

    # size of the marker in cm
    size = 4
    # number of rows
    rows = 5
    # number of cols
    cols = 7
    # space between markers in cm
    gap = 1.5

    base = './arlibs/ARToolkitPlus/id-markers/simple/std-border/'

    dwg = svgwrite.Drawing('test.svg', size=pageFormat['a3l'], profile='tiny')

    for i in range(0, rows):
        for j in range(0, cols):
            filename = '%sSimpleStd_%03d.png' %(base, (i*cols+j))
            # print(filename)
            dwg.add(dwg.image(filename, insert=(str(gap+j*(size+gap))+'cm',str(gap+i*(size+gap))+'cm'), size=(str(size)+'cm',str(size)+'cm')))
    
    dwg.save()