time=`date '+%Y-%m-%d--%H-%M-%S'`
dirTmp=`date '+%Y-%m-%d'`
dir=/home/bak/data/$dirTmp/serused
mkdir -p $dir

tmp1=`date '+%Y-%m-%d--%H'`
serusedlog=${dir}/${tmp1}.log
date '+%Y-%m-%d--%H-%M-%S' >> ${serusedlog}
ps auxw|head -1;ps auxw|sort -rn -k3|head -10  >> ${serusedlog}
ps auxw|head -1;ps auxw|sort -rn -k4|head -10  >> ${serusedlog}
ps auxw|head -1;ps auxw|sort -rn -k5|head -10  >> ${serusedlog}
echo "*******************************************************************">>${serusedlog}
echo "*******************************************************************">>${serusedlog}
echo "*******************************************************************">>${serusedlog}
echo "">>${serusedlog}
echo "">>${serusedlog}
echo "">>${serusedlog}

