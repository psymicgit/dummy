function zcompress_test()
	print "----------------------开始zcompress测试----------------------"

	--快用公钥
	plain_data = "23434444444444445444444444444444444444444444444442323333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333334sf";

	print("原始数据 = "..plain_data);
	print("原始数据长度 = "..string.len(plain_data));

	--通过公钥解密密文，得到明文
	compress_data, compress_len, errmsg = zlib.zcompress(plain_data);
	if compress_data then
		print("压缩后数据 = "..compress_data);
		print("压缩长度 = " .. compress_len);
		print("压缩数据字符串长度 = ".. string.len(compress_data));
	else
		print("压缩失败：错误提示 = "..errmsg);
	end

	--使用私钥进行签名
	decompress_text, decompress_len, errmsg = zlib.zdecompress(compress_data, compress_len);
	if decompress_text then
		print("解压后数据 = "..decompress_text);
		print("解压长度 = ".. decompress_len);
	else
		print("解压失败：错误提示 = "..errmsg);
	end

	print "----------------------结束zcompress测试----------------------"
end

function gzcompress_test()
	print "----------------------开始gzcompress测试----------------------"

	--快用公钥
	plain_data = "23434444444444445444444444444444444444444444444442323333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333334sf";

	print("原始数据 = "..plain_data);
	print("原始数据长度 = "..string.len(plain_data));

	--通过公钥解密密文，得到明文
	compress_data, compress_len, errmsg = zlib.gzcompress(plain_data, string.len(plain_data));

	if compress_data then
		print("压缩后数据 = "..compress_data);
		print("压缩长度 = " .. compress_len);
		print("压缩数据字符串长度 = ".. string.len(compress_data));
	else
		print("压缩失败：错误提示 = "..errmsg);
	end

	--使用私钥进行签名
	decompress_text, decompress_len, errmsg = zlib.gzdecompress(compress_data, compress_len);
	if decompress_text then
		print("解压后数据 = "..decompress_text);
		print("解压长度 = ".. decompress_len);
	else
		print("解压失败：错误提示 = "..errmsg);
	end

	print "----------------------结束gzdecompress测试----------------------"
end

function zdecompress_test()
	print "----------------------开始zdecompress测试----------------------"

	-- gzipfile = io.open("gzipfile.txt", "r");
	-- compress_data = gzipfile:read("*a");

	compress_data = ""
	compress_len = string.len(compress_data);

	print("压缩数据 = "..compress_data);
	print("压缩数据长度 = "..compress_len);

	decompress_text, decompress_len, errmsg = zlib.gzdecompress(compress_data, compress_len);
	if decompress_text then
		print("解压后数据 = "..decompress_text);
		print("解压长度 = ".. decompress_len);
	else
		print("解压失败：错误提示 = "..errmsg);
	end

	print "----------------------结束zdecompress测试----------------------"
end

function http_test()
	http=require("socket.http")
	for i = 1, 400 do
		result=http.request("http://manager.91.com/plus/vote_ajax.php?aid=24&id=9&r=1394096883251")
		print(result)
	end
end

function kongzhong_test()
	http=require("socket.http")
	for i = 1, 40000 do
		result=http.request("http://app.shunwang.com/?app=online&controller=marfifteen&callback=?&gameid=10&gametype=2")
		print(result)
	end
end


require "zlib"

for k, v in pairs(zlib) do
    print(k, v);
end

if zlib then
	zcompress_test()
	gzcompress_test()
	-- zdecompress_test()
	-- http_test()
	kongzhong_test();
end
