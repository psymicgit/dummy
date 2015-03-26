function kuaiyong_test()
	print "----------------------开始快用支付平台测试----------------------"

	--快用公钥
	public_key = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+WNnrVVHQEcE3VwiuOPdAMZ5NGgs4Dikn6vd+XTURtAwn/86jd7En86F4s61pq7zjABNsVka8lpeq0Dq9XbXOQAPHJBt4PyEKA9EJ9XMeGF63UeJYPmN5VgrGtLJdPoFLY5AdTiNf3v6+CHyIoDs0NWsw3OSbostMbgwm7Z5A+wIDAQAB";

	--快用密文
	encrypt_text = "V/5/99ubmARZ0uOT/KOBrOdns/91mm23mAGDvfvgJjhG36/R82QaaEQrD7+vIR7850Hx03wZr7QcdIy2CGiEB+p5lgKDjU8FAQAjwLxqIWNT0T8ugdwGiMI3pC/SKlY3I0mKtoN78YNIgFLWA5QM0xSWvjls5p7hLbs4cZz6Oe4=";

	--通过公钥解密密文，得到明文
	decrypt_text, decrypt_len, errmsg = rsa.public_key_decrypt(encrypt_text, public_key);

	if decrypt_text then
		print("明文 = "..decrypt_text);
		print("明文长度 = " .. decrypt_len);
	else
		print("错误提示 = "..errmsg);
	end

	sign_public_key  = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDQl074HcPTQ+cOyoUaL4cznY8yt5WBg76oIUiaqO+eOG0a31J08e9jqU1kEs9Ik2yjU+VVMQdZpYEoewg/XmKhtj4GvvClvkvmMxZUXTP0pWLG8EIX0seHEt+9oECwmVsfJyVvXl2yAc0FKLRCmGfjBko38YRa0NJ4iW871toPnQIDAQAB";
    sign_private_key = "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBANCXTvgdw9ND5w7KhRovhzOdjzK3lYGDvqghSJqo7544bRrfUnTx72OpTWQSz0iTbKNT5VUxB1mlgSh7CD9eYqG2Pga+8KW+S+YzFlRdM/SlYsbwQhfSx4cS372gQLCZWx8nJW9eXbIBzQUotEKYZ+MGSjfxhFrQ0niJbzvW2g+dAgMBAAECgYBbhxlCrCTiTfhUOC9Bdzt3PZq5IW873mhJdBlcq+ZA4feWBAo1gHePnplz4QENkorn/Ac0Y4gbIRFUhu/uhmb2TZ0OeoaLEYpgXQya6v6YsUrtc5SNNnl5YyWD1Il0J8x5qo69Tx1zJ2K3JJsBX6XqLvr/KwryCz4nt0gFclQMLQJBAOxdnAJSA1t0YAnLTcFbtBOTn+oBG+U42k4GfV77wP0gIdl1Hj60FcYSMpnwm1Z0c71EkiIRaz59fgy6HoFB2q8CQQDh6xApLDNNz5xi4a1+G1wRw3vkrwaZ9/iZ4PFT6G1gxqtgXKV/9aZ7L4jGBxZpz/O94R7oBuFhPll4MkgbMR1zAkBe3UcjC0V7rB5rb+Q6KZMBlGtHIMDQY9wCGsqwl3wiW4YaX84OvnpKr70NWasOrNhS6zV3ZORVBUrU0tIxcHvTAkEAmx+UkTDQaXVMpHjhb93JckkQIQ4tMjuab86M5liKAhFoS1pSKXK7RYp2F8N2GGG5s3+IgTl4OBVfybB84m7vEQJBAMLq5hGVV3LJHPA+gnxmZF0n4kkwNlDyCdWGBR7BZ9e2aSMdCa6/tXgnRlkaXpZTxxXnxGG5DGcCRe/WI9p2dEc=";
    sign_plain_text  = "dealseq=20130222105228127&fee=0.01&notify_data=V/5/99ubmARZ0uOT/KOBrOdns/91mm23mAGDvfvgJjhG36/R82QaaEQrD7+vIR7850Hx03wZr7QcdIy2CGiEB+p5lgKDjU8FAQAjwLxqIWNT0T8ugdwGiMI3pC/SKlY3I0mKtoN78YNIgFLWA5QM0xSWvjls5p7hLbs4cZz6Oe4=&orderid=130222-23-10523450458&payresult=0&subject=充值100金&uid=放着我来&v=1";

	--使用私钥进行签名
	sign_text, sign_len, errmsg = rsa.private_key_sign(sign_plain_text, sign_private_key);
	if sign_text then
		print("sign = 签名 = "..sign_text);
		print("签名长度 = ".. sign_len);
	else
		print("errmsg = "..errmsg);
	end

	--base64编码
	base64_sign_text = rsa.base64_encode(sign_text, sign_len);
	if base64_sign_text then
		print("base64编码后的签名 = "..base64_sign_text);
	end

	--使用公钥进行验签
	verify_ok, errmsg = rsa.public_key_verify(sign_plain_text, base64_sign_text, sign_public_key);
	print("是否验签成功 = ", verify_ok);

	if verify_ok == false then
		if string.len(errmsg) > 0 then
			print("错误原因 = "..errmsg);
		end
	end

	print "----------------------结束快用支付平台测试----------------------"
