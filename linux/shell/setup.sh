#!/bin/bash

#  @File Name: setup.sh
#  @Author: xxx
#  @Company: xxx
#  @Created Time: Sat 02 Nov 2013 08:56:54 AM PST

# 初次安装linux时所执行的bash，里面的内容包括升级gcc、安装cmake等，以及配置vim等

# cmp ver1 ver2
# eg.: 2.8.11.2 < 2.8.11.4, then it return -2
function ver_cmp_func()
{
	local str_ver1=$1
	local str_ver2=$2

	ver1=${str_ver1//./}
	ver2=${str_ver2//./}

	result=`expr $ver1 - $ver2`
	echo $result

	#echo $ver1 > $ver2
}

rubbish_dir="rubbish"
info_txt=$rubbish_dir/"info.txt"

D="[0-9]\{1,\}"

echo "start <install&configuration>..."
	if [ -d $rubbish_dir ] ;then
		echo "Directory ${rubbish_dir} Exists!"
	else
		mkdir $rubbish_dir
	fi

echo "1. gcc/g++ update to 4.8.1"
	#yum update -y gcc &>$info_txt
	#yum update -y gcc-c++ &>$info_txt
	gcc_required_ver="4.8.1"

	gcc -v &>$info_txt
	gcc_ver=$(grep "^gcc version*" $info_txt)
	gcc_ver=`expr match "$gcc_ver" ".*\($D\.$D\.$D\).*"`
	gcc_ver_cmp=$(ver_cmp_func $gcc_ver $gcc_required_ver)
	if [ $gcc_ver_cmp \< 0 ]; then
		echo "gcc/g++ version:$gcc_ver < required version:$gcc_required_ver"
		echo "starting updating to $gcc_required_ver"

		./gcc_install.sh
	else
		echo "gcc/g++ version:$gcc_ver >= required version:$gcc_required_ver, passing"
	fi

echo "2. vim configure"
	vimrc=".vimrc"
	cp $vimrc ~/
	if [ ! -d $vimrc ] ;then
		cp $vimrc ~/
	fi

echo "3. make update to "

echo "4. install cmake 2.8.11.2"
	which cmake > /dev/null 2>&1
	is_cmake_exist=$?
	cmake_need_install=0
	cmake_target_ver="2.8.11.2"
	if [ $is_cmake_exist != 0 ]; then
		echo "cmake not exist, start installing $cmake_target_ver..."
	else
		cmake_ver=$(cmake --version)
		cmake_ver=`expr match "$cmake_ver" ".*\($D\.$D\.$D\.$D\).*"`

		cmake_ver_cmp=$(ver_cmp_func $cmake_ver $cmake_target_ver)
		#echo "version cmp = $cmake_ver_cmp"

		if [ $cmake_ver_cmp \< 0 ]; then
			cmake_need_install=1
			echo "current cmake version:$cmake_ver < required version:$cmake_target_ver"
			echo "start updating to $cmake_target_ver"
		else
			echo "current cmake version:$cmake_ver >= required version:$cmake_target_ver, passing"
		fi
	fi

	if [ $cmake_need_install != 0 ]; then
		./cmake_install.sh
	fi

	echo ""

echo "finishing"

