#!/bin/sh

#-----------------------------------------------------------------
CMDNAME=`basename $0`
BASE=`pwd`
VER=$(date '+0.%y.%m.%d')
#-----------------------------------------------------------------
BINDIR="xmil-$VER-dc-sd-plainfiles"




make_bin() {

	if [ ! -d $BINDIR ];then
		mkdir -p disttmp/$BINDIR
	fi 

	cp README disttmp/$BINDIR/
	cp IP.BIN disttmp/$BINDIR/
	cp XMIL.BIN disttmp/$BINDIR/
	cp 1ST_READ.BIN disttmp/$BINDIR/
	cd $BASE/disttmp/
	zip -r $BINDIR.zip $BINDIR
	cd $BASE
}

case $1 in
    all)
		rm -rf disttmp
		make_bin
		;;
    bin)
		make_bin
		;;
    *)
		echo "USAGE: $CMDNAME (all|bin)" 1>&2
		;;
esac
