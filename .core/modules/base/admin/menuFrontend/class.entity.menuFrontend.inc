<?php

use Ekv\Helpers\System\Lang;
use Ekv\System\MenuFront\MenuFrontHelper;

class BaseMenufrontend extends Entity
{
    public $create_for_id = 0;
    public $add_type = "";

    public $langID;
    public $menuType;

    public $RedirectVars = array("create_for_id", "add_type", 'menuType', 'langID');
    public $breadcrumps_additional = array();
    public $nested_level = 1;

    function initButtonsDisplayParamsforBrowseAction()
    {
        //--- buttons display params for browse page ---//
        $shown_buttons =
            array(
                "create" => true,
                "edit" => true,
                "delete" => true,
                "move" => true,
            );
        return $shown_buttons;
    }

    function ManualBeforeExecuteIncludeAction()
    {
        if(EkvBackendAdmins::isSignedInAdminModer()){
            $moderInfo = EkvBackendAdmins::getSessionAdminInfo();
            if(!$moderInfo["has_menu_access"]){
                $this->accessDeniedMsgStop();
            }
        }

        //$this->_initNew('id', "browse");
        $this->browseAction = "browse";
        $this->DB->table = EkvGlobalHelper::TABLE_MENU_FRONT;
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "menu");

        $buttonsDisplayParams = $this->initButtonsDisplayParamsforBrowseAction();
        $this->TML->assign("buttonsDisplayParams", $buttonsDisplayParams);
        //EkvAdminLog::initLogParams($this, "menu_frontend");

        $this->TML->assign("langID", $this->langID);
        $this->TML->assign("menuType", $this->menuType);
    }

    function __construct()
    {
        parent::__construct();

        if (!empty($_REQUEST['_input_id'])) {
            $this->id = (int)$_REQUEST['_input_id'];
        }


        if (@$_REQUEST["id_top"] > 0) {
            $this->id_top = (int)$_REQUEST["id_top"];
        }

        if (@$_REQUEST["id_parent"] > 0) {
            $this->parent_id = (int)$_REQUEST["id_parent"];
        }
    }

    function SetBreadcrump()
    {
        $this->addBcItem("Меню-Index", "index");

        if($this->menuType > 0){
            $menuTypeName = MenuFrontHelper::getMenuTypeName($this->menuType);
            $langObj = Lang::getInstance();
            $langInfo = $langObj->getLangInfo($this->langID);

            $itemName = "Тип:{$menuTypeName} [{$langInfo["lang_code"]}]";
            $this->addBcItem(
                $itemName,
                "browse?menuType={$this->menuType}&langID={$this->langID}"
            );
        }

        return $this->_breadcrumps;
    }

}

