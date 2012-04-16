import sys
from pymake import Task, run_task, ReplacementHelper, InFileWriter

mcu = 'msp430'
cc = 'msp430-gcc'
ld = 'msp430-ld'
ar = 'msp430-ar'
rm = 'rm'
debug = ''

os_dir =  __file__[:__file__.rfind('/config')]
os_include  = os_dir + '/include'
mcu_include = os_dir + '/hardware/mcu/include'
msp_dir = os_dir + '/hardware/mcu/msp430'

cflags = '-O1 -I '+ os_include + ' -I ' + mcu_include
cflags += ' -nostdlib -nostdinc -mdisable-hwmul -mmcu=msp430x1611 ' + ' -I '+msp_dir

repld = {'os_dir': os_dir,'mcu': mcu,'cc': cc, 'ld': ld, 'ar': ar, 'rm': rm, 'cflags': cflags,'debug': debug }
repl = ReplacementHelper(sys.argv, repld)

build_kernel = Task('build kernel',repl=repl, workdir = '%(os_dir)s/kernel', commands = ['%(cc)s -c %(cflags)s %(debug)s *.c'])
clean_kernel = Task('clean kernel',repl=repl, workdir = '%(os_dir)s/kernel', commands = ['%(rm)s -f *.o'])

build_lib = Task('build library',repl=repl, workdir = '%(os_dir)s/lib',target = '%(mcu)s-libs.a',
    commands = ['%(cc)s -c %(cflags)s %(debug)s *.c', '%(ar)s -q -s %(mcu)s-libs.a *.o ' ] )
clean_lib = Task('clean library',repl=repl, workdir = '%(os_dir)s/lib', commands = [ '%(rm)s -f *.o *.a' ])

build_mculib = Task('build mcu library',repl=repl, workdir = '%(os_dir)s/hardware/mcu/%(mcu)s/lib', 
    target = '%(mcu)s-hwlibs.a', commands = [ '%(cc)s -c %(cflags)s %(debug)s *.c', '%(ar)s -q -s %(mcu)s-hwlibs.a *.o'])
clean_mculib = Task('clean mcu library',repl=repl, workdir = '%(os_dir)s/hardware/mcu/%(mcu)s/lib', 
    commands = ['%(rm)s -f *.o *.a'])

build_mcu = Task('build mcu',repl=repl, workdir = '%(os_dir)s/hardware/mcu/%(mcu)s', 
    commands = [ '%(cc)s -c %(cflags)s %(debug)s *.c *.S'])
clean_mcu = Task('clean mcu',repl=repl, workdir = '%(os_dir)s/hardware/mcu/%(mcu)s', 
    commands = [ '%(rm)s -f *.o' ])

build_all = Task( 'Build WiseOS',repl=repl, workdir = os_dir, target = 'wiseos-%(mcu)s.exe',
    commands = ['%(cc)s %(cflags)s -Xlinker -T %(os_dir)s/config/msp/os.ldx '+ 
    '%(os_dir)s/kernel/*.o %(os_dir)s/hardware/mcu/%(mcu)s/*.o '+
    ' %(os_dir)s/lib/%(mcu)s-libs.a %(os_dir)s/hardware/mcu/%(mcu)s/lib/%(mcu)s-hwlibs.a '+
    ' -o wiseos-%(mcu)s.exe'],
    dependencies = ( build_kernel, build_lib, build_mcu, build_mculib, ) )

clean_all = Task( 'Clean WiseOS',repl=repl, workdir = os_dir, commands = [ '%(rm)s -f *.o *.exe' ],
    dependencies = ( clean_kernel , clean_mcu, clean_mculib, clean_lib ) )

all     = build_all
clean   = clean_all

