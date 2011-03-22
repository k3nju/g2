#! /usr/bin/python
import os;

cxxflags = "-fpermissive -g -Wpointer-arith -pedantic -O2 -W -Wall -Wno-variadic-macros";

# src configuration
src_env = Environment( CXXFLAGS = cxxflags );
Export( "src_env" );

# test configuration
test_env = Environment( CXXFLAGS = cxxflags );
Export( "test_env" );

# source directory
src_dir = os.getcwd() + "/src";
Export( "src_dir" );

def Build():
	SConscript( dirs="src", variant_dir="./build", src_dir="./src" );
	if "test" in COMMAND_LINE_TARGETS:
		test_env.SConscript( "test/SConscript" );
		
Build();

