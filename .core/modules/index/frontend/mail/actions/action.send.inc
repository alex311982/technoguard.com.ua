<?php

$path = __FILE__;
//--- fix when path is applied only like a dir ---//
if(is_dir($path)){
    $dir = $path;
}else{
    $dir = dirname($path);
}

$mail = file_get_contents($dir . '/../mail.tpl');

$mail = str_replace(
    ['%name%', '%email%', '%text%'],
    [$_REQUEST['name'], $_REQUEST['email'], $_REQUEST['text']],
    $mail
);

$to = 'valeriymir@gmail.com';

$subject = 'Заявка c сайта technoguard.com.ua';

$headers = "From: " . "info@technoguard.com.ua" . "\r\n";

$headers .= "Content-Type: text/html; charset=UTF-8\r\n";

$message = $mail;


$res = mail($to, $subject, $message, $headers);

echo $res ? 'true' : 'false';