<?php
/**
 * ============================================================
 * File name: xxx.xxx
 * Date: xx/xx/2010
 * Author: Yegor Konoval
 * Url:	
 * Purpose: 
 * ============================================================
 */
    //--- can process prepared for openid auth data insted of real one ---//
    $DEBUG_MODE_LOGIN = 0;
    if(isset($_REQUEST["ask_email"])){
        include "include.login_ask_email.inc";
        exit;
    }
    $USE_PASSWORD_HASH_INSTEAD_PLAIN_PASSWORD = false;
    $userObj = new EkvUser();
  	$referer = $userObj->checkOuterReferer(@$_SERVER["HTTP_REFERER"]);
	$VARS = $this->get_parsed_vars();
	//--- correct return url when first attemp is success ---//
	if(empty($VARS["return_url"]) && !empty($referer)){
		$VARS["return_url"] = $referer;
	}else if(empty($VARS["return_url"])){
        $VARS["return_url"] = "/";
    }
	//pa($VARS);
	$err = array();

    if (isset($_POST['token']) || $DEBUG_MODE_LOGIN == 1){
        include "include.login_openid.inc";
    }
		
	if( (!empty($referer) && isset($VARS["email"]) && isset($VARS["password"]) )
		|| isset($VARS["is_form_submited"]) 
		|| isset($VARS["is_hp_login_form_submited"])
	){

        $user_info = array();
		//--- success login ---//
		if($userObj->tryLogin($VARS["email"], $VARS["password"], $user_info, $USE_PASSWORD_HASH_INSTEAD_PLAIN_PASSWORD) == true){
            if(isset($VARS["autologin"])){
                $userObj->setAutologinCookie($user_info);
            }
			//$return_url = "/";
            $return_url  = $VARS["return_url"];
			force_location($return_url);
		}
		//--- login failed ---//
		else{
			$err["email"] = PROFILE_CREDENTIALS_INVALID;			
		}
	}
	$this->TML->assign("err", $err);
	$this->TML->assign("referer", $referer);
	$this->TML->assign('vars', $VARS);
	$this->Display('common:base/user/page.loginpage.tpl');
?>