-- require "rsa"

function kuaiyong_test()
	print "----------------------start kuaiyong test----------------------"

	--快用公钥
	public_key = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5NGgs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPHJBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw3OSbostMbgwm7Z5A+wIDAQAB";

	--快用密文
	encrypt_text = "V/5/99ubmARZ0uOT/KOBrOdns/91mm23mAGDvfvgJjhG36/R82QaaEQrD7+vIR7850Hx03wZr7QcdIy2CGiEB+p5lgKDjU8FAQAjwLxqIWNT0T8ugdwGiMI3pC/SKlY3I0mKtoN78YNIgFLWA5QM0xSWvjls5p7hLbs4cZz6Oe4=";

	--通过公钥解密密文，得到明文
	decrypt_text, decrypt_len, errmsg = rsa.public_key_decrypt(encrypt_text, public_key);

	if decrypt_text then
		print("decrypt_text = ", decrypt_text);
		print("decrypt_len = ", decrypt_len);
	else
		print("errmsg = ", errmsg);
	end


	sign_public_key  = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDQl074HcPTQ+cOyoUaL4cznY8yt5WBg76oIUiaqO+eOG0a31J08e9jqU1kEs9Ik2yjU+VVMQdZpYEoewg/XmKhtj4GvvClvkvmMxZUXTP0pWLG8EIX0seHEt+9oECwmVsfJyVvXl2yAc0FKLRCmGfjBko38YRa0NJ4iW871toPnQIDAQAB";
    sign_private_key = "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBANCXTvgdw9ND5w7KhRovhzOdjzK3lYGDvqghSJqo7544bRrfUnTx72OpTWQSz0iTbKNT5VUxB1mlgSh7CD9eYqG2Pga+8KW+S+YzFlRdM/SlYsbwQhfSx4cS372gQLCZWx8nJW9eXbIBzQUotEKYZ+MGSjfxhFrQ0niJbzvW2g+dAgMBAAECgYBbhxlCrCTiTfhUOC9Bdzt3PZq5IW873mhJdBlcq+ZA4feWBAo1gHePnplz4QENkorn/Ac0Y4gbIRFUhu/uhmb2TZ0OeoaLEYpgXQya6v6YsUrtc5SNNnl5YyWD1Il0J8x5qo69Tx1zJ2K3JJsBX6XqLvr/KwryCz4nt0gFclQMLQJBAOxdnAJSA1t0YAnLTcFbtBOTn+oBG+U42k4GfV77wP0gIdl1Hj60FcYSMpnwm1Z0c71EkiIRaz59fgy6HoFB2q8CQQDh6xApLDNNz5xi4a1+G1wRw3vkrwaZ9/iZ4PFT6G1gxqtgXKV/9aZ7L4jGBxZpz/O94R7oBuFhPll4MkgbMR1zAkBe3UcjC0V7rB5rb+Q6KZMBlGtHIMDQY9wCGsqwl3wiW4YaX84OvnpKr70NWasOrNhS6zV3ZORVBUrU0tIxcHvTAkEAmx+UkTDQaXVMpHjhb93JckkQIQ4tMjuab86M5liKAhFoS1pSKXK7RYp2F8N2GGG5s3+IgTl4OBVfybB84m7vEQJBAMLq5hGVV3LJHPA+gnxmZF0n4kkwNlDyCdWGBR7BZ9e2aSMdCa6/tXgnRlkaXpZTxxXnxGG5DGcCRe/WI9p2dEc=";
    sign_plain_text  = "dealseq=20130222105228127&fee=0.01&notify_data=V/5/99ubmARZ0uOT/KOBrOdns/91mm23mAGDvfvgJjhG36/R82QaaEQrD7+vIR7850Hx03wZr7QcdIy2CGiEB+p5lgKDjU8FAQAjwLxqIWNT0T8ugdwGiMI3pC/SKlY3I0mKtoN78YNIgFLWA5QM0xSWvjls5p7hLbs4cZz6Oe4=&orderid=130222-23-10523450458&payresult=0&subject=充值100金&uid=放着我来&v=1";

	--使用私钥进行签名
	sign_text, sign_len, errmsg = rsa.private_key_sign(sign_plain_text, sign_private_key);
	if sign_text then
		print("sign_text = ", sign_text);
		print("sign_len = ", sign_len);
	else
		print("errmsg = ", errmsg);
	end

	--base64编码
	base64_sign_text = rsa.base64_encode(sign_text, sign_len);
	if base64_sign_text then
		print("base64_sign_text = ", base64_sign_text);
	end

	--使用公钥进行验签
	verify_ok, errmsg = rsa.public_key_verify(sign_plain_text, base64_sign_text, sign_public_key);
	print("verify_ok = ", verify_ok);

	if verify_ok == false then
		if string.len(errmsg) > 0 then
			print("error msg = ", errmsg);
		end
	end

	print "----------------------end kuaiyong test----------------------"
end

function kebi_test()
	print "----------------------start kebi test----------------------"

	--可币公钥
	sign_public_key  = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCmreYIkPwVovKR8rLHWlFVw7YDfm9uQOJKL89Smt6ypXGVdrAKKl0wNYc3/jecAoPi2ylChfa2iRu5gunJyNmpWZzlCNRIau55fxGW0XEu553IiprOZcaw5OuYGlf60ga8QT6qToP0/dpiL/ZbmNUO9kUhosIjEu22uFgR+5cYyQIDAQAB";

	--可币签名原串
    sign_plain_text  = "notifyId=GC2013110815435301400004525090&partnerOrder=1383896624538&productName=红钻&productDesc=商品描述&price=1&count=1&attach=自定义字段";

	--可币签名
	sign_text		 = "kHKXA7brp+2G1Sys3fT7JEcK13dukRLNoK0LBFi1BdKpngw2WAr8sEcP37UBfIAaRT4UXOb5dXnfzc0WIrfrWxfE+TXqfgFMBAIV6Nk2wQinZ2I45pt3zTYo3I2NFfU2IehqA+VOmgTG8uWDhjOL63HHfm1pv7CFzFedd3OM9kA=";

	--使用可币公钥进行验签
	verify_ok, errmsg = rsa.public_key_verify(sign_plain_text, sign_text, sign_public_key);
	print("verify_ok = ", verify_ok);

	if verify_ok == false then
		if string.len(errmsg) > 0 then
			print("error msg = ", errmsg);
		end
	end

	print "----------------------end kebi test----------------------"
end

local rsadll = package.loadlib("dll/rsa.dll", "luaopen_rsa");
if rsadll then
	print(rsadll)
	rsadll();
end

if rsa then
	kuaiyong_test()
	kebi_test()
end

--for k, v in pairs(rsa) do
    --print(k, v);
--end
