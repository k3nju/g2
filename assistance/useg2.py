#! /usr/bin/python

import os
import sys;

libpath = "/home/kj/devel/g2/build/";
srcpath = "/home/kj/devel/g2/src/";
params = "-lpthread -L%s -lg2 -I%s" % ( libpath, srcpath );

if __name__ == "__main__":
	if len( sys.argv ) < 2:
		sys.stderr.write( "usage:#useg2 <source file>\n" );
		sys.exit( -1 )

	oscmd = "g++ -g %s %s && ./a.out" % ( " ".join( sys.argv[1:] ), params,  );
	print oscmd;
	os.system( oscmd );
