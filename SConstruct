#!/usr/bin/env python
EnsureSConsVersion(4, 0, 0)
EnsurePythonVersion(3, 10, 12)

# Import locals
import atexit
import pickle
import sys
import os
import glob 
import time

opts = Variables()

env = Environment(tools=['default', 'gcc'], CC='gcc', CXX='g++', CCFLAGS='-Wall -O2',variables=opts)
env.PrependENVPath("PATH", os.getenv("PATH"))
env.PrependENVPath("PKG_CONFIG_PATH", os.getenv("PKG_CONFIG_PATH"))

# Define the target platform 
platform = ['linux', 'windows', 'macos']


opts.Add(BoolVariable('debug', 'Enable debug mode', False))
opts.Add(BoolVariable("debug_symbols", "Build with debugging symbols", False))
opts.Add(BoolVariable("separate_debug_symbols", "Extract debugging symbols to a separate file", False))
opts.Add(BoolVariable('opengl2', 'Enable OpenGL 2', False))
opts.Add(BoolVariable('opengl3', 'Enable OpenGL 3', True))
opts.Add(BoolVariable('windows', 'Target Windows platform', False))
opts.Add(BoolVariable('linux', 'Target Linux platform', True))  # Default to Linux
opts.Add(BoolVariable('macos', 'Target macOS platform', False))


def add_source_files(env , source_file):
    env.Append(SRC=[source_file])


def add_library(name, sources):
    env.Append(LIBS=[name])

def add_sub_dir(name):
    sources = Glob(os.path.join(name, '*.c'))
    env.Append(SRC=sources)


env.AddMethod(add_source_files, 'add_source_files')
env.AddMethod(add_library, 'add_library')
env.AddMethod(add_sub_dir, 'add_sub_dir')

# Export the environment globally.
Export('env')

# Include the SCsub file in the source directory.
SConscript('core/SCsub')
SConscript('servers/SCsub')
SConscript('editor/SCsub')
SConscript('scene/SCsub')
SConscript('main/SCsub')
SConscript('shaders/SCsub')
SConscript('pyshics/SCsub')
SConscript('plugins/SCsub')
SConscript('platform/SCsub')



