#!/bin/env python

import os
import sys

#######################################
# OPTIONS
#######################################
opts = Options(ARGUMENTS)

# conditional features
opts.Add(BoolOption('use_env', 'Use environment variables (CC, CXX, CCFLAGS, CXXFLAGS, CPPPATH, LIBPATH, QTDIR)', 0));

# paths
opts.Add(('prefix', 'Install prefix', ''));
opts.Add(('bindir', 'Path (relative to prefix) where to install executables to (default bin)', 'bin'));
opts.Add(('datadir', 'Path (relative to prefix) where to install read-only data to (default share/gaia)', 'share/gaia'));

#######################################
# ENVIRONMENT & SYSTEM_SPECIFIC CONFIGURATION
#######################################
env = Environment( options = opts )

env.ParseConfig('sdl-config --cflags --libs')
env.Append(LIBS = [ 'SDL_image', 'SDL_gfx' ])

if env['prefix'] == "":
	env['prefix'] = '.'
	env['bindir'] = '.'
	env['datadir'] = 'data'

SConsignFile()

if int(env['use_env']):
	for key in ['CC', 'CCFLAGS', 'CXX', 'CXXFLAGS', 'CPPPATH', 'LIBPATH', 'QTDIR' ]:
		if os.environ.has_key(key):
			env.Replace( **{key: os.environ[key].split(' ')} )

#######################################
# TARGETS
#######################################
srcs = Split("""main.c
		sprites.c
		error.c""")


env.Program('zenitka', [srcs])
