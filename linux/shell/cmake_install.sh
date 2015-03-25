
#  @File Name: cmake_install.sh
#  @Author: xxx
#  @Company: xxx
#  @Created Time: Sun 03 Nov 2013 05:56:54 AM PST

#!/bin/bash

# install cmake

cmake_dir=cmake-2.8.11.2
cmake_tar=$cmake_dir.tar.gz
if [ ! -f $cmake_tar ]; then
	echo "err: could not find $cmake_tar"
fi
tar -zxvf $cmake_tar
cd $cmake_dir
./Bootstrap -prefix=usr/local/cmake

make
make install

echo PATH=/usr/local/cmake/bin:\$PATH >> /etc/profile
echo export PATH >> /etc/profile

source /etc/profile
cd ..
