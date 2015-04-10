#2014_01_06
GetDays()
{
	if [ "-$1" = "-" ];then
		echo 999999999;
	else
		DayTmp=`echo $1 | cut -c 7-8`
		MonthTmp=`echo $1 | cut -c 4-5`
		YearTmp=`echo $1 | cut -c 1-2`

		MonthTmp=`expr $MonthTmp \* 30`
		YearTmp=`expr $YearTmp \* 365`
		Days=`expr $DayTmp + $MonthTmp + $YearTmp`
		echo $Days
	fi
}

#20140106
GetDays2()
{
	DayTmp=`echo $1 | cut -c 7-8`
	MonthTmp=`echo $1 | cut -c 5-6`
	YearTmp=`echo $1 | cut -c 1-4`

	MonthTmp=`expr $MonthTmp \* 30`
	YearTmp=`expr $YearTmp \* 365`
	Days=`expr $DayTmp + $MonthTmp + $YearTmp`
	echo $Days
}

#2014-01-06
GetDays3()
{
	DayTmp=`echo $1 | cut -c 9-10`
	MonthTmp=`echo $1 | cut -c 6-7`
	YearTmp=`echo $1 | cut -c 1-4`

	MonthTmp=`expr $MonthTmp \* 30`
	YearTmp=`expr $YearTmp \* 365`
	Days=`expr $DayTmp + $MonthTmp + $YearTmp`
	echo $Days
}

