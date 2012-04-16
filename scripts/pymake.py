#############################################################################
#
# Copyright (c) 2004 Chris McDonough
#
# This software is subject to the provisions of the Plope License
# Version 1.0.  A copy of the PL should accompany this distribution.
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY AND ALL EXPRESS OR IMPLIED
# WARRANTIES ARE DISCLAIMED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF TITLE, MERCHANTABILITY, AGAINST INFRINGEMENT, AND FITNESS
# FOR A PARTICULAR PURPOSE
#
##############################################################################

""" $Id: pymake.py,v 1.13 2006/08/08 18:15:41 chrism Exp $ """

import UserDict
import types
import os
import warnings
import sys

class PyMakeError(Exception):
    pass

class PyMakeTargetNotCreated(PyMakeError):
    pass

VERBOSE = 0
STRICT_TARGET_CHECKING = 1
MAKEFILE_TIMESTAMP_CHECKING = 0
FALSE_CONDITION = ()
FALSE_TARGETS = (FALSE_CONDITION,)

def postorder(startnode):
    """ Postorder depth-first traversal of the dependency graph implied
    by startnode and its children """
    seen = {}
    def visit(node):
        seen[node] = True
        for child in node.dependencies:
            if not isinstance(child, Task):
                raise TypeError, ("Can't use non-Task %r as a dependency"
                                  % child)
            if child not in seen:
                for result in visit(child):
                    yield result
        yield node
    return visit(startnode)

def run_task(task):
    """ Run the task and all of its dependents in dependency order """
    error('run_task starting with root task named "%s"' % task.getName())
    tasks = postorder(task)
    for task in tasks:
        if task.hasCommands() and task.needsCompletion():
            task.attemptCompletion()
    error('done')
        
class SystemWrapper:
    def __init__(self, command, repl):
        self.command = command
        self.repl = repl

    def __call__(self):
        return os.system(self.command % self.repl)

    def __repr__(self):
        return self.command % self.repl

class Task:
    def __init__(self, name, repl=None, target=(), dependencies=(),
                 commands=(), workdir=None, targets=(), mglobals=None,
                 condition=None):
        # competing 'target' and 'targets' in the kw list is used for backwards
        # compatibility (pymake <= 0.9 could only accept a single target)
        self.name = name
        if target:
            self.targets = sequence_helper(target)
        else:
            self.targets = sequence_helper(targets)
        self.dependencies = sequence_helper(dependencies)
        self.workdir = workdir
        if repl is None:
            self.repl = {}
        else:
            self.repl = repl
        self.condition = condition
        self.commands = command_helper(sequence_helper(commands), self.repl)
        if mglobals is None:
            # Get a hold of caller's globals around so we can figure out
            # whether the source file has changed or not.  This is sneaky
            # but it's more convenient than needing to explicitly pass in
            # globals to each task's constructor.
            caller_globals = sys._getframe(1).f_globals
        else:
            caller_globals = mglobals
        makefile = caller_globals.get('__file__', sys.argv[0])
        if makefile.endswith('pyc') or makefile.endswith('.pyo'):
            makefile = makefile[:-1]
        self.makefile = os.path.abspath(makefile)

    # getters

    def getMakefile(self):
        return self.makefile

    def getWorkDir(self):
        if self.workdir:
            return self.interpolate(self.workdir)
        return None

    def getTarget(self):
        # b/w compatibility
        targets = self.getTargets()
        if targets:
            return targets[0]

    def getTargets(self):
        targets = []
        if self.condition is not None:
            if not self.condition(self.repl):
                return FALSE_TARGETS
                
        if self.targets:
            for target in self.targets:
                target =  self.interpolate(target)
                workdir = self.getWorkDir()
                if workdir:
                    if target and workdir and not os.path.isabs(target):
                        target = os.path.join(workdir, target)
                targets.append(target)
        return targets

    def getName(self):
        if self.name:
            return self.interpolate(self.name)
        return None

    # helpers
    
    def __repr__(self):
        return '<%s %s named "%s">' % (self.__class__, id(self), self.name)

    def interpolate(self, s):
        if isinstance(s, types.StringType):
            return s % self.repl
        if hasattr(s, 'interpolated'):
            return s.interpolated(repl)
        return s

    # meat

    def needsCompletion(self):

        """ If our target doesnt exist or any of our dependency's
        targets are newer than our target, return True, else return
        False """

        name = self.getName()

        if not self.targets:
            # if we don't have a target, we always need completion
            if VERBOSE:
                error('"%s" has a null set of targets' % name)
            return 1

        if self.condition is not None:

            if not self.condition(self.repl):
                error('"%s" had a condition which prevents this target from '
                      'needing completion' % name)
                return 0

        missing = self.missingTargets()
        if missing:
            errormsg = []
            for target in missing:
                # if one of our targets doesnt exist we definitely need
                # completion
                errormsg.append('"%s" is missing target %s' % (name, target))
            error('\n'.join(errormsg))
            return 1
        # otherwise, we *might* need to recomplete if one of our
        # dependencies' target's timestamps is newer than our target's
        # timestamp
        if self.dependencyTargetIsNewer():
            error('"%s" has a dependency with a newer target' % name)
            return 1
        if MAKEFILE_TIMESTAMP_CHECKING:
            olders = self.targetsOlderThanMakefile()
            if olders:
                errormsg = []
                for older in olders:
                    errormsg.append(
                        'makefile "%s" is newer than target "%s" of task "%s"'
                        % (self.makefile, older, self.getName())
                      )
                error('\n'.join(errormsg))
                return 1
        return 0

    def targetsOlderThanMakefile(self):

        """ If the makefile in which the task instance has been defined
        is newer than our target, return true """

        name = self.getName()
        targets = self.getTargets()
        makefile = self.getMakefile()
        olders = []
        for target in targets:
            if os.path.getmtime(makefile) > os.path.getmtime(target):
                olders.append(target)
        return olders

    def dependencyTargetIsNewer(self):
        
        """ If any of our dependencies' targets are newer than our
        target, return True, else return False """

        targets = self.getTargets()
        name = self.getName()

        for dep in self.dependencies:
            deptargets = dep.getTargets()
            depname = dep.getName()

            if not deptargets:
                # dependencies with no target are always newer, shortcut
                if VERBOSE:
                    error('%s: dependency "%s" has a null target set so is '
                          'newer'% (name, depname)
                          )
                return 1

            missingdeptarget = None
            for deptarget in deptargets:
                if deptarget is not FALSE_CONDITION:
                    if not os.path.exists(deptarget):
                        # nonexistent dependency target, it will definitely need
                        # completion
                        error('%s: "%s" missing dependency target %s' %
                              (name, depname, deptarget)
                              )
                        missingdeptarget = 1

            if missingdeptarget:
                return 1

            newerdeptarget = None
            for deptarget in deptargets:
                for target in targets:
                    if (target is not FALSE_CONDITION and
                        deptarget is not FALSE_CONDITION):
                        if os.path.getmtime(deptarget) > os.path.getmtime(
                            target):
                            error('%s: dependency "%s" has a newer tgt "%s"' %
                                  (name, depname, deptarget)
                                  )
                            newerdeptarget = 1
            if newerdeptarget:
                return 1

        return 0

    def attemptCompletion(self):

        """ Do the work implied by the task (presumably create the
        target file)"""

        name = self.getName()

        old_workdir = os.getcwd()
        try:
            error('executing %s' % name)
            workdir = self.getWorkDir()
            if workdir:
                os.chdir(workdir)
                if VERBOSE:
                    error('changed working directory to %s' % workdir)
            for command in self.commands:
                error('running "%s"' % command)
                status = command()
                if status:
                    raise PyMakeError, (
                        'Task "%s": command "%s" failed with status code '
                        '"%s"' % (name, command, status)
                        )
        finally:
            os.chdir(old_workdir)
            if VERBOSE:
                error('reset working directory to %s' % old_workdir)
        if not self.getTargets():
            # it's completed if we don't have a target
            return 1
        if STRICT_TARGET_CHECKING:
            missing_targets = self.missingTargets()
            if missing_targets:
                errormsg = []
                for missing in missing_targets:
                    errormsg.append('target of %s (%s) was not created and '
                        'STRICT_TARGET_CHECKING is turned on' % (name,
                                                                 missing)
                        )
                raise PyMakeTargetNotCreated('\n'.join(errormsg))
            for target in self.getTargets():
               if target is not FALSE_CONDITION:
                   os.utime(target, None)
                   
        return not self.needsCompletion()

    def missingTargets(self):

        """ Check for the existence of our target files """

        targets = self.getTargets()
        missing = []

        for target in targets:
            exists = os.path.exists(target)
            if not exists:
                missing.append(target)

        return missing

    def hasCommands(self):
        return not not self.commands

