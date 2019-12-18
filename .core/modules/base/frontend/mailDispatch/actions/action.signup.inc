<?php
	$err = array();
    $VARS = $this->get_parsed_vars();
	MailDispatch::initDb($this->DB);

	if(isset($VARS["is_form_submited"])){
        $VARS["email"] = trim($VARS["email"]);

        if(empty($VARS["email"])){
            $err["email"] = FIELD_EMPTY;
        }elseif( MailDispatch::checkEmailCorrectness($VARS["email"]) == false ){
            $err["email"] = "Неправильний формат email-адреси";
        }elseif(MailDispatch::checkEmailExistance($VARS["email"]) == true){
            $err["email"] = "На цю email-адресу вже ведеться розсилання";
        }

        if(empty($VARS["name"])){
            $err["name"] = FIELD_EMPTY;
        }
        if(empty($VARS["organisation"])){
            $err["organisation"] = FIELD_EMPTY;
        }

        if(empty($VARS["captcha"]) || $VARS["captcha"] != $_SESSION['CAPTCHA_CODE']){
            $err["captcha"] = "Невірний захисний код";
        }

		if(empty($err)){
			$this->TML->assign("finished", true);
            MailDispatch::signupForDispatch($VARS["email"], $VARS["name"], $VARS["organisation"]);
		}
		$this->TML->assign("err", $err);
	}else{
		//$this->force_referer();
	}
    $this->TML->assign("vars", $VARS);
	$this->Display('common:base/mailDispatch/page.signup.tpl');	
?>
