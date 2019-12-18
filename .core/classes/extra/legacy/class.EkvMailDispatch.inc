<?php
/**
 * User: sunsey
 * Date: 12 ���� 2011
 */

class EkvMailDispatch
{
    const TABLE_DISPATCHES = "base_mail_dispatch",
        TABLE_DISPATCH_TASK = "ekv_mail_dispatch_task",
        TABLE_DISPATCH_TASK_EXTRA = "ekv_mail_dispatch_task_extra"
    ;
    const DISPATCH_FILES_PATH_BASE =  "/files/mail_dispatch/";

    private $DB,
        $_smartyObj // required to perform delivery
    ;
    function __construct($smartyObj = null, DB $db = null)
    {
        if(is_null($db) == false){
            $this->initDB($db);
        }
        else if( is_null($this->DB) ){
            $this->initDB(EkvCommonHelper::getDB());
        }

        if(is_null($smartyObj) == false){
            $this->setSmartyObj($smartyObj);
        }
    }
    function initDB(DB $db)
    {
        $this->DB = $db;
    }
    public function getSmartyObj()
    {
        return $this->_smartyObj;
    }

    public function setSmartyObj($smartyObj)
    {
        $this->_smartyObj = $smartyObj;
    }
    
    function getFullDispatchFilesPath($filename)
    {
        $path = PATH_ROOT . self::DISPATCH_FILES_PATH_BASE . $filename;
        return $path;
    }

    /**
     * performs dispatch with no queue
     * @deprecated
     * @param $dispatch_row_id
     */
    function performDispatchRowById($dispatch_row_id)
    {
        $dispatch_info = $this->getDispatchInfo($dispatch_row_id);
        //return $delivery_info;
        $this->performDispatch($dispatch_info);
    }

    function getDispatchInfo($dispatch_id)
    {
        $dispatch_info = $this->DB->BGetInfoById(self::TABLE_DISPATCHES, $dispatch_id);
        return $dispatch_info;
    }

    /**
     * dispatches mails in real time or if $ref_QUEUE_PARAMS is passed - returns prepared data for using in
     * dispatching from queue
     *  
     * @throws InvalidArgumentException
     * @param  $dispatch_info
     * @param  $ref_QUEUE_PARAMS
     * @return void
     */
    function performDispatch($dispatch_info, &$ref_QUEUE_PARAMS = null )
    {
        if(empty($dispatch_info)){
            throw new InvalidArgumentException();
        }
        $from_name = DISPATCH_FROM_NAME;
        $from_email = DISPATCH_FROM_EMAIL;

        $subject = $dispatch_info['subject'];
        $message = $dispatch_info['body'];
        $message = $this->addHostToWysiwygImages($message);

        $mailObj = new EkvPHPMailerLite();
        $mailObj->CharSet = 'utf-8';
        $mailObj->IsMail(); // telling the class to use native PHP mail()
        $mailObj->SetFrom($from_email, $from_name);

        $mailObj->Subject = $subject;
        $mailObj->AltBody = 'To view the message, please use an HTML compatible email viewer!'; // optional - MsgHTML will create an alternate automatically

        $tpl_email_vars = array(
                        "message_body"		    => $message,
                    );
        $html_contetns = $this->getEmailHtmlContents($tpl_email_vars);
        
        $mailObj->MsgHTML_RootFiles($html_contetns);
//        $mailObj->AddAttachment('w:\home\bilet.dev\www\files\mail_dispatch\delivery_1305201751_4dcbcc5775851.txt',
//                "supername.txt", "base64"
//            );
        //--- attaches required files to mail obj ---//
        $this->addDispatchRowAttachments($dispatch_info, $mailObj);
        
        if($dispatch_info["test_mode"] == 1){

        }

        if(is_null($ref_QUEUE_PARAMS)){
            $receivers = $this->getReceivers($dispatch_info["event_id"], $dispatch_info["shipment_type"], $dispatch_info["test_mode"] );
            //pa($receivers);exit;

            $mails_count = 1;
            foreach($receivers as $receiver_val){
                $receiver_email = $receiver_val["email"];

                $mailObj->ClearAddresses();
                $mailObj->AddAddress($receiver_email);

                echo "email: {$receiver_val["email"]} - {$receiver_val["fio"]} <br />";
                $mailObj->Send();
                $mails_count++;
                if($mails_count % 10){
                    @flush();
                }
            }
            $this->updateDispatchStatus($dispatch_info["id"]);
        }else{
            $ref_QUEUE_PARAMS =
                array(
                    "mail_obj" => $mailObj,
                    "mail_body" => $html_contetns
                );
        }
    }

    function addDispatchRowAttachments($dispatch_row, PHPMailerLite $mailObj)
    {

        $atch_keys = array("file1", "file2", "file3");
        foreach($atch_keys as $at_field_key){
            if(
                array_key_exists($at_field_key, $dispatch_row)
                && !empty($dispatch_row[$at_field_key])
            ){
                $attached_fname = $dispatch_row[$at_field_key];
                $mailObj->AddAttachment($this->getFullDispatchFilesPath($attached_fname), $attached_fname);
            }
        }
    }

