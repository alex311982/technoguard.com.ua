<?php
//pa($_REQUEST);
$user_hash = @$_REQUEST["hash"];
if(!empty($user_hash)){
    $userObj = new EkvUser();
    $user_info = $userObj->getUserInfoByHash($user_hash);

    if(!empty($user_info)){
        //--- notify admin ---//
        if($user_info["dispatch_subscriber"] == 1){
            $userLocationObj = new EkvUserLocation();
            $city_name = $userLocationObj->getCityNameFromInputDataWithExtraQuery($user_info);
            $anchor =
                array(
                    "id" => $user_info["id"],
                    "email" => $user_info["email"],
                    "fio" => $user_info["fio"],
                    "city" => $city_name
                );
            sendmail("ADMIN_USER_UNSUBSCRIBED", ADMIN_EMAIL, $anchor);
        }

        $userObj->setUserDispatchSubscriptionField($user_info["id"], 0);
        $this->TML->assign("result", "ok" );


    }else{
        $this->TML->assign("result", "user_failed" );
    }
    $this->Display("common:base/user/page.dispatch_unsubscribe.tpl");
}else{
    force_location_index();
}

?>