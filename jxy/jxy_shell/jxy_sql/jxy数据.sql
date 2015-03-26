


1. 	经统计，1区中的oprecord关于元宝操作的记录数大概为每10天15万条，同时，这也意味着：每1000万条中约含有15万条元宝相关的操作，这样的比例是很小的，其他区的数据与此类似，所以玩家关于元宝的记录可以每个区只用一个表不用拆表（2014_3_11日统计）

	select count(1) from ios_app_s001_log.oprecord_20140302224806 WHERE ModuleType =12 or ModuleType = 13; = 124693条
	select count(1) from ios_app_s001_log.oprecord_20140222001126 WHERE ModuleType =12 or ModuleType = 13; = 136620条
	select count(1) from ios_app_s001_log.oprecord_20140213215555 WHERE ModuleType =12 or ModuleType = 13; = 137766条
	select count(1) from ios_app_s001_log.oprecord_20140205154914 WHERE ModuleType =12 or ModuleType = 13; = 160500条
	select count(1) from ios_app_s001_log.oprecord_20140128091958 WHERE ModuleType =12 or ModuleType = 13; = 172779条

	goldrecord经统计，每区每月约有50万条数据，大概占用56.4MB

	ios区115个区
	androi 40个区

	160 * 56.4MB = 9G，所以，每个月约10GB左右的元宝记录

2. 
	囧1-6区、37-40区剩余可用内存
	total       used       free     shared    buffers     cached
	Mem:         64410      63978        432          0        555      39415
	-/+ buffers/cache:      24007      40402
	Swap:         8031       4085       3946
	Total:       72442      68063       4378

	囧7-19区
				 total       used       free     shared    buffers     cached
	Mem:         64410      62741       1668          0        416      39990
	-/+ buffers/cache:      22334      42076
	Swap:        16111          0      16111
	Total:       80522      62741      17780


3. 每天8点半和10点半是玩家高峰时段，2点~3点半是最佳维护时段

4. 2014_3_26日开始统计元宝

5. 2014_4_2日18:40分左右修复服务器回档bug

6. 目前囧西游每秒读取速率在190MB左右

7. 充值表
	exchange表
	第一笔				最后一笔
	2013-07-22 21:48:55	2014-01-03 14:17:07

	pay表
	第一笔
	2013-11-09 21:31:12