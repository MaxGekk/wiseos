#!/usr/bin/env python

import os
import sys
    
os_dir = os.path.abspath(os.path.dirname( sys.argv[0] )) 
sys.path += [ os_dir+'/scripts']

if sys.argv[1:]:
    sys.path += [ os_dir+'/config/'+sys.argv[1] ]
else:
    sys.path += [ os_dir+'/config/msp' ]

from pymake import Task, run_task, ReplacementHelper, InFileWriter
from build import *

default = all

if __name__ == '__main__':
    opts = repl.opts
    if opts[1:]:
	task = locals()[opts[1]]
    else:
        task = default	
    run_task( task )

