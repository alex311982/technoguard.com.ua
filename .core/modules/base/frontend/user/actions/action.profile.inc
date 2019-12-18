<?php
/**
 * edit profile data
 */

if(__UID > 0 == false){
    force_location_index();
}
$VARS_initial = $this->get_parsed_vars();
$VARS = $this->vars_strip_tags($VARS_initial);
$err = array();

$this->DB->table = EkvUser::TABLE_USER;
$this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "signup");
$this->prepareProfileFields();
$userObj = new EkvUser();

//#------------------- profile pwd change -------------------#//
if(isset($_REQUEST["pwd_change"])){
    require_once "include.profile_pwd.inc";
    exit;
}

//--- country-city ---//
$userLocationObj = new EkvUserLocation();
$country_options = $userLocationObj->getCountryOptions();
$city_options_ukr = $userLocationObj->getCityOptions();
$this->TML->assign("country_options", $country_options );
$this->TML->assign("city_options", $city_options_ukr );
$this->TML->assign("country_ukr", EkvUserLocation::COUNTRY_ID_UKR );
$user_info_full = $userObj->getFullUserInfo(__UID);

if(isset($VARS["is_form_submited"])){
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
	elseif(!empty($VARS["email"])  && $userObj->isUserEmailFree($VARS["email"], $user_info_full["email"]) == false){
		$err["email"] = PROFILE_EMAIL_BUSY;
	}

	//--- pwd ---//
	if(!empty($VARS["password"]) 
		&& $userObj->isPasswordValid($VARS["password"]) == false
	  )
	{
		$err["email"] = "Минимальная длинна пароля ".PROFILE_MIN_PWD_LENGTH." символа(ов)";
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
        $uid = $this->DB->UpdateRecord(__UID, $VARS);
		if($uid > 0){
			$this->TML->assign("show_success_greeting", true);
		}else{
			//$this->TML->assign("insert_res", false);
		}
	}	
	//pa($err);	
	$this->TML->assign("err", $err);
}else{
	$VARS = $user_info_full;
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
$this->Display('common:base/user/page.profile.tpl');

?>
