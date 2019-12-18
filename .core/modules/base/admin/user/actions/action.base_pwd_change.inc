<?php
    $this->DB->ResetErrors();
    $VARS = $this->get_parsed_vars();
    //pa($VARS);
    //pa($this->id);

    if(isset($VARS["is_form_submited"])){
        $userObj = new EkvUser();
        $new_pwd = trim($VARS["pwd"]);
        if(empty($new_pwd)){
            $this->DB->AddTextError("Введите новый пароль");   
        }else{
            $pwd_hash = $userObj->generatePwdHash($new_pwd);
            $userObj->updateUserPassword($this->id, $pwd_hash);

            $this->ChangeLocation("base_browse");
        }

    }

	//$this->TML->assign('result', $result);
	$this->Display('common:custom_templates/user/page.pwd_change.tpl');

?>