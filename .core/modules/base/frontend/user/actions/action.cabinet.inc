<?php
if(__UID > 0 == false){
    force_location_login();
}
//#------------------- CMD -------------------#//
$cmd = isset($_REQUEST["cmd"]) ? $_REQUEST["cmd"] : '';
if($cmd == "subscriptions"){
    require_once "inc.subscriptions.inc";exit;
}


$this->Display('common:base/user/page.cabinet.tpl');
?>