end

function kebi_test()
	print "----------------------开始可币支付平台测试----------------------"

	--可币公钥
	sign_public_key  = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCmreYIkPwVovKR8rLHWlFVw7YDfm9uQOJKL89Smt6ypXGVdrAKKl0wNYc3/jecAoPi2ylChfa2iRu5gunJyNmpWZzlCNRIau55fxGW0XEu553IiprOZcaw5OuYGlf60ga8QT6qToP0/dpiL/ZbmNUO9kUhosIjEu22uFgR+5cYyQIDAQAB";

	--可币签名原串
    sign_plain_text  = "notifyId=GC2014012017520312700024066238&partnerOrder=andoppo_1390211520_100069268_8491&productName=x330个元宝&productDesc=&price=3000&count=1&attach=100069268";

	--可币签名
	sign_text		 = "bG+TIq7JVkLL4H0o4Npd9r/Vp84mi6MAJbUh+DKNMbTELKetdzmIbY0vzQNHBVgUJcc2o7jpgdx58dSsM1rjtXZ+B+WEbNZ748JWhNWr+MY1+KYcV+YM0fBJuuc4esyxxfVuiYpTolzrvKPB2L3dPKcqbvKKEAssIGlH1aXo9Yk=";

	--使用可币公钥进行验签
	verify_ok, errmsg = rsa.public_key_verify(sign_plain_text, sign_text, sign_public_key);
	print("是否验签成功 = ", verify_ok);

	if verify_ok == false then
		if string.len(errmsg) > 0 then
			print("错误原因 = "..errmsg);
		end
	end

	print "----------------------结束可币支付平台测试----------------------"
end

function rsa_sign_test()
	local privateKey="MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAIwcoWXtY6IHT1DesY7nqOr1Sc6lf7ZyFLKE/OuaEyDvWVYjHNnrHvuF2shC1YfEKJaz6+sAsg8Y38bAamtm/KZJcWv08Dr2h8YGWz4sLfng6iPqfHLVAhXpveegUh8Y1WXfT1hMX2BGq0tSuLpmC4jNjDAAKwmWn+WwQNnX3MGjAgMBAAECgYAa4LMldnCSeIyCmvW80DvbNsq91Ym2WhQWWuSZ3ailvMSKfZnfQ/dhkL/zgr6rmLmgj0tGfY+qzb6/xdGkYNE+W332XtN8qJ9C4T2Ahh4acLCVl+kmk0X0OzTw13Wek+si/20XAfsT8mLCtGghTycsr/qjJgyid1YSGR644A9QwQJBANyX9ORwl3GRnohqRCJ9mIAK0YTHZGCXvfN7GyHsSntviA56h2WIJL6hylUAvmohVUX3HeQKE78m4g+sCeZKLAkCQQCimbuMbc++tjm6C1SzTC86sRe+G2O5o0mvNkEfifQGBv+QzyV3dHmRL5nE48UZzCYwXLl6NXm2ESKYVosPn8NLAkAKEiXsB/fiB3PlZnSV3oYH0naeSTwb7NGPPB+Q/Oipmf3sK0vjJsQGr/w8xTvsPHlYKGAE+TDNmUjYzlHlYZ2BAkAdVf8wZt+EWPwxRLoTXztmK21NL0PeykcRBYjaV9le/QQ+7CCmXK1SNHRZwX5eGScKsrAEDIHZB15mfba5KunhAkEAqvY+lNXuVXtzOj3AtRgbKBujgMqg0vPZROaKlncB9It05uBqpR7LhrnqfmhFhZTlVgaP/6U2Vzk5FSM1d1AlHg=="
	local text="D052CB7408B94E83B0FEF8AD9F9FAE3061http://a-737-root.jxy.737.com:8081/3rd_jinli/pay_andjinlia1398761941b10030731960201404291659016"

	local signtext,signlen,errmsg = rsa.private_key_sign(text,privateKey)
	print("长度 = ", string.len(signtext))
	print("实际长度 = ", signlen)
	print("errmsg  = ", errmsg)
	print(signtext)

	print(rsa.base64_encode(signtext,signlen))


	local base64_text="UBIHwDJvc6b4tUZGPcuxA3ED+fCctOCJUmQ6fSIL6BpRbv4cnsGufMyR6aojUVekcgQ/x436di5JtUeRSbs+v40XS8VLbFifpkp9yBjJ0KzGasvd7l5GjtYGfoAcNjpMCfL7EdwrJ2mxuQIAVL76UN+SH3QQ2m3NRa7OIZGjyCc="
	local base64_decode,decodelen=rsa.base64_decode(base64_text)
	print(rsa.base64_decode(base64_text))
end

require "rsa"

for k, v in pairs(rsa) do
    print(k, v);
end

if rsa then
	kuaiyong_test()
	kebi_test()
	--rsa_sign_test()
end
