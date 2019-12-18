<?php
/**
 * DEPRECATED since 20.06.2011
 */

	//setlocale(LC_ALL, 'ru_RU.CP1251'); 
 	//ini_set("MAX_EXECUTION_TIME", null);
 	set_time_limit(0);

    $mailDispatch = new EkvMailDispatch($this->TML);

    $mdTaskObj = new EkvMailDispatch_Tasks($mailDispatch);

 	foreach ($this->ids as $k=>$val){
		//$this->performDispatch($val);
        $mailDispatch->performDispatchRowById($val);
        //$mdTaskObj->createTask($val);
        //$fLoggerObj = new EkvFileLogger("vasya");
	}
	//back to browse via js because headers have been already sent
	$back_url = "/admin/base/mailDispatch/browse.html?delivery_status=success";
	echo "<a href='$back_url'>назад</a>";
	//echo "<script> document.location = '$back_url' ; </script>";
 	//$this->ChangeLocation('browse');

?>
