<?php

	$hash = @$_REQUEST["hash"];
	MailDispatch::initDb($this->DB);
	$res = MailDispatch::unsubscribeUserByHash($hash);
	$this->TML->assign("res", $res);
	$this->Display("common:base/mailDispatch/page.unsubscribe.tpl");
?>