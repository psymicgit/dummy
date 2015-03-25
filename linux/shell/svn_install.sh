
#  @File Name: gcc_install.sh
#  @Author: hongkunan
#  @Company: 
#  @Created Time: Sun 03 Nov 2013 07:01:00 AM PST

#!/bin/bash

#installing subversion

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

svn_ver=1.6.17
svn_dir="subversion-${svn_ver}"
svn_tar=$svn_dir.tar.gz
svn_deps_tar=subversion-deps-$svn_ver.tar.gz
openssl_dir=openssl-1.0.0a
openssl_tar=$openssl_dir.tar.gz
neon_dir=neon-0.30.0
neon_tar=$neon_dir.tar.gz

echo "detecting required files & directorys"
	if [ -d $rubbish_dir ] ;then
		echo "ok: Directory ${rubbish_dir} Exists!"
	else
		mkdir $rubbish_dir
	fi


if [ ! -f $svn_tar ]; then
	echo "svn update fail: ${svn_tar} not exist, exiting"
	exit
fi

if [ ! -f $svn_deps_tar ]; then
	echo "svn update fail: ${svn_deps_tar} not exist, exiting"
	exit
fi

if [ ! -f $openssl_tar ]; then
	echo "svn update fail: ${openssl_tar} not exist, exiting"
	exit
fi

echo "svn update to ${svn_dir}"
	which svn > /dev/null 2>&1
	is_svn_installed=$?
	svn_need_install=0

	if [ $is_svn_installed != 0 ]; then
		echo "svn not exist, start installing ${svn_dir}..."
	else
		cur_svn_ver=$(svn --version --quiet)
		svn_ver_cmp=$(ver_cmp_func $cur_svn_ver $svn_ver)

		if [ $svn_ver_cmp \< 0 ]; then
			echo "svn version:$cur_svn_ver < required version:$svn_ver"
			echo "starting updating to $svn_ver..."
		else
			echo "ok: svn version:$cur_svn_ver >= required version:$svn_ver, passing"
			exit
		fi
	fi

echo "1. install $openssl_tar"
	openssl_bin_dir=/usr/local/ssl

	which openssl > /dev/null 2>&1
	is_openssl_installed=$?

	if [ $is_openssl_installed -eq 0 ]; then
		echo "ok: detecting $openssl_bin_dir existing"
	else
		echo "detecting $openssl_bin_dir is not installed"

		echo "removing $openssl_bin_dir ..."
		rm -rf $openssl_bin_dir
		echo "start install $openssl_tar ..."

		tar -zxvf $openssl_tar
		cd $openssl_dir
			./config --prefix=$openssl_bin_dir
			./config -t 
			make depend

			#编译
			#cpu 是四核的话，可以 make -j4 加速编译
			make -j4

			make test 

			#安装
			#可能需要管理员权限
			sudo make install
		cd ..

		echo export PATH=/usr/local/openssl/bin:/sbin/:$PATH:/usr/sbin >> /etc/profile
		source /etc/profile
	fi

echo "2. install $svn_tar"
	if [ -d $svn_dir ]; then
		echo "detecting $svn_dir existing, pass extracting"
	else
		tar -zxvf $svn_tar $svn_dir
		tar -zxvf $svn_deps_tar $svn_dir
	fi

	echo "move newest $neon_dir to $svn_dir"
	# 用最新的neon替换掉subversion过时的neon，否则将导致编译时出现undefined reference to `SSL_SESSION_cmp'错误
	tar	zxvf $neon_tar
	rm -rf $svn_dir/neon/
	mv $neon_dir $svn_dir/neon

	svn_bin_dir=/usr/local/svn

	echo "removing $svn_bin_dir ..."
	rm -rf $svn_bin_dir
	echo "start install $svn_tar ..."

	#配置：选择安装目录以及编译的语言
	cd $svn_dir
		./configure --prefix=$svn_bin_dir --with-openssl=$openssl_bin_dir --with-ssl

		#编译
		#cpu 是四核的话，可以 make -j4 加速编译
		make -j4
		
		#安装
		#可能需要管理员权限
		sudo make install
	cd ..

	echo export PATH=/usr/local/svn/bin:$PATH >> /etc/profile
	source /etc/profile

echo "3. detecting whether $svn_tar successfully installed"
	which svn > /dev/null 2>&1
	is_install_svn_succ=$?
	
	if [ $is_install_svn_succ -eq 0]; then
		echo "ok! now you have installed $svn_tar"
	else
		echo "fail! please check the error msg while installing and then try to reinstall $svn_tar"
	fi
