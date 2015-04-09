<?php
    $username = "jjs361@163.com"; 
    $username = urlencode($username); 
    $srcdata = "username=".$username."&authtype=3";
    
    $key = "3t+abo!1qsf6K)dsfjas*heAF4=he~%|#aKGasdj";
    
    $sign = hash_hmac('md5', $srcdata, $key); 
    
    $postdata = $srcdata."&sign=".$sign;
    
    print $postdata;
    print "<br><br>";
    
    //$url = "http://localhost:80/jxy/getUserPlayerList.php"; 
    $url = "http://localhost:8889/jxy/getUserPlayerList.php"; 
    
    $ch = curl_init();
    curl_setopt($ch,CURLOPT_URL, $url);
    curl_setopt($ch,CURLOPT_HEADER,0);
    curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
    curl_setopt($ch,CURLOPT_POST,80);
    curl_setopt($ch,CURLOPT_POSTFIELDS,$postdata);
    
    $data = curl_exec($ch); 
    curl_close($ch);
    
    print_r($data);
    print "<br><br>";
    
    $arr = array();
    $arr = json_decode($data, TRUE);  
    
    print_r($arr);
    print "<br>";
?>