    function getEmailHtmlContents($tpl_vars)
    {
        $smartyObj = $this->getSmartyObj();
        if(is_null($smartyObj)){
            throw new Exception();
        }
        $smartyObj->assign("email_vars", $tpl_vars);
//        $tpl_full_path = "common:generics/cron/periodical_dispatch/index.tpl";
//        $smartyObj->assign("content_tpl", "common:generics/cron/periodical_dispatch/i.backend_dispatch.tpl" );

        $tpl_full_path = SmartyClass::getFrontendTemplatesAbsolutePath(
            "generics/cron/periodical_dispatch/index.tpl");
        $content_tpl = SmartyClass::getFrontendTemplatesAbsolutePath(
            "generics/cron/periodical_dispatch/i.backend_dispatch.tpl");
        $smartyObj->assign("content_tpl", $content_tpl);

        $smartyObj->assign("host", EkvCommonHelper::getSiteHostFromConf(true) );
        $html_contents = $smartyObj->fetch($tpl_full_path);
        //$html_contents = $smartyObj->fetch("common:custom_templates/mail_dispatch/email.tpl");
        return $html_contents;
    }

    function getReceivers($event_id, $shipment_type, $test_mode)
    {
        $receiversHelperObj = new EkvMailDispatch_Receivers($event_id, $shipment_type);
        return $receiversHelperObj->mainGetReceivers($test_mode);
    }

    function addHostToWysiwygImages($body)
    {
        return str_replace("/images/ckeditor/",
            "http://".SITE_HOST."/images/ckeditor/",
            $body);
    }

    function updateDispatchStatus($dispatch_id, $status = 'sent')
    {
        $sql = "
            UPDATE `".self::TABLE_DISPATCHES."`
            SET
                `status` = '{$status}'
            WHERE
                `id` = '$dispatch_id'
        ";
        $this->DB->query($sql);
    }
    function setDispatchStatusSent($dispatch_id)
    {
        $this->updateDispatchStatus($dispatch_id, 'sent');
    }

    function getMailDispatchOptions()
    {
        $sql = "
            SELECT id, subject
            FROM `".self::TABLE_DISPATCHES."`
            WHERE
                1
            ORDER BY
                id DESC
        ";
        //$this->DB->RunSql($sql, true);
        $options = array();
        $this->DB->query($sql);
        while($row = $this->DB->fetch_row()){
            $options[$row["id"]] = "{$row["subject"]} [{$row["id"]}]";
        }
        return $options;
    }
}


//#------------------- EkvMailDispatch_Receivers -------------------#//
class EkvMailDispatch_Receivers
{
    private $DB;
    private $_event_id = 0, $_shipment_type = 0;
    function __construct($event_id, $shipment_type, DB $db = null)
    {
        if(is_null($db) == false){
            $this->initDB($db);
        }
        else if( is_null($this->DB) ){
            $this->initDB(EkvCommonHelper::getDB());
        }
        $this->_event_id = $event_id;
        $this->_shipment_type = $shipment_type;
    }
    function initDB(DB $db)
    {
        $this->DB = $db;
    }

    function mainGetReceivers($test_mode)
    {
        $event_id = $this->_event_id;
        $shipment_type = $this->_shipment_type;
        $receivers = array();

        //--- get all active users ---//
        if($test_mode == 1){
            $receivers = $this->getTestEmails();
        }
        else if($event_id == 0 && $shipment_type == 0){
            $receivers = $this->getAllActiveUsers();
        }
        else{
            $receivers = $this->getOrdersUsers($event_id, $shipment_type);
        }
        return $receivers;
    }

    function getTestEmails()
    {
        $final = array();
        $tmp = explode(",", DISPATCH_TEST_EMAILS);
        if(!empty($tmp)){
            foreach($tmp as $email){
                $final[] =
                    array(
                        "email" => $email,
                        "fio" => "[test]"
                    );
            }
        }
        return $final;
    }

    function getOrdersUsers($event_id, $shipment_type)
    {
        $SQL_EVENT_ID = "";
        if($event_id > 0){
            $SQL_EVENT_ID = " AND ob.event_id = $event_id ";            
        }
        
        $SQL_SHIPMENT_TYPE = "";
        if($shipment_type > 0){
            $SQL_SHIPMENT_TYPE = " AND ob.shipment_type = $shipment_type ";
        }        
        /*
        // ob.email as order_email, ob.fio as order_fio,
        $sql = "
            SELECT u.email as user_email, u.fio as user_fio                 
            FROM `".EkvRoomHelper::TABLE_ORDER_BASE."` ob
                INNER JOIN `".EkvRoomHelper::TABLE_USERS."` u
                    ON u.id = ob.uid
            WHERE
                1
                $SQL_EVENT_ID
                -- AND ob.order_status = ".EkvRoomHelper::ORDER_STATUS_SUCCEED."
                $SQL_PAYMENT_TYPE
        ";
        */
        
        $sql = "
            SELECT u.email, u.fio
            FROM `".EkvRoomHelper::TABLE_USERS."` u
            WHERE
                u.id IN
                (
                    SELECT distinct ob.uid
                    FROM `".EkvRoomHelper::TABLE_ORDER_BASE."` ob
                    WHERE
                        1
                        $SQL_EVENT_ID
                        AND ob.order_status = ".EkvRoomHelper::ORDER_STATUS_SUCCEED."
                        $SQL_SHIPMENT_TYPE
                        
                )
        ";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    function getAllActiveUsers()
    {
        $sql = "
            SELECT email, fio
            FROM `".EkvRoomHelper::TABLE_USERS."`
            WHERE
                `enabled` = 1
                AND `dispatch_subscriber` = 1
            ORDER BY
                `id` ASC
        ";
        $res = $this->DB->RunSql($sql, true);

        return $res;
    }
    

    
}