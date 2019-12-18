<?php
/**
 *  is included in action.login.inc
 */

$loginza_token = @$_POST["token"];
$userOpenidObj = new EkvUserOpenid();
$openid_json_data = $userOpenidObj->getUserInfoByTokenApiCall($loginza_token);
//$packed_test_data = 'YTo2OntzOjg6ImlkZW50aXR5IjtzOjM3OiJodHRwOi8vb3BlbmlkLnlhbmRleC5ydS91aWQ6NTc1MDc2NzMvIjtzOjg6InByb3ZpZGVyIjtzOjMxOiJodHRwOi8vb3BlbmlkLnlhbmRleC5ydS9zZXJ2ZXIvIjtzOjM6ImRvYiI7czoxMDoiMTk4Ni0wMS0wNSI7czo4OiJuaWNrbmFtZSI7czoyMzoi0JrQvtC90L7QstCw0Lsg0JXQs9C+0YAiO3M6NDoibmFtZSI7YToxOntzOjk6ImZ1bGxfbmFtZSI7czoyMzoi0JrQvtC90L7QstCw0Lsg0JXQs9C+0YAiO31zOjU6ImVtYWlsIjtzOjE4OiJla29ub3ZhbEBnbWFpbC5jb20iO30=';
//$openid_json_data = EkvCommonHelper::unpackArray2Text($packed_test_data);

    
EkvUserOpenid::logOpenidData($openid_json_data);
if (
    empty($openid_json_data)
    || is_array($openid_json_data) == false
    || count($openid_json_data) > 2 == false // each openid provider has 2 main items, but should have more
) {
    $err["global"] = "Ошибка провайдера OpenID";
}
else {

    //--- openid UID can be not passed directly from some providers ---//
    $openid_json_data_UID = "";
    if( isset($openid_json_data["uid"]) ){
        $openid_json_data_UID = $openid_json_data["uid"];
    }
    else if(isset($openid_json_data["identity"])){
        $openid_json_data_UID = $userOpenidObj->getTextUidFromIdentity($openid_json_data["identity"]);
    }

    $OPENID_INT_TYPE = EkvUserOpenid::getOpenidProviderIntType($openid_json_data["provider"]);
    $user_info_by_openid = $userOpenidObj->getUserInfoByOpenidUid($openid_json_data_UID);

    if(empty($openid_json_data_UID)){
        $err["global"] = "Ошибка провайдера OpenID";
    }    
    //--- user is not registered yet ---//
    else if (empty($user_info_by_openid)) {
        //--- check has email been returned by openid provider ---//
        if (isset($openid_json_data["email"])) {
            //--- check does user already exist with this email ---//
            $existent_user_info = $userOpenidObj->getBaseUserInfoByEmail($openid_json_data["email"]);
            //pa($existent_user_info);
            //--- if there is user with email taken from loginza connect our account to social network uid ---//
            if (!empty($existent_user_info)) {
                $userOpenidObj->connectOpenidData2RealUser($existent_user_info["id"], $openid_json_data_UID, $OPENID_INT_TYPE);

                //--- set params for sign in performing ---//
                $userOpenidObj->prepareVarsForLoginPerforming($VARS,
                    $existent_user_info["email"], $existent_user_info["password"],
                    $USE_PASSWORD_HASH_INSTEAD_PLAIN_PASSWORD
                );
            } else {
                $new_user_id = $userOpenidObj->registerUserByOpenidData($openid_json_data);
                if ($new_user_id > 0) {
                    $new_user_info = $userOpenidObj->getBaseUserInfo($new_user_id);
                    //--- set params for sign in performing ---//
                    $userOpenidObj->prepareVarsForLoginPerforming($VARS,
                        $new_user_info["email"], $new_user_info["password"],
                        $USE_PASSWORD_HASH_INSTEAD_PLAIN_PASSWORD
                    );
                }
            }
        }
        //--- no email has been passed by openid provider, ask user for email ---//
        else {
            $_SESSION["ekv_openid_data"] = $openid_json_data;
            $_SESSION["ekv_openid_data"]["referer"] = $referer;
            $force_url = "/user/login.html?ask_email=1";
            force_location($force_url);
        }
    } else {
        //--- openid user is ok do logging ---//
        $userOpenidObj->prepareVarsForLoginPerforming($VARS,
            $user_info_by_openid["email"], $user_info_by_openid["password"],
            $USE_PASSWORD_HASH_INSTEAD_PLAIN_PASSWORD
        );
    }
}

?>