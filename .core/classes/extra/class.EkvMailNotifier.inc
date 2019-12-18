<?php
class EkvMailNotifierException extends Exception{}

class EkvMailNotifier
{
	public $developerEmail = DEVELOPER_EMAIL;
    private $_templatesPath = "/frontend/generics/email/";
    private $_tplFilename, $_mailObj, $_smartyObj;
    function __construct(SmartyClass $smartyObj = null, $email_tplfile_name = null, $set_admin_receivers = false)
    {
        $this->_templatesPath = PATH_TEMPLATES . $this->_templatesPath;
        $this->_tplFilename = $email_tplfile_name;

        $this->_smartyObj = $smartyObj;
        $this->_mailObj = new EkvPHPMailerLite();

        if($set_admin_receivers){
            $this->setReceiversAdmins();
        }
    }

    private function _getFullTplFname()
    {
        $path = $this->_templatesPath . $this->_tplFilename;
        if(file_exists($path) == false){
            throw new EkvMailNotifierException("Tpl filename doesn't exist '{$path}'");
        }
        return $path;
    }
    function setMailMessage($msg)
    {
        $this->_mailObj->Body = $msg;
    }
    function setMailSubject($subject)
    {
        $this->_mailObj->Subject = $subject;
    }

    function fetchMessageFromTpl($email_params = null)
    {
        if(is_null($this->_smartyObj)){
            throw new EkvMailNotifierException("No Smarty Object");
        }
        if(is_null($email_params) == false){
            $this->_smartyObj->assign("email_params", $email_params);
        }
        $tpl_path = $this->_getFullTplFname();
        $msg = $this->_smartyObj->fetch($tpl_path);
        $this->setMailMessage($msg);
        return $msg;
    }

    function setEmailParams($email_params)
    {
        $this->_smartyObj->assign("email_params", $email_params);
    }

    function addReceiver($email)
    {
        $this->_mailObj->AddAddress($email);
    }

    /**
     * takes str of emails and returns array of valid email
     * @param  $str_input - i.e. "email1@mail.com, email2@mail.com"
     * @param string $separator
     * @return array - valid emails
     */
    private function _getValidEmailsFromStr($str_input, $separator = ",")
    {
        $emails_ar = explode($separator, $str_input);
        $emails_valid = array();
        if(!empty($emails_ar)){
            foreach($emails_ar as $eval){
                $eval = trim($eval);
                if(fCheckEmail($eval)){
                    $emails_valid[] = $eval;
                }
            }
        }
        return $emails_valid;
    }

    /**
     * set first email as receivers, the rest as CC fields
     * @return void
     */
    function setReceiversAdmins($receivers_str = null)
    {
        if(is_null($receivers_str)){
            $receivers_str = ADMIN_EMAIL;
        }
        $valid_emails = $this->_getValidEmailsFromStr($receivers_str);
        if(!empty($valid_emails)){
            //--- add first as receiver, the rest as CC ---//
            $emails_counter = 0;
            foreach($valid_emails as $email){
                $emails_counter++;
                if($emails_counter > 1){
                    $this->_mailObj->AddCC($email);                    
                }else{
                    $this->addReceiver($email);
                }
            }            
        }
        //$this->addReceiversFromStr(ADMIN_EMAIL);
    }

    function setCCReceivers($receivers_str = "")
    {
        $valid_emails = $this->_getValidEmailsFromStr($receivers_str);
        if(!empty($valid_emails)){
            foreach($valid_emails as $email){
                $this->_mailObj->AddCC($email);
            }
        }
    }

    function setCCReceiversAdmins()
    {
        $this->setCCReceivers(ADMIN_EMAIL);
    }

    function addCCDeveloper()
    {
        $email = $this->developerEmail;
        $this->_mailObj->AddCC($email);
    }

    /**
     * ADMIN_EMAIL_MINI - has fewer receiver then  ADMIN_EMAIL
     * @return void
     */
    function setReceiversAdminsMini()
    {
        $this->setReceiversAdmins(ADMIN_EMAIL_MINI);        
    }

    function SendOnly()
    {
        $this->_mailObj->Send();
    }

    function SendAndFetch()
    {
        $this->fetchMessageFromTpl();
        $this->SendOnly();
    }

