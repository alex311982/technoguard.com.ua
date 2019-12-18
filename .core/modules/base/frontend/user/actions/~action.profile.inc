<?php
/**
 * ============================================================
 * File name: action.profile.inc
 * ============================================================
 */
  

if (__UID > 0){
	$VARS = $this->get_parsed_vars();
	$this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "base");
	$err = array();
	$user_info = $this->user->frGetFullUserInfoById(__UID);
	$VARS["email"] = $user_info["email"];
	$DO_PASSWORD_CHANGING = false;
	//pa($user_info);
	//pa($VARS);
	
	//--- form submited ---//
	if(isset($VARS["is_form_submited"])){
		if(!empty($VARS["current_password"])
			|| !empty($VARS["password"])
			|| !empty($VARS["confirmation"])
		){
			$DO_PASSWORD_CHANGING = true;
		}
		if($DO_PASSWORD_CHANGING == false){
			$this->cancelPasswordFields($this->DB->fields);
		}else{
			$this->configurePasswordFields($this->DB->fields);
		}
		$this->DB->CheckVariablesFrontend($VARS, $err);
		
//		//--- email check ---//
//		if(!empty($VARS["email"]) && fCheckEmail($VARS["email"]) == false){
//			$err["email"] = PROFILE_EMAIL_INCORRECT;
//		}
//		//check is email free
//		elseif(!empty($VARS["email"])  && $this->isUserEmailFree($VARS["email"], $user_info["email"]) == false){
//			$err["email"] = PROFILE_EMAIL_BUSY;
//		}
		
		//--- check password correctness only if change password checkbox is checked ---//
		if($DO_PASSWORD_CHANGING == true){
			//--- pwd current ---//
			if( !empty($VARS["current_password"]) 
				&& $this->user->frEncryptPwd($VARS["current_password"]) != $user_info["password"]
			  )
			{
				$err["current_password"] = PROFILE_CURRENT_PASSWORD_WRONG;
			}
			//--- pwd ---//
			if(!empty($VARS["password"]) 
				&& $this->isPasswordValid($VARS["password"]) == false
			  )
			{
				$err["password"] = "Минимальная длинна пароля ".PROFILE_MIN_PWD_LENGTH." символа(ов)";
			}
			//--- pwd confirmation ---//
			elseif(!empty($VARS["password"]) 
					&& !empty($VARS["confirmation"])
					&& $VARS["password"] != $VARS["confirmation"]
			  ){
				$err["password"] = PROFILE_PWD_CONFIRM_NOT_EQUAL;
			}
		}
		
		
		//--- data is correct -> update record ---//
		$tpl_vars = $VARS;
		if(empty($err)){
			$VARS["password"] = $this->user->frEncryptPwd($VARS["password"]);
			$this->DB->UpdateRecord( __UID, $VARS);
			$this->TML->assign("update_res", true);
		}
		$this->TML->assign("err", $err);
	}
	//--- initial form ---//
	else{
		$tpl_vars = $user_info;
		$tpl_vars["checkbox_show_password"] = false;
		unset($tpl_vars["password"]);
	}
	
	$this->TML->assign("vars", $tpl_vars);
	$this->Display('common:base/user/page.profile.tpl');
}else{
	force_location("/");
}
?>