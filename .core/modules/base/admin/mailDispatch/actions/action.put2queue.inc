<?php

	//setlocale(LC_ALL, 'ru_RU.CP1251'); 
 	//ini_set("MAX_EXECUTION_TIME", null);
 	set_time_limit(0);

    $mailDispatch = new EkvMailDispatch($this->TML);
    $mdTaskObj = new EkvMailDispatch_Tasks($mailDispatch);
    foreach ($this->ids as $k=>$val){        
        //$this->performDispatch($val);
        //$mailDispatch->performDispatchRowById($val);
        $task_add_res = $mdTaskObj->createTask($val);
        if($task_add_res){
            $mailDispatch->updateDispatchStatus($val, 'queue_added');

        }
        //$fLoggerObj = new EkvFileLogger("vasya");
    }
	//back to browse via js because headers have been already sent
	$back_url = "/admin/base/mailDispatch/browse.html?task_add=success";
    force_location($back_url);

?>
