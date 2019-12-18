<?php
/**
 * @var $this BaseUser
 */

$this->DB->ResetErrors();

$fLoggerObj = new EkvFileLogger("backend_auth");

//pa($this);
$VARS = $this->get_parsed_vars();

//--- form submited ? ---//
if(isset($VARS["login"])){
    //--- login & pwd passed ---//
    if(
        !empty($VARS['login'])
        && !empty($VARS['password'])
    ){
        $VARS['login'] = @trim($VARS['login']);
        $VARS['password'] = @trim($VARS['password']);

        $this->user->Login($VARS['login'], $VARS['password']);

        $log_msg = $VARS['login'] . "\t ip: ".EkvCommonHelper::getCurrentIp()." \t login_res: ";

        //--- user became authed sucessfuly ---//
        if($this->user->id > 0)
        {
            if(
                isset( $this->last_url['path']) &&
                $this->last_url['path'] != $this->login_url &&
                $this->last_url['path'] != $this->logout_url
            )
            {
                 $url = $this->last_url['path'];
            }
            else{
                if(EkvBackendAdmins::isSignedInAdminModer()){
                    $url = "/admin/scheduleMod/tpl/channel_list.html";
                }else{
                    $url = $this->default_url;
                }
            }
            //echo "<h2>redir url = $url</h2>";

            $log_msg .= "success";
            $fLoggerObj->writeStringPlain($log_msg, 1);

            force_location($url);
        }
        //--- incorrect login pwd ---//
        else{
            $this->AddTextError("Некорректный логин-пароль");

            $log_msg .= "failed";
            $fLoggerObj->writeStringPlain($log_msg, 1);
        }
    }else{
        //--- login or pwd not passed ---//
        $this->AddTextError("Введите логин-пароль");
    }
}

$this->Display('common:base/user/page.login.tpl');
