<?php
if (isset($VARS["is_form_submited"])) {
    $password = @$VARS["password"];
    $password_confirm = @$VARS["password_confirm"];

    if (empty($password)) {
        $err["password"] = FIELD_EMPTY;
    } elseif ($userObj->isPasswordValid($password) == false) {
        $err["password"] = "Минимальная длинна пароля " . PROFILE_MIN_PWD_LENGTH . " символа(ов)";
    }

    if (empty($password_confirm)) {
        $err["password_confirm"] = FIELD_EMPTY;
    }

    if (
        !empty($password) && !empty($password_confirm)
        && $password != $password_confirm
    ) {
        $err["password"] = "Пароль и подтверждение не совпадают";
    }


    if (empty($err)) {
        $new_pwd_hash = $userObj->generatePwdHash($password);
        $userObj->updateUserPassword(__UID, $new_pwd_hash);
        $this->TML->assign("show_success_greeting", true);
    }
}


$this->TML->assign("err", $err);
$this->TML->assign("vars", $VARS);
$this->Display('common:base/user/page.profile_pwd.tpl');
?>