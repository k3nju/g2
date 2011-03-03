#! /usr/bin/python
cxxflags = "-fpermissive -g -Wpointer-arith -pedantic -O2 -W -Wall -Wno-variadic-macros";

# src configuration
srcEnv = Environment( CXXFLAGS = cxxflags );
Export( "srcEnv" );

# test configuration
testEnv = Environment( CXXFLAGS = cxxflags );
Export( "testEnv" );

def Build():
	srcEnv.SConscript( "src/SConscript", variant_dir="./build" );	
	if "test" in COMMAND_LINE_TARGETS:
		testEnv.SConscript( "test/SConscript" );
		
Build();