    static function sendSimpleAdminNotification($subject, $msg, $add_developer_cc = false)
    {
        $notifObj = new self();
        $notifObj->setReceiversAdmins();

        if($add_developer_cc){
            $notifObj->addCCDeveloper();
        }

        $notifObj->setMailSubject($subject);
        $notifObj->setMailMessage($msg);
        $notifObj->SendOnly();
        return true;
    }

    static function sendSimpleAdminNotificationForDeveloper($subject, $msg)
    {
        $notifObj = new self();
        $notifObj->_mailObj->ClearAddresses();
        $notifObj->addReceiver($notifObj->developerEmail);
        $notifObj->setMailSubject($subject);
        $notifObj->setMailMessage($msg);
        $notifObj->SendOnly();
        return true;
    }

    static function oldAnchorReplace($anchor, $message)
    {
        foreach ($anchor as $key => $value){
			$message = str_replace("%$key%", $value, $message);
		}
        return $message;
    }

    /**
     * takes closest events by order id and assigns events_linked var to smarty
     * $events_linked - will be used to display this events in email
     * @param  $order_id
     * @return void
     */
    function setFooterForOrderNotifications($order_id)
    {
        //#------------------- get closest events of the same city -------------------#//
        $orderObj = new EkvRoomEventOrder();
        $order_info = $orderObj->getBaseOrderInfo($order_id);
        if(!empty($order_info)){
            $city_id = $order_info["event_city_id"];

            $SQL_EMAIL_CONDITIONS = EkvRoomEvent::emailSqlCondition();

            $sql = "
                SELECT e.*, r.title as room_name,  vc.city_name, vc.city_text_id
                FROM `".EkvRoomHelper::TABLE_EVENT."` e
                    INNER JOIN `".EkvRoomHelper::TABLE_ROOM."` r
                        ON r.id = e.room_id
                    INNER JOIN `".EkvRoomHelper::TABLE_VERSION_CITY."` vc
                        ON vc.id_city = r.city_id
                WHERE
                    e.enabled = 1
                    AND e.passed = 0
                    {$SQL_EMAIL_CONDITIONS}
                    AND vc.id_city = {$city_id}
                    AND e.id != '{$order_info["event_id"]}'
                ORDER BY
                    e.is_top DESC,
                    e.date ASC
                LIMIT 5
            ";
            $events = EkvCommonHelper::getDB()->RunSql($sql, true);
            $eventObj = new EkvRoomEvent();
            $eventObj->setUSESEPARATESUBDOMAINSFORURLS(true);
            $eventObj->prepareDisplayData($events, true);
            $this->_smartyObj->assign("events_linked", $events); // assign !
            //pa($events);
        }
    }

    function setMailMessageWithImagesEmbeded($msg, $img_dir)
    {
        //#------------------- replaces image pathes with embeded cid -------------------#//
        //$img_dir = PATH_ROOT . DIR_SEP;
        $this->_mailObj->MsgHTML($msg, $img_dir);
    }

    /**
     * main method to send order notification with prepared footer
     * @static
     * @param  $order_id
     * @param  $email_params
     * @param  $tpl_name
     * @param  $receiver_email
     * @param  $subject
     * @return void
     */
    static function sendOrderNotification($order_id, $email_params, $tpl_name, $receiver_email, $subject)
    {
        $smartyObj = new SmartyClass(SITE_FRONTEND);
        //pa(SITE_FRONTEND, $smartyObj);exit;
        
        EkvCommonHelper::setSiteCoreViewFake("frontend");

        $smartyObj->assign("email_params", $email_params);
        
        $notifObj = new EkvMailNotifier($smartyObj, $tpl_name);
        $notifObj->addReceiver($receiver_email);
        $notifObj->setCCReceiversAdmins();
        $notifObj->setMailSubject($subject);
        $notifObj->setFooterForOrderNotifications($order_id);
        //--- proper image displaying ---//
        $msg = $notifObj->fetchMessageFromTpl();
        $notifObj->setMailMessageWithImagesEmbeded($msg, PATH_ROOT);

        EkvCommonHelper::setSiteCoreViewFake(null);

        $notifObj->SendOnly();
    }
}
