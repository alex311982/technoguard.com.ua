<?php
	$VARS = $this->get_parsed_vars();
    $err = array();
    $openid_data = @$_SESSION["ekv_openid_data"];

    if(empty($openid_data)){
        force_location("/user/login.html");
        exit;
    }

	if(isset($VARS["is_form_submited"])){
        $userOpenidObj = new EkvUserOpenid();
        if(fCheckEmail($VARS["email"]) == false){
            $err["email"] = "Некорректный email";
        }else{
            $user_info = $userOpenidObj->getBaseUserInfoByEmail($VARS["email"]);
            if(!empty($user_info)){
                $err["email"] = "такой email уже используется в системе";
            }
        }

        if(empty($err)){
            $openid_data["email"] = $VARS["email"];
            $new_user_id = $userOpenidObj->registerUserByOpenidData($openid_data);
            if($new_user_id > 0){
                unset($_SESSION["ekv_openid_data"]);
                $user_info = $userOpenidObj->getBaseUserInfo($new_user_id, "");
                $tmp_param = null;
                if($userOpenidObj->tryLogin($VARS["email"], $user_info["password"], $tmp_param, true) == true){
                    if(isset($VARS["autologin"])){
                        $userOpenidObj->setAutologinCookie($user_info);
                    }
                    $return_url = "/";
//                    if(isset($openid_data["referer"])){
//                        $return_url  = $openid_data["referer"];
//                    }
                    force_location($return_url);
                }                
            }else{
                force_location("/user/login.html");
            }
        }
    }
	$this->TML->assign("err", $err);
	$this->TML->assign('vars', $VARS);
	$this->Display('common:base/user/page.loginpage_ask_email.tpl');
?>