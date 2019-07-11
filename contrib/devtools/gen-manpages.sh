#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

THOUGHTD=${THOUGHTD:-$SRCDIR/thoughtd}
THOUGHTCLI=${THOUGHTCLI:-$SRCDIR/thought-cli}
THOUGHTTX=${THOUGHTTX:-$SRCDIR/thought-tx}
THOUGHTQT=${THOUGHTQT:-$SRCDIR/qt/thought-qt}

[ ! -x $THOUGHTD ] && echo "$THOUGHTD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BTCVER=($($THOUGHTCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for thoughtd if --version-string is not set,
# but has different outcomes for thought-qt and thought-cli.
echo "[COPYRIGHT]" > footer.h2m
$THOUGHTD --version | sed -n '1!p' >> footer.h2m

for cmd in $THOUGHTD $THOUGHTCLI $THOUGHTTX $THOUGHTQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
