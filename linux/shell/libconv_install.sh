
#  @File Name: gcc_install.sh
#  @Author: hongkunan
#  @Company: 
#  @Created Time: Sun 03 Nov 2013 07:01:00 AM PST

#!/bin/bash

#installing subversion

libconv_dir=libiconv-1.14
libconv_tar=$libconv_dir.tar.gz

echo "detecting required files & directorys"
	if [ -d $rubbish_dir ] ;then
		echo "ok: Directory ${rubbish_dir} Exists!"
	else
		mkdir $rubbish_dir
	fi


if [ ! -f $libconv_tar ]; then
	echo "install $libconv_tar fail: ${libconv_tar} not exist, exiting"
	exit
fi

echo "1. installing ${libconv_tar}"
	tar zxvf $libconv_tar
	cd $libconv_dir
		./configure --prefix=/usr/local
		make
		make install
	cd ..