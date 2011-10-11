#! /usr/bin/python

import subprocess;
import sys;
import re;
import os;
import signal;

re_guts = re.compile( "<guts>(.*?)</guts>", re.M | re.I | re.S );
re_entry = re.compile( "#ifdef GENSIN__GUTS__ID__(.+)" );
re_include = re.compile( "#include \"(.+?)\"" );

here = os.getcwd() + "/";
unittestflags = [ "-DGENSIN__GUTS" ];
cxxflags = [ "-fpermissive", "-g", "-O2", "-Wall", "-DDEBUG" ];
libs = [ "g2", "pthread", "rt", "gtest" ];
libpath = [ "/home/hoge/g2/build/" ];
includepath = [ "/home/hoge/2/src/" ];
deps = set(); # dependency files
SConstructSource = """
here=%(here)s;
cxxflags=%(cxxflags)s;
unittestflags=cxxflags + " "  + %(unittestflags)s;
libs=%(libs)s;
libpath=%(libpath)s;
includepath=%(includepath)s;
deps=%(deps)s;
targets=set();
%(gutsSrc)s
deps = deps.union( targets );
slib = StaticLibrary( "guts.o", list(deps), CXXFLAGS=cxxflags, LIBS=libs, LIBPATH=libpath, CPPPATH=includepath, buid_dir=\"/tmp/\" );
mainunitobj = Object( %(unittestfile)s, CXXFLAGS=unittestflags, LIBS=libs, LIBPATH=libpath, CPPPATH=includepath );
Program( "guts", [ mainunitobj, slib ], CXXFLAGS=unittestflags, LIBS=libs, LIBPATH=libpath, CPPPATH=includepath, buid_dir=\"/tmp/\" );
""";

undef="""
objects.append( Object( %(unittestfile)s, CXXFLAGS=unittestflags, LIBS=libs, LIBPATH=libpath, CPPPATH=includepath ) );
"""

gutsMainData = """
#include <stdio.h>
#include "exception.h"
#include "debugutility.h"
int %(entry_point)s( int argc, char **argv );

int main( int argc, char **argv )
\t{
\tputs( "-----start" );
\tint result = 0;
\ttry{ result = %(entry_point)s( argc, argv ); }
\tcatch( g2::Exception &E ){ G2_EOUTPUT( E ); }
\tputs( "-----end");
\treturn result;
\t}
""";

sconsConfigFile = "/tmp/guts_sconstruct";
gutsMainFile = "/tmp/guts_main.cpp";
sconsCmd = "scons %s -f " + sconsConfigFile;
execute = "/tmp/guts";

def ParseInclude( srcFile ):
	print "ParseInclude";
	srcDir = os.path.dirname( srcFile ) + "/";
	alreadySearched = set();
	search = set( [ srcFile ] );

	while len( alreadySearched ) < len( search ):
		for f in search.difference( alreadySearched ):
			alreadySearched.add( f );
			
			data = None;
			if f == srcFile:
				data = open( f ).read();
			else:
				data = "".join( open( f ).readlines()[:20] );
				
			includes = re.findall( "\n#include \"(.+?)\"", data );
			
			print "%s includes" % f;
			for i in includes:
				print "  %s" % i;
				if os.path.exists( srcDir + i ) == True:
					search.add( srcDir + i );

	alreadySearched.remove( srcFile );
	return set( [ i.replace( ".h", ".cpp" ) for i in alreadySearched ] );

if __name__ == "__main__":
	if len( sys.argv ) < 2:
		print "guts.py <source file>";
		sys.exit( -1 );
		
	srcFile = os.path.abspath( sys.argv[1] );
	includepath.append( os.path.dirname( srcFile ) );

	os.chdir( "/tmp/" );
	if "-autoinclude" in sys.argv:
		deps = deps.union( ParseInclude( srcFile ) );
		print deps;
		sys.argv.remove( "-autoinclude" );

	args = " ".join( sys.argv[2:] );
		
	data = open( srcFile ).read();
	
	match = re_entry.search( data );
	if match == None:
		print "guts entry point not found";
		sys.exit( -1 );
	entry = match.group( 1 );
	deps.add( gutsMainFile );
	unittestflags.append( "-DGENSIN__GUTS__ID__" + entry );
	
	match = re_guts.search( data );
	gutsSrc = "";
	if match != None:
		gutsSrc = match.group( 1 );
		
	src = SConstructSource % ( { "gutsSrc"  : str( gutsSrc ),
								 "cxxflags" : repr( " ".join( cxxflags ) ),
								 "libs"     : str( libs ),
								 "libpath"  : str( libpath ),
								 "includepath" : str( includepath ),
								 "deps"  : str( deps ),
								 "here"     : repr( here ),
								 "unittestfile" : repr( srcFile.replace( ".h", ".cpp" ) ),
								 "unittestflags" : repr( " ".join( unittestflags ) ),} );
		
	f = open( sconsConfigFile, "wb" );
	f.write( src );
	f.close();

	f = open( gutsMainFile, "wb" );
	f.write( gutsMainData % ( { "entry_point" : "GutsEntry" + entry } ) );
	f.close();

	sconsCmd = sconsCmd % ( args );
	if subprocess.call( sconsCmd.split( " " ) ) != 0:
		sys.exit( -1 );
	if os.path.exists( execute ) == False:
		sys.exit( -1 );
	try:
		p = subprocess.Popen( execute, shell = True );
		print "PID:%d" % p.pid;
		os.waitpid( p.pid, 0 );
	except KeyboardInterrupt:
		#os.kill( p.pid, signal.SIGINT );
		os.waitpid( p.pid, 0 );

