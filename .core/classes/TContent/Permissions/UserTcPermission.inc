<?php
namespace Ekv\TContent\Permissions;

use Ekv\Moderator\Moderator;
use EkvBackendAdmins;

abstract class UserTcPermission
{
    protected $DB;

    /**
     * @var \EkvException
     */
    protected $ex;

    /**
     * @var TcHelper
     */
    protected $tcHelper;

    protected $sessionKey;

    function __construct()
    {
        $this->DB = \DB::getInstance();
        $this->_initException();
        $this->_initOverridable();
    }

    protected function _initException()
    {
        $this->ex = new TcPermissionException();
    }


    /**
     * Set tchelper descendant
     * Set session key $sessionKey
     * @return TcHelper
     */
    abstract protected function _initOverridable();

    /**
     * @param $has_premoderation
     * @param $super_editor
     * @param $moder_info
     * @return TcPermissionContainer - descendant
     */
    abstract protected function _createTcPermContainer($has_premoderation, $super_editor, $moder_info);
//    {
//        $tcPermContainerObj = new TcPermissionContainer(
//            $has_premoderation,
//            $super_editor,
//            $moder_info
//        );
//
//        //pa($newsPermObj->hasPremoderation);
//        return $tcPermContainerObj;
//    }

    private function _getModeTcPermissions($moderID)
    {
        $moderObj = new Moderator();
        $moder_info = $moderObj->getModerInfoById($moderID);

        $this->ex->ensure(!empty($moder_info), "Moder info is empty");

        return $this->_createTcPermContainer(
            $moder_info[$this->tcHelper->hasPremoderationField],
            $moder_info[$this->tcHelper->superEditorField],
            $moder_info
        );
//        $newsPermContainerObj = new NewsPermissionContainer(
//            $moder_info["news_has_premoderation"],
//            $moder_info["news_super_editor"],
//            $moder_info
//        );
//
//        //pa($newsPermObj->hasPremoderation);
//        return $newsPermContainerObj;
    }

    function getPreparedPermissionObj()
    {
        $moderID = EkvBackendAdmins::getSignedInModerID();
        return $this->_getModeTcPermissions($moderID);
    }

    function setTcPermToSession($uid, $role)
    {
        $newsPermContainer = null;
        if($role == EkvBackendAdmins::ROLE_SUPER){
            $newsPermContainer = $this->_getTcPermForCurrentAdmin($uid);
        }elseif($role == EkvBackendAdmins::ROLE_MODER){
            $newsPermContainer = $this->_getModeTcPermissions($uid);
        }

        $this->ex->ensure(!is_null($newsPermContainer), "TcPermissionContainer can't be retrieved");

        $_SESSION[$this->sessionKey] = $newsPermContainer;
    }

    /**
     * @param bool $check_with_exception
     * @return TcPermissionContainer
     */
    function getSessionTcPermContainer($check_with_exception = true)
    {
        $obj = null;
        if($check_with_exception){
            $this->ex->ensure(isset($_SESSION[$this->sessionKey]), "TcPermissionContainer doesn't exist in session");
            $obj = $_SESSION[$this->sessionKey];
        }else{
            if(isset($_SESSION[$this->sessionKey])){
                $obj = $_SESSION[$this->sessionKey];
            }
        }

        return $obj;
    }

    private function _getTcPermForCurrentAdmin($uid)
    {
        $fake_lang_permissions = array(
            $this->tcHelper->langUK => 1,
            $this->tcHelper->langEN => 1,
            $this->tcHelper->langDE => 1,
            $this->tcHelper->langRO => 1,
            $this->tcHelper->langRU => 1
        );

//        $tcPermContainerObj = new NewsPermissionContainer(
//            false,
//            true,
//            $fake_lang_permissions
//        );

        $tcPermContainerObj = $this->_createTcPermContainer(false, true, $fake_lang_permissions);

        return $tcPermContainerObj;
    }
}
 