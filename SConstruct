#! /usr/bin/python
# -*- coding:utf-8 -*-

import os;

# definitions

TARFLAGS = [ "-c", "-z" ];

# exports
CXXFLAGS = [
	"-g",
	"-O2",
	"-fpermissive",
	"-Wpointer-arith",
	"-pedantic",
	"-Wall",
	"-Wno-variadic-macros",
	];
Export( "CXXFLAGS" );

TOP_DIR = os.getcwd() + "/";
Export( "TOP_DIR" );

SRC_DIR = TOP_DIR + "src/";
Export( "SRC_DIR" );

VAR_DIR = TOP_DIR + "build/";
Export( "VAR_DIR" )

ENV = Environment( CXXFLAGS = CXXFLAGS,
				   TARFLAGS = TARFLAGS );
Export( "ENV" );


# functions

# build libg2
def build_libg2():
	libg2 = ENV.SConscript( dirs = SRC_DIR, variant_dir= VAR_DIR );
	ENV.Alias( "libg2", libg2 );
	return libg2;

# make tar.gz
def make_tar_gz():
	ENV.Tar( "g2.tar.gz", SRC_DIR );

# make dummy install
def make_dummy_install():
	libg2 = ENV.SConscript( dirs = SRC_DIR, variant_dir= VAR_DIR );
	dum_dir = TOP_DIR + "dummy_install/";
	libg2_target = dum_dir + "libg2.a";
	ENV.InstallAs( libg2_target, [ libg2 ] );
	ENV.Alias( "install", libg2_target );
	for src in Glob( SRC_DIR + "*.h" ):
		basename = os.path.basename( src.get_path() );
		dst = dum_dir + "g2/" + basename;
		ENV.InstallAs( dst, src );
		ENV.Alias( "install", dst );

# main
Default( build_libg2() );
make_tar_gz();
make_dummy_install();

# print summary
print "[*] BUILD_TARGETS: " + " ".join( map( str, BUILD_TARGETS ) );
