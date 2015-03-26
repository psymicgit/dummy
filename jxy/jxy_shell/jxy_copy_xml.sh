
#  @File Name: gcc_install.sh
#  @Author: hongkunan
#  @Company: 
#  @Created Time: Sun 03 Nov 2013 07:01:00 AM PST

#!/bin/bash

function cp_ext()
{
	local from=$1
	local to=$2
	local ext=$3

	rsync -arv --include="*/" --include="*.$ext" --exclude="*" $from/ $to/
	find $to -name "*.${ext}"
	#rsync -ave $ext $from/ $to/
}

win_jxy=/mnt/hgfs/maxnet/bin/server/Win32/Debug/
vm_jxy=/home/psy/proj/jxy/maxnet/bin/server/linux64/debug/
ext=xml

cp_ext $win_jxy $vm_jxy $ext
# cp_ext from to sh

#cp -r `find $win_jxy -iname *.xml` $vm_jxy

#rsync -avz --include "*.zone"
#cp -rf `find from/ -name *.sh` to
