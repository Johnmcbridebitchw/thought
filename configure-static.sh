#!/bin/bash
# Copyright (c) 2017-2018 Thought Networks LLC
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

echo_run() {
	echo - $@
	$@
	return $?
}

BASEDIR=$(pwd)

if [ -f Makefile ]; then
	echo_run make distclean
fi

if [ "x${HOST}y" = "xy" ]; then
	echo_run export HOST="$(./depends/config.guess)"
fi

echo_run cd depends
echo_run make
echo_run cd ..

if [ '!' -f configure ]; then
	echo_run ./autogen.sh
fi

echo - env "CONFIG_SITE=$BASEDIR/depends/$HOST/share/config.site" "LDFLAGS=-static-libgcc -static-libstdc++" ./configure --prefix=/
env "CONFIG_SITE=$BASEDIR/depends/$HOST/share/config.site" "LDFLAGS=-static-libgcc -static-libstdc++" ./configure --prefix=/ $@

