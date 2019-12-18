<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	$this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "base");
	$VARS = $this->get_parsed_vars();
	$err = array();
	if( isset($VARS["email"]) ){
        $userObj = new EkvUser();
                
		$email = strip_tags($VARS["email"]);
		$email = trim($email);
		if(empty($email)){
			$err["email"] = PROFILE_FORGOT_EMAIL_EMPTY;
		}elseif(fCheckEmail($email) == false){
				$err["email"] = PROFILE_EMAIL_INCORRECT;
		}else{
			//-- try to get user info by email ---//
			$user_info = $userObj->getBaseUserInfoByEmail($email);
			if(empty($user_info)){
				$err["email"] = PROFILE_FORGOT_NO_USER;
			}else{
				$tmp_pass = $userObj->generateTmpPassword(6);
				$tmp_pass_encrypted = $userObj->generatePwdHash($tmp_pass);
				$userObj->updateUserPassword($user_info["id"], $tmp_pass_encrypted);
				// send user email
				$anchor = array(
					"user"	=> "{$email}",
					"site" => EkvCommonHelper::getSiteHostFromConf(),
					"password" => $tmp_pass,
					);
				sendmail("USER_FORGOT_PASSWORD", $VARS['email'], $anchor);
				
				$this->TML->assign("finished", true);
			}
		}
	}
	$this->TML->assign("err", $err);
	$this->TML->assign('vars', $VARS);	
	$this->Display('common:base/user/page.forgot.tpl');

?>
