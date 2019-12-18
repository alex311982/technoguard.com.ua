<?php

$to = 'alex.gubarev.web@gmail.com';

$subject = 'Website Change Reqest';

$headers = "From: " . "alex.gubarev.web@gmail.com" . "\r\n";

$headers .= "Content-Type: text/html; charset=UTF-8\r\n";

$message = '<p><strong>This is strong text</strong> while this is not.</p>';


mail($to, $subject, $message, $headers);