#!/bin/sh

gcc -v

if [ $? != 0 ]; then
	echo "GCC is not installed!"
	exit
fi

ld -v
if [ $? != 0 ]; then
	echo "Install 'binutils'"
	exit
fi

gcc src/life.c -o cLIFE
mkdir /usr/bin/cLIFE
mv cLIFE /usr/bin/cLIFE
mv maps /usr/bin/cLIFE
ln -s /usr/bin/cLIFE/cLIFE /bin/cLIFE
