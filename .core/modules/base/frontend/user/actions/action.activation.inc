<?php

	// incoming var 'id' => hash of user
	if( isset($_REQUEST["uid"]) && !empty($_REQUEST["uid"]) )
	{
        $userObj = new EkvUser();
		$uid = $userObj->activateUserByHash($_REQUEST["uid"]);
		if($uid > 0)
		{
			$this->Display('common:base/user/page.activate.tpl');
			exit();
		}
	}
	// if param 'id' not finded then go to home page
	force_location("/");
	exit;

?>