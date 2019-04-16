#!/bin/bash
# Copyright (c) 2017-2019 Thought Networks LLC
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

set -e
set -x

BASEDIR=$(pwd)

if [ -f Makefile ]; then
	make distclean
fi

if [ "x${HOST}y" = "xy" ]; then
	export HOST="$(./depends/config.guess)"
fi

cd depends
make
cd ..

if [ '!' -f configure ]; then
	./autogen.sh
fi

export CONFIG_SITE="$BASEDIR/depends/$HOST/share/config.site"
./configure --prefix=/ $@
