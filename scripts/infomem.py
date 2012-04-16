#!/usr/bin/env python

import sys
import _parjtag
from optparse import OptionParser

VERSION = "0.1"
DEBUG = 1
LPT = "/dev/parport0"
DEFAULT_CONFIG = "infomem.cfg"

#frame specific consts
ERASE_MASS  = 2
ERASE_MAIN  = 1
ERASE_SGMT  = 0

SEGA_ADDR = 0x1080
SEGB_ADDR = 0x1000

vars = {}
new_vars = {}

usage = """usage: %prog [-c configname.cfg ] var0=value0 .. varN=valueN"""
parser = OptionParser(usage)
parser.add_option("-c", "--config", action="store", dest="config", help="configuration file", default = DEFAULT_CONFIG )
(options, args) = parser.parse_args()

print args

class JTAG:
    """wrap the _parjtag extension"""

    def __init__(self):
        self.showprogess = 1
        self.data = None

    def open(self, lpt=None):
        """open specified or default port"""
        if lpt is None:
            _parjtag.open()
        else:
            _parjtag.open(lpt)
       
    def connect(self, lpt=None):
        """connect to specified or default port"""
        if lpt is None:
            _parjtag.connect()
        else:
            _parjtag.connect(lpt)

    def close(self):
        """release JTAG"""
        _parjtag.release()

    def uploadData(self, startaddress, size):
        """upload a datablock"""
        return _parjtag.memread(startaddress, size)

    def downloadData(self, startaddress, data ):
        """download a datablock"""
        return _parjtag.memwrite(startaddress, data )

    def actionReset(self):
        """perform a reset"""
        _parjtag.reset(0, 0)

    def eraseSegment(self, address, length ):
        """Selective segment erase"""
        _parjtag.memerase( ERASE_SGMT, address, length )

def hexdump( (adr, memstr) ):
    """Print a hex dump of data collected with memread
    arg1: tuple with adress, memory
    return None"""
    count = 0
    ascii = ''
    for value in map(ord, memstr):
        if not count: print "%04x: " % adr,
        print "%02x" % value,
        ascii += (32 <= value < 128) and chr(value) or '.'
        count += 1
        adr += 1
        if count == 16:
            count = 0
            print "  ", ascii
            ascii = ''
    if count < 16: print "   "*(16-count), " ", ascii

def readConf( confFile ):
    fd = open( confFile, "r")
    s = fd.readline()
    while s:
        s = s[:-1]
        l = s.split(':')
        if len(l) == 3:
            vars[ l[0] ] = [ int(l[1],16), int(l[2]) ]
        s = fd.readline()
    fd.close()

def parseArgs():
    for i in args:
        l = i.split('=')
        if len(l) == 2:
            new_vars[l[0]] = int(l[1])

def changeVars( data, addr ):
    start_addr = addr
    end_addr = start_addr + len(data)
    for i in new_vars.keys():
        if start_addr <= vars[i][0] and vars[i][0] < end_addr:
            offset = vars[i][0] - start_addr
            for j in xrange(vars[i][1]):
                data[ offset+j ] = chr(new_vars[i] % 256)
                new_vars[i] >>= 8

if __name__ == '__main__':
    try:
        jtag    = JTAG()       
        lpt     = LPT
        dataA   = []
        dataB   = []

        sys.stderr.write("Info memory programmer for msp430 Version: %s\n" % VERSION)

        sys.stderr.write("Read configuration %s ... " % options.config )
        readConf( options.config )
        parseArgs()
        sys.stderr.write("ok\n")
        sys.stderr.flush()

        sys.stderr.write("Open LPT port: %s ... " % lpt )
        sys.stderr.flush()
        jtag.open(lpt)
        sys.stderr.write("ok\n")

        sys.stderr.write("Connect to %s ... " % lpt )
        sys.stderr.flush()
        jtag.connect()
        sys.stderr.write("ok\n")        

        sys.stderr.write("Reset msp430 ... ")
        sys.stderr.flush()
        jtag.actionReset()
        sys.stderr.write("ok\n")

        sys.stderr.write("Read info mememory segment B [%x] ... " % SEGB_ADDR )
        dataB = list( jtag.uploadData( SEGB_ADDR, 128 ) )
        sys.stderr.flush()
        sys.stderr.write("ok\n")

        if 0 < DEBUG:
            hexdump( (SEGB_ADDR, dataB) )
            sys.stdout.flush()

        sys.stderr.write("Read info mememory segment A [%x] ... " % SEGA_ADDR )
        dataA = list( jtag.uploadData( SEGA_ADDR, 128 ) )
        sys.stderr.flush()
        sys.stderr.write("ok\n")

        if 0 < DEBUG:
            hexdump( (SEGA_ADDR, dataA) )
            sys.stdout.flush()

        sys.stderr.write("Write new values ... ")
        """Change dataA and dataB"""
        changeVars( dataA, SEGA_ADDR )
        changeVars( dataB, SEGB_ADDR )
        sys.stderr.write("ok\n")
        sys.stderr.flush()
         
        if 0 < DEBUG:
            hexdump( (SEGB_ADDR, dataB) )
            hexdump( (SEGA_ADDR, dataA) )
            sys.stdout.flush()

        sys.stderr.write("Erase info mem segments ... ")
        jtag.eraseSegment( SEGA_ADDR, 128 )
        jtag.eraseSegment( SEGB_ADDR, 128 )
        sys.stderr.write("ok\n");

        sys.stderr.write("Write info memory segments ...")
        jtag.downloadData( SEGA_ADDR, "".join(dataA) )
        jtag.downloadData( SEGB_ADDR, "".join(dataB) )
        sys.stderr.write("ok\n")
        sys.stderr.flush()

        sys.stderr.write("Reset and release msp430 ... ")
        sys.stderr.flush()
        _parjtag.reset(1, 1 )
        jtag.close()
        sys.stderr.write("ok\n")
        sys.stderr.write("Good Night, and Good Luck\n")

    except SystemExit:
        raise
    except KeyboardInterrupt:
        sys.stderr.write("User abort.\n");
        sys.exit(1)
    except Exception, msg:
        sys.stderr.write("\nAn error occoured:\n%s\n" % msg )
        sys.exit(1)


