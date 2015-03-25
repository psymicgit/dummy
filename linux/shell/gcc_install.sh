
#  @File Name: gcc_install.sh
#  @Author: hongkunan
#  @Company: 
#  @Created Time: Sun 03 Nov 2013 07:01:00 AM PST

#!/bin/bash

#installing gcc/g++
gcc_dir="gcc-4.8.1"
gcc_tar="gcc-4.8.1.tar.gz"
echo "detecting required files & directorys"
if [ ! -f $gcc_tar ]; then
	echo "gcc update fail: $gcc_tar not exist, exiting"
	exit
fi
if [ -d $gcc_dir ]; then
	echo "ok: detecting ${gcc_dir} existed, skip extracting"
else
	tar -zxvf $gcc_tar
fi

if [ -d $gcc_dir/mpfr ]; then
	echo "ok: detecting mpfr existed, skip ./contrib/download_prerequisites"
else
	echo "run ./contrib/download_prerequisites"
	cd $gcc_dir
		./contrib/download_prerequisites
	cd ..
fi

echo "detecting mpfr, gmp, mpc exist or not"
if [ ! -d $gcc_dir/mpfr ]; then
	echo "err: ${gcc_dir}/mpfr configure fail, can not found the ${gcc_dir}/mpfr directory"
	exit
fi
if [ ! -d $gcc_dir/gmp ]; then
	echo "err: ${gcc_dir}/gmp configure fail, can not found the ${gcc_dir}/gmp directory"
	exit
fi
if [ ! -d $gcc_dir/mpc ]; then
	echo "err: ${gcc_dir}/mpc configure fail, can not found the ${gcc_dir}/mpc directory"
	exit
fi

echo "create required directorys"
#创建目录：编译目录和安装目录
if [ ! -d build ]; then
	mkdir build
else
	echo "ok: detecting build existed" 
fi
if [ ! -d /usr/local/$gcc_dir ]; then
	mkdir /usr/local/$gcc_dir
else
	echo "ok: detecting /usr/local/${gcc_dir} existed"
fi
if [ ! -d /usr/local/gcc ]; then
	mkdir /usr/local/gcc
else
	echo "ok: detecting /usr/local/gcc existed"
fi

echo "start configuring"
#配置：选择安装目录以及编译的语言
cd build
../$gcc_dir/configure --prefix=/usr/local/gcc-4.8.1 --exec-prefix=/usr/local/gcc --enable-languages=c,c++ --disable-multilib --enable-lto --disable-isl-version-check --enable-threads=posix
#--disable-checking

exit

echo "start making"
#编译
#cpu 是四核的话，可以 make -j4 加速编译
make -j4 

#安装
#可能需要管理员权限
sudo make install 


#mv /usr/bin/gcc /usr/bin/gcc-old
#mv /usr/bin/g++ /usr/bin/g++-old

echo export PATH=/usr/local/gcc/bin:\$PATH >> /etc/profile
echo export LD_LIBRARY_PATH=/usr/local/lib >> /etc/profile
echo export CPLUS_INCLUDE_PATH=/usr/include/c++/4.8.1:/usr/include/c++/4.8.1/x86_64-unknown-linux-gnu >> /etc/profile

source /etc/profile

ln -s /usr/local/gcc-4.8.1/include/c++/4.8.1 /usr/include/c++/4.8.1
