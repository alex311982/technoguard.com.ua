<?php
/**
 * ============================================================
 * File name: xxx.xxx
 * Date: xx/xx/2010
 * Author: Yegor Konoval
 * Url:	
 * Purpose: 
 * ============================================================
 */
class IndexIndex extends Entity
{
    function __construct()
    {
        parent::__construct();
        if(isset($_REQUEST["_input_id"]) && $_REQUEST["_input_id"] > 0){
            $this->id = (int)$_REQUEST["_input_id"];
        }
    }

    function ManualBeforeExecuteIncludeAction()
    {
        $page_type = $this->definePageType();
        if($page_type == ""){
//            $this->DB->table = EkvNews::TABLE_NEWS;
//            $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "news");
//            //$this->TML->assign("toolbar_action_prefix", "{$page_type}_");
        }
    }

    function definePageType()
    {
        $page_type = "";
//        if(strpos($this->action, "room_") !== false){
//            $page_type = "room";
//        }
        return $page_type;
    }
//
//    function SetBreadcrump()
//    {
//        $page_type = $this->definePageType();
//        $breadcrumps = array();
//        $breadcrumps []= array('title' => "Новости",
//                        'href' => 'browse');
//
//        return $breadcrumps;
//    }
}

?>
