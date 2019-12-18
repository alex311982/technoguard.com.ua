<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
class BaseUser extends Entity
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
        $this->TML->assign("toolbar_action_prefix", "{$page_type}_");
        if($page_type == "base"){
            $this->DB->table = EkvUser::TABLE_USER;
            $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "base");

            //--- user city-country ---//
            $userLocationObj = new EkvUserLocation();
            $city_options = $userLocationObj->getCityOptions();
            $country_options = $userLocationObj->getCountryOptions();
            Entity::prependOptionsWithDefaultValue($country_options);
            $this->setSelectFieldOptions($country_options, "user_country_id", "base_browse");
            Entity::prependOptionsWithDefaultValue($city_options);
            $this->setSelectFieldOptions($city_options, "user_city_id");
        }
    }

    function definePageType()
    {
        $page_type = "";
        if(strpos($this->action, "base_") !== false){
            $page_type = "base";
        }
        elseif(strpos($this->action, "extra_") !== false){
            $page_type = "extra";
        }

        return $page_type;
    }

    function SetBreadcrump()
    {
        $page_type = $this->definePageType();
        $breadcrumps = array();
        $breadcrumps []= array('title' => "Пользователи", 
                        'href' => 'base_browse');


        if($page_type == "base"){    
            if ($this->id > 0){
                $details = $this->DB->BGetInfoById($this->DB->table, $this->id );
                $breadcrumps []= array('title' => "{$details['fio']} - {$details['email']}",
                                        'href' => "base_edit?id={$this->id}"
                                );
            }
        }
        return $breadcrumps;
    }

}

?>
