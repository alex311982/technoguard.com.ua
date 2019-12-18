<?php
namespace Ekv\Moderator;
use EkvGlobalHelper, DB, EkvBackendAdmins;
use \Ekv\Schedule\ScheduleTpl;
use \Ekv\Schedule\ScheduleRoot;

/**
 * supermoder is allowed for all channels and other moders only for their own white list
 */
class ModeratorCheckAccess
{
    private $DB;
    private $_authInfo;

    function __construct()
    {
        $this->DB = DB::getInstance();
        $this->_authInfo = EkvBackendAdmins::getSessionAdminInfo();
    }

    function getAuthItem($key)
    {
        $res = isset($this->_authInfo[$key]) ? $this->_authInfo[$key] : null;
        return $res;
    }

    function isModerSuper()
    {
        $res = $this->getAuthItem("is_moder_super");
        $res = (bool)$res;
        if($res == false){
            $res = EkvBackendAdmins::isSignedInAdminSuper();
        }

        return $res;
    }

//    /**
//     * @deprecated
//     * @return int
//     */
//    function getChannelId()
//    {
//        return $this->getAuthItem("moder_channel_id");
//    }

    function getChannelIdsAllowed()
    {
        static $channel_ids = null;
        //--- cache ---//
        if(is_null($channel_ids)){
            $tmp = $this->getAuthItem("channels_allowed");

            if(!empty($tmp)){
                $channel_ids = $tmp;
            }else{
                $channel_ids = array();
            }
        }
        return $channel_ids;
    }

    function checkChannelInAllowedIds($channel_id)
    {
        return in_array($channel_id, $this->getChannelIdsAllowed());
    }

    function accessDeniedMsg($msg = null)
    {
        if(is_null($msg)){
            $msg = "<h2>Доступ заборонено</h2>";
        }
        echo $msg;
        exit;
    }

    /**
     * Main method to check channel access
     * Supermoder can access all channels other moders - only allowed for them
     * @param $channel_id
     */
    function checkChannelAccess($channel_id)
    {
        if(
            $this->isModerSuper() == false
            //&& $this->getChannelId() != $channel_id
            && $this->checkChannelInAllowedIds($channel_id) == false
        ){
            $this->accessDeniedMsg();
        }
    }

    function checkBankProgAccess()
    {
        $bank_prog_manage = $this->getAuthItem("bank_prog_manage");
        if(
            $this->isModerSuper() == false
            && $bank_prog_manage == 0
        ){
            $this->accessDeniedMsg();
        }
    }

    function checkBankPresenterAccess()
    {
        $bank_presenter_manage = $this->getAuthItem("bank_presenter_manage");
        if(
            $this->isModerSuper() == false
            && $bank_presenter_manage == 0
        ){
            $this->accessDeniedMsg();
        }
    }

    function checkTplIdAccess($tpl_id)
    {
        $schedTplObj = new ScheduleTpl();
        $tpl_info = $schedTplObj->getTplPlusChannelInfo($tpl_id);
        if(!empty($tpl_info)){
            $this->checkChannelAccess($tpl_info["channel_id"]);
        }
    }

    function checkPeriodIdAccess($period_id)
    {
        $schedRootObj = new ScheduleRoot();
        $period_info = $schedRootObj->getPeriodInfo($period_id);
        if(!empty($period_info)){
            $this->checkChannelAccess($period_info["channel_id"]);
        }
    }

}
