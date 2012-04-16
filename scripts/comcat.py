#!/usr/bin/env   python

baudrate = 115200
default_port = "/dev/ttyUSB1"
default_file = 'wiseos-test-msp430.exe'

from optparse import OptionParser
usage = """usage: %prog [-b]"""
parser = OptionParser(usage)
parser.add_option("-b", "--binary",
				  action="store_true", dest="binary", help="print binary data as numbers")
parser.add_option("-p", "--port",
				  action="store", dest="port", help="input port", default=default_port)
parser.add_option("-f", "--file",
				  action="store", dest="file", help="wiseos execute file", default=default_file)

(options, args) = parser.parse_args()

import sys
import serial
import time
import elf

BEGIN_MARKER = 0xeb
ESC_MARKER = 0xda
XOR_MARKER = 0x19
buffer = []
xor_char = 0
dbgstr = {}

def prt_num( buf, pos, len, hex ):
    num = 0
    for k in xrange(0,len):
        num |= buf[pos + k] << 8*k
    if hex == 0:    
        sys.stdout.write("%d"%num)
    else:
        sys.stdout.write("%x"%num)

def dbg_prt( buf ):
    i = 3
    s = dbgstr[ ( buf[ i+1 ]<<8 )|buf[ i ] ]
    i = i + 2
    j = 0
    while j < len(s):
        if '%hhx' == s[j:j+len('%hhx')]:
            prt_num( buf, i, 2, 1 )
            i += 2
            j += len('%hhx')
        elif '%x' == s[j:j+len('%x')]:
            prt_num( buf, i, 2, 1 )
            i += 2
            j += len('%x')
        elif '%lx' == s[j:j+len('%lx')]:
            prt_num( buf, i, 4, 1 )
            i += 4
            j += len('%lx')
        elif '%llx' == s[j:j+len('%llx')]:
            prt_num( buf, i, 8, 1 )
            i += 8
            j += len('%llx')
        elif '%hhd' == s[j:j+len('%hhd')]:
            prt_num( buf, i, 2, 0 )
            i += 2
            j += len('%hhd')
        elif '%d' == s[j:j+len('%d')]:
            prt_num( buf, i, 2, 0 )
            i += 2
            j += len('%d')
        elif '%ld' == s[j:j+len('%ld')]:
            prt_num( buf, i, 4, 0 )
            i += 4
            j += len('%ld')
        elif '%lld' == s[j:j+len('%lld')]:
            prt_num( buf, i, 8, 0 )
            i += 8
            j += len('%lld')
        else:
            sys.stdout.write(s[j])
            j += 1
    sys.stdout.write('\n')

try:

    elf = elf.ELFObject()
    elf.fromFile(open(options.file))

    s = ''
    str_addr = elf.getSection('.dbgstr').sh_addr
    cur_addr = str_addr

    for i in elf.getSection('.dbgstr').data:
        if  0 == ord(i):
            if s <> '':
                dbgstr[ str_addr ] = s
                s = ''
        else:
            if s == '':
                str_addr = cur_addr
            s += i
        cur_addr = cur_addr + 1
        
    port = serial.Serial(port=options.port, baudrate=baudrate, bytesize=8, parity='N', stopbits=1, timeout=1, xonxoff=0, rtscts=0)

    while(port.isOpen()):
        t = int(time.time())
        data = port.read(1)
        t = int(time.time()) - t
        if not data:
            if t<1:
                break
            else:
                continue
        if options.binary:
            sys.stdout.write("%02X"%ord( data )+" ")
            sys.stdout.flush()
            continue

        data = ord(data)

        if BEGIN_MARKER == data:
            buffer = [ data ]
            xor_char = 0
            continue
        if len( buffer ) == 0:
            continue
        if ESC_MARKER == data:
            xor_char = XOR_MARKER
            continue
       
        data ^= xor_char
        xor_char = 0
        buffer += [ data ]

        if len(buffer) <= 2 or len(buffer) < buffer[1]:
            continue

        csum = 0
        for i in buffer[:-1]:
            csum += i
        csum = csum % 256    

        if csum != buffer[-1]:
            print "control sum is bad: csum="+str(csum)+", buffer[-1]="+str( buffer[-1])
            buffer = []
            continue

        #print buffer
        dbg_prt( buffer )
        buffer = []


except KeyboardInterrupt:
	pass
except Exception, exc:
	print exc
		
