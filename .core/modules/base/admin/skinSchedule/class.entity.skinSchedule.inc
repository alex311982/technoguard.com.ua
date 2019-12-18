<?php

class BaseSkinSchedule extends Entity
{
    public $PK = 'id_row';
    public $breadcrumps_additional = array();

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
        if(EkvBackendAdmins::isSignedInAdminSuper() == false){
            $this->accessDeniedMsgStop();
        }

        $this->browseAction = "browse";
        $this->DB->table = EkvGlobalHelper::TABLE_SKIN_SCHEDULE;
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "skin_sched");

        $buttonsDisplayParams = $this->initButtonsDisplayParamsforBrowseAction();
        $this->TML->assign("buttonsDisplayParams", $buttonsDisplayParams);
    }

    function __construct()
    {
        parent::__construct();

        if (!empty($_REQUEST['_input_id'])) {
            $this->id = (int)$_REQUEST['_input_id'];
        }
    }

    function SetBreadcrump()
    {
        $this->addBcItem("Розклад тем сайту", "browse");

        return $this->_breadcrumps;
    }

    public function fixDatesOrder(&$vars)
    {
        $dateFrom = @strtotime($vars["dt_begin"]);
        $dateTo = @strtotime($vars["dt_end"]);

        if ($dateFrom > $dateTo) {
            $tmp = $vars["dt_begin"];
            $vars["dt_begin"] = $vars["dt_end"];
            $vars["dt_end"] = $tmp;
        }
    }

}

