<?php

$this->DB->table = EkvUser::TABLE_USER;
$this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "signup");
$VARS_initial = $this->get_parsed_vars();
$VARS = $this->vars_strip_tags($VARS_initial);
$err = array();

//--- country-city ---//
$userLocationObj = new EkvUserLocation();
$country_options = $userLocationObj->getCountryOptions();
$city_options_ukr = $userLocationObj->getCityOptions();
$this->TML->assign("country_options", $country_options );
$this->TML->assign("city_options", $city_options_ukr );
$this->TML->assign("country_ukr", EkvUserLocation::COUNTRY_ID_UKR );

if(isset($VARS["is_form_submited"])){
    $userObj = new EkvUser();

	// prepare phone number
	if(!empty($VARS['phonecode']))
	{
		$VARS['phone'] = $VARS['phonecode'].$VARS['phonenum'];
	}
	elseif(!empty($VARS['phonecodecustom']))
	{
		$VARS['phone'] = $VARS['phonecodecustom'].$VARS['phonenum'];
	}

	$this->DB->CheckVariablesFrontend($VARS, $err);

	//--- phone validation ---//
	if(!empty($VARS["phone"])){
		$phoneValidObj = new EkvValidationPhone();
		$phoneValidObj->frontendValidation($VARS["phone"], $err);
		$VARS["phone"] = preg_replace("/^\+/","",$VARS["phone"]);
	}

	//--- email check ---//
	if(!empty($VARS["email"]) && fCheckEmail($VARS["email"]) == false){
		$err["email"] = PROFILE_EMAIL_INCORRECT;
	}
	//check is email free
	elseif(!empty($VARS["email"])  && $userObj->isUserEmailFree($VARS["email"]) == false){
		$err["email"] = PROFILE_EMAIL_BUSY;
	}

	//--- pwd ---//
	if(!empty($VARS["password"]) 
		&& $userObj->isPasswordValid($VARS["password"]) == false
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
	
	//--- captcha ---//
	if(!empty($VARS["captcha"]) && $VARS["captcha"] != $_SESSION["CAPTCHA_CODE"]){
		$err["captcha"] = PROFILE_CAPTCHA_WRONG;
	}
    //--- correct city field ---//
    $userLocationObj->maintainCorrectCityField($VARS);

	//--- insert record ---//
	if(empty($err)){
		$VARS["enabled"] = 0;
		$VARS["password"] = $userObj->generatePwdHash($VARS["password"]);
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
}

// country-city selections using geo
if(!empty($_SERVER['REMOTE_ADDR']))
{
	$res = EkvGeo::getInstance()->correlateCountryCity($_SERVER['REMOTE_ADDR']);
	if(!empty($res->user_country_id)) { $VARS['user_country_id'] = $res->user_country_id; }
	if(!empty($res->user_city_id)) { $VARS['user_city_id'] = $res->user_city_id; }
	if(!empty($res->user_city)) { $VARS['user_city'] = $res->user_city; }
}

// assign phone codes and parse current phone number
$this->TML->assign("phonecodes", EkvValidationPhone::getCountryCodesSel());
if(!empty($VARS['phone']))
{
	$p = EkvValidationPhone::parseNumber($VARS['phone']);
	if(!empty($p->number)){ $VARS['phonenum'] = $p->number; }
	if(!empty($p->code)){ $VARS['phonecode'] = $p->code; }
	$VARS['phoneCodeFromList'] = $p->codeFromList;
}
else
{
	$VARS['phoneCodeFromList'] = 1;
	$VARS['phonecode'] = '+38';
}

$this->TML->assign("vars", $VARS);
$this->Display('common:base/user/page.registration.tpl');

?>
