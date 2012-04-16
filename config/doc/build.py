import sys
from pymake import Task, run_task, ReplacementHelper, InFileWriter

os_dir =  __file__[:__file__.rfind('/config')]

doxygen = 'doxygen'
rm = 'rm'
rar = 'rar'

repld = {'os_dir': os_dir, doxygen : 'doxygen', 'rm': rm, 'rar': rar }
repl = ReplacementHelper(sys.argv, repld)

doc = Task('build docs',repl=repl,  workdir = '%(os_dir)s', target = '%(os_dir)s/doc/html', commands = ['%(doxygen)s'] )
pack_doc = Task('pack docs',repl=repl, workdir = '%(os_dir)s/doc', target = '%(os_dir)s/doc/html.rar', 
    commands = ['%(rar)s a -r -m5 html.rar html'], dependencies = ( doc ) )
unpack_doc = Task('unpack docs',repl=repl, workdir = '%(os_dir)s/doc', target = '%(os_dir)s/doc/html', commands = ['%(rar)s x html.rar'])
clean_doc = Task('clean docs',repl=repl, workdir = '%(os_dir)s/doc/', commands = ['%(rm)s -rf html html.rar'])

build_all = Task( 'Build WiseOS docs',repl=repl, dependencies = ( pack_doc ) )
clean_all = Task( 'Clean WiseOS docs',repl=repl, dependencies = ( clean_doc ) )

all     = build_all
clean   = clean_all

