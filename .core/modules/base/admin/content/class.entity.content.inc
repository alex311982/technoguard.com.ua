<?php

class BaseContent extends Entity
{
    public $useWysiwyg = 0;
    public $RedirectVars = array('useWysiwyg');
    
    function __construct()
    {
        parent::__construct();
        if(!empty($_REQUEST['_input_id'])) $this->id = (int)$_REQUEST['_input_id'];
    }

    function ManualBeforeExecuteIncludeAction()
    {
        if($this->useWysiwyg == 1){
            $this->DB->fields["text"]["type"] = 'html';
        }
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();
        $breadcrumps []= array('title' => "Контент-модуль",
                    'href' => 'browse');
        if($this->id > 0)
        {
            $details = $this->DB->RetrieveRecord((int)$this->id);
            $breadcrumps []= array('title' => $details['record']['key']['value'],
                        'href' => 'edit?id='.$details['record']['id']['value']);

        }
        elseif( ($this->action == 'new') || ($this->action == 'save') )
        {
            $breadcrumps []= array('title' => "Создать новый пункт",
                        'href' => 'new');
        }
        return $breadcrumps;
    }
}

?>
