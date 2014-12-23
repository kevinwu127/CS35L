#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def chooseline(self):
        return random.choice(self.lines)

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 [FILE2] 

Output randomly selected lines from FILE(S). 
Files must be non-empty.

OPTIONS: 
    -n: number of lines to be printed
    -u: unique
    -w: without replacement
"""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--numlines",
                      action="store", dest="numlines", default=1,
                      help="output NUMLINES lines (default 1)")

    parser.add_option("-u", "--unique",
                      action="store_true", dest="unique", default=False,
                      help="Deletes duplicate lines")

    parser.add_option("-w", "--without-replacement",
                      action="store_true", dest="replace", default=False,
                      help="without replacement option")
    options, args = parser.parse_args(sys.argv[1:])
    
    try:
        numlines = int(options.numlines)
        unique = bool(options.unique)
        replace = bool(options.replace)
    except:
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines))
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))
    if len(args) < 1:
        parser.error("wrong number of operands")


    
    # Append for multiple files
    AppendedList=[]
    for i in range(len(args)):
        try:
            myFile=open(args[i],'r')
            aLine=myFile.readlines()

        except IOError:
            parser.error("File not found")
        
        if aLine:
            # append a newline char '\n' if necessary
            if aLine[-1][-1]!='\n':
                aLine[-1]=aLine[-1]+'\n'
        
                #append line to whole file
            AppendedList=AppendedList+aLine
        myFile.close()

    if not AppendedList:
        parser.error("No lines were provided.")

    # "-u"
    if unique == True:
        #remove duplicates
        AppendedList=list(set(AppendedList))


    #w+ option to create a file with the lines that we wanted 
    tempfile=open("A_New_File.txt",'w+')
    tempfile.writelines(AppendedList)
    tempfile.close()


    input_file = "A_New_File.txt"


    
    try:
        generator = randline(input_file)
        # "-w" option
        if replace == True:
            #lines requested is bigger than lines present
            if numlines > len(AppendedList):
                print('Numlines should be <= {0}.'.format(len(AppendedList))) 
                sys.exit()

            for i in range(numlines):
                lineToBeReplaced=generator.chooseline()
                sys.stdout.write(lineToBeReplaced)
                generator.lines.remove(lineToBeReplaced)

        else:  #with replacement allowed
            for i in range(numlines):
                sys.stdout.write(generator.chooseline())

    except IOError as err:
        (errno, strerror) = err.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

if __name__ == "__main__":
    main()