def error(s):
    sys.stderr.flush()
    sys.stderr.write('pymake: ' + str(s) + '\n')
    sys.stderr.flush()
    
def sequence_helper(val):
    if not isinstance(val, (types.ListType, types.TupleType)):
        val = [val]
    return list(val)

def command_helper(seq, repl):
    L = []
    for item in seq:
        if isinstance(item, types.StringTypes):
            # if the item is a string, it's a shell command
            L.append(SystemWrapper(item, repl))
        else:
            # otherwise its assumed to be a callable
            L.append(item)
    return L


class ReplacementHelper(UserDict.UserDict, UserDict.DictMixin):
    """A mapping that allows initialization from a list of strings
    representing keyword arguments.

    The first item in the list is ignored (commonly the list is
    sys.argv).  The remaining items are parsed for equal signs; if one
    is present, the right side is set as a string value in self.data;
    if not, the item is appended to self.opts.

    The optional second argument is a dictionary that initializes self.data.
    """
    def __init__(self, argv, d=None):
        if d is None:
            self.data = {}
        else:
            self.data = d
        # BBB compatibility for code that meddles with self.d.
        self.d = self.data
        opts = []
        for arg in argv[1:]:
            if '=' in arg:
                k, v = arg.split('=', 1)
                self[k.strip()] = v.strip()
            else:
                opts.append(arg)
        self.opts = opts

    def copy(self):
        rh = ReplacementHelper([], d=self.data.copy())
        rh.opts = self.opts[:]
        return rh


class InFileWriter:
    """Helper class that reads *infile*, and writes it to *outfile*
    with string interpolation values obtained from the *repl* mapping.

    outfile is created with permissions *mode* (default 0755).
    """
    def __init__(self, infile, outfile, repl, mode=0755):
        self.infile = infile
        self.outfile = outfile
        self.repl = repl
        self.mode = mode

    def __call__(self):
        text = open(self.infile % self.repl).read()
        for k, v in self.repl.items():
            if isinstance(v, types.StringType):
                text = text.replace('<<%s>>' % k.upper(), v)
        outfile = open(self.outfile % self.repl, 'w')
        outfile.write(text)
        outfile.close()
        os.chmod(self.outfile % self.repl, self.mode)

    def __repr__(self):
        return '<pymake.InFileWriter instance for %s>' % self.outfile

