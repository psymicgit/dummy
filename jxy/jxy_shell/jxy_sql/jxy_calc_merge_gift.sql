select zoneid as 区号, name as 区名, starttime as 开服时间, TO_DAYS(NOW())-TO_DAYS(starttime) as 开服天数, 1000 - TO_DAYS(NOW()) + TO_DAYS(starttime) as 元宝, (1000 - TO_DAYS(NOW()) + TO_DAYS(starttime)) * 100 as 铜钱 from gamezones;













select concat('"', zoneid) as 区号, 10000 + zoneid as 礼包id, 1000 - TO_DAYS(NOW()) + TO_DAYS(starttime) as 元宝, concat((1000 - TO_DAYS(NOW()) + TO_DAYS(starttime)) * 100, '"') as 铜钱 from gamezones;