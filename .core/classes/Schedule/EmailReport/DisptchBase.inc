<?php
namespace Ekv\Schedule\EmailReport;
use Ekv\Schedule\EmailReport\PrepareData;
use EkvGlobalHelper, DB;
use \Ekv\Schedule\ScheduleRoot;

class DisptchBase
{
    //private $DB;

    private $_structuredData;

    const EMAIL_TPL_TYPE_SHORT = 1;
    const EMAIL_TPL_TYPE_EXTENDED = 2;

    private $_emailTplType;

    private $_fLoggerObj;

    private $_singleModePeriodID = false;

    function __construct($structured_data)
    {
        $this->_structuredData = $structured_data;
        //$this->DB = DB::getInstance();
        $this->_fLoggerObj = new \EkvFileLogger("email_reports");
    }

    public function setSingleModePeriodID($singleModePeriodID)
    {
        $this->_singleModePeriodID = $singleModePeriodID;
    }

    private function _getStrucuturedDataForSinglePeriod()
    {
        $res = array();
        if(!empty($this->_structuredData)){
            $all_channel_ids = array_keys($this->_structuredData);
            $channel_id = array_pop($all_channel_ids);
            $res = $this->_structuredData[$channel_id];
        }
        return $res;
    }

    function mainPerformDispatch()
    {
        $log_msg = PHP_EOL . "Dispatch started";
        if($this->_singleModePeriodID){
            $log_msg .= " [single mode periodID #{$this->_singleModePeriodID}]";
        }
        $this->_fLoggerObj->writeStringPlainDense($log_msg);

        foreach($this->_structuredData as $channel_data){
            $this->_emailTplType = self::EMAIL_TPL_TYPE_SHORT;
            $this->_dispatchChannelEmail($channel_data);

            $this->_emailTplType = self::EMAIL_TPL_TYPE_EXTENDED;
            $this->_dispatchChannelEmail($channel_data);
        }

        $this->_fLoggerObj->writeStringPlainDense("Dispatch ended");
    }

    private function _composeSubject($channel_name)
    {
        $date_now = PrepareData::nextDayDate();
        $schedRootObj = new \Ekv\Schedule\ScheduleRoot();
        $date_now_fromated = $schedRootObj->getPeriodNameFromDate($date_now);
        $subject = "Розклад на {$date_now_fromated}. [Канал {$channel_name}]";
        return $subject;
    }

    private function _dispatchChannelEmail($channel_data)
    {
        $channel_id = $channel_data["id_channel"];
        $email_receiver = $this->_emailTplType ==
            self::EMAIL_TPL_TYPE_SHORT ? $channel_data["report_email_short"] : $channel_data["report_email_long"];

        $subject = "";
        //--- change subj for single mode ---//
        if($this->_singleModePeriodID){
            $channel_data = $this->_getStrucuturedDataForSinglePeriod();

            $period_name = @$channel_data["period"]["per_name"];
            $subject = "Розклад на {$period_name}. [Канал {$channel_data["ch_name"]}]";
        }else{
            $subject = $this->_composeSubject($channel_data["ch_name"]);
        }

        $mailerObj = new \EkvPHPMailerLite();
        $mailerObj->AddAddress($email_receiver);
        // 2013-06-03 remove superreceiver
        //$mailerObj->AddCC($channel_data["report_email_all"]); // super receiver
        $mailerObj->Subject = $subject;

        $mailerObj->setMessage($this->getEmailMsgFromTpl($channel_id));
        $mailerObj->Send();

        unset($mailerObj);
    }

    function getEmailMsgFromTpl($channel_id)
    {
        //pa($this->_structuredData[$channel_id]);
        $smartyObj = new \SmartyClass(SITE_FRONTEND);
        $smartyObj->assign("data", $this->_structuredData[$channel_id]);

        //--- show prog descr for long tpl ---//
        if($this->_emailTplType == self::EMAIL_TPL_TYPE_EXTENDED){
            $smartyObj->assign("DISPLAY_TYPE", "extended");
        }
        $msg = $smartyObj->fetch(PATH_TEMPLATES . "/frontend/generics/email/reports_daily.tpl");
        //pa($msg);
        return $msg;
    }

}
