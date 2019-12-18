<?php

/** @var $this BaseUser */
$this->DB->table = EkvUser::TABLE_USER;
$this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "signup");
$this->prepareSubscriptionIndepFields();

$VARS_initial = $this->get_parsed_vars();
$VARS = $this->vars_strip_tags($VARS_initial);
$err = array();

//--- for authorized user - redir to profile subscription page ---//
if(__UID > 0){
    //force_location("/user/cabinet.html?cmd=subscriptions");    
}

//--- country-city ---//
$userLocationObj = new EkvUserLocation();
$country_options = $userLocationObj->getCountryOptions();
$city_options_ukr = $userLocationObj->getCityOptions();
$this->TML->assign("country_options", $country_options );
$this->TML->assign("city_options", $city_options_ukr );
$this->TML->assign("country_ukr", EkvUserLocation::COUNTRY_ID_UKR );

if(isset($VARS["is_form_submited"])){
    $userObj = new EkvUser();
	//pa($VARS);
	$this->DB->CheckVariablesFrontend($VARS, $err);
	//--- email check ---//
	if(!empty($VARS["email"]) && fCheckEmail($VARS["email"]) == false){
		$err["email"] = PROFILE_EMAIL_INCORRECT;
	}
	//check is email free
	elseif(!empty($VARS["email"])  && $userObj->isUserEmailFree($VARS["email"]) == false){
		$err["email"] = SUBSCR_EMAIL_BUSY;
	}


	//--- captcha ---//
	if(!empty($VARS["captcha"]) && $VARS["captcha"] != $_SESSION["CAPTCHA_CODE"]){
		$err["captcha"] = PROFILE_CAPTCHA_WRONG;
	}
    //--- correct city field ---//
    $userLocationObj->maintainCorrectCityField($VARS, $err);

	//--- insert record ---//
	if(empty($err)){
		$VARS["enabled"] = 0;
		$VARS["password"] = $userObj->generateTmpPasswordHash();
		$VARS["reg_date"] = phpdate2mysqldate(time());
		$VARS["hash"] = $userObj->generateUserHash($VARS["email"]);
		
		$uid = $this->DB->InsertRecord($VARS);
		if($uid > 0){
            $VARS["id"] = $uid;
            $email_input_data = $VARS;

            $country_name = $country_options[$email_input_data['user_country_id']];
            $city_name = $userLocationObj->getCityNameFromInputData($email_input_data, $city_options_ukr);
            $email_input_data["user_country_id"] = $country_name;
            $email_input_data["user_city"] = $city_name;

            $auto_str = EkvCommonHelper::generateAutoStrForEmailTemplate($this->DB->fields, $email_input_data);

            //#------------------- DON'T FORGET ABOUT AUTOREGISTRATION ON EVENT PREVIEW PAGE -------------------#//

			//--- send email to admin ---//
			$anchor = array("auto_str" => $auto_str);				
			sendmail("REG_ADMIN_MAIL", ADMIN_EMAIL, $anchor);

			//--- send email with confirmation to user ---//
			$anchor = array(
					"SITE_HOST"		=> EkvCommonHelper::getSiteHostFromConf(),
					"confirm_url"	=> EkvCommonHelper::getSiteHostFromConf()."/user/activation.html?uid={$VARS["hash"]}"
				);				
			sendmail("REGISTRATION_USER_MAIL", $VARS["email"], $anchor);			
			
			$this->TML->assign("show_success_greeting", true);
		}else{
			//$this->TML->assign("insert_res", false);
		}
	}	
	//pa($err);	
	$this->TML->assign("err", $err);
}else{
	
}


$this->display3Columns();
$this->TML->assign("vars", $VARS);
$this->Display('common:base/user/page.subscription_independent.tpl');

?>