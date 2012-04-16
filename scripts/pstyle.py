#!/usr/bin/env python

import os 
import sys 

options = '-npro -kr -nut -i4 -ts4 -sob -l80 -ss -ncs ' 
os.system('indent ' + options + ' '.join(sys.argv[1:]))
