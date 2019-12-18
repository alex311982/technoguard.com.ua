<?php
$VARS = $this->get_parsed_vars();

$uid = __UID;
$userObj = new EkvUser();
$user_info = $userObj->getBaseUserInfo($uid, ", dispatch_subscriber, sms_dispatch_subscriber");
//pa($user_info);
//pa($VARS);

if(isset($VARS["is_form_submited"])){
    $email_flag = @intval($VARS["dispatch_subscriber"]);
    if($email_flag != 0){
        $email_flag = 1;
    }
    //--- sms ---//
    $sms_flag = @intval($VARS["sms_dispatch_subscriber"]);
    if($sms_flag != 0){
        $sms_flag = 1;
    }
    $userObj->setUserDispatchAllSubscriptionFields($uid, $email_flag, $sms_flag);
    $this->TML->assign("result", "ok" );
}else{
    $VARS = $user_info;
}

$this->TML->assign("vars", $VARS);
$this->Display('common:base/user/page.subscriptions.tpl');
