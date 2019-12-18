<?php

class EkvPHPMailerLite extends PHPMailerLite
{
    //public $AltBody = 'To view the message, please use an HTML compatible email viewer!';
    public function __construct() {
        parent::__construct();
        $this->CharSet = 'utf-8';
        $this->IsMail(); // telling the class to use native PHP mail()
        $this->SetFrom("no-reply@schedule.nrcu.gov.ua", "Schedule-NRCU");
        $this->Subject = 'schedule.nrcu.gov.ua';
        $this->AltBody = 'To view the message, please use an HTML compatible email viewer!'; // optional - MsgHTML will create an alternate automatically
    }

    function setMessage($message)
    {
        $this->Body = $message;
    }

    function setSubject($subj)
    {
        $this->Subject = $subj;
    }

    /**
     * set html message and set PATH_ROOT as root for all files in email tpl
     * so if you use <img src="/files/1.img"> this image would be EMBEDED to the email
     * can be used for small or system images
     * is used at email dispatch
     * @param $msg
     */
    function MsgHTML_RootFiles($msg)
    {
        parent::MsgHTML($msg, PATH_ROOT . DIR_SEP);
    }

    function addTestReceivers($just_me = true)
    {
		$this->AddAddress(DEVELOPER_EMAIL);
        $this->AddAddress("quadroval@gmail.com");
        $this->AddAddress("sunseyblade@mail.ru");
        $this->AddAddress("sunseyblade@i.ua");
        $this->AddAddress("quadroval@rambler.ru");
        $this->AddAddress("quadroval@bigmir.net");
        $this->AddAddress("sunseyblade@ukr.net");
        $this->AddAddress("sunseyblade@yahoo.com");
        $this->AddAddress("sunseyblade@yandex.ru");
        if(!$just_me){
            $this->AddAddress("avdoshyn@gmail.com");
        }
    }
}
