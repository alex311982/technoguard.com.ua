<?php

use Ekv\Helpers\System\Lang;
use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontHelper;
use Ekv\TContent\Banner\BannerRoot;
use Ekv\TContent\Slide\SlideRoot;

class TcontentDecision extends Entity
{
    public $browseActionTranslation = "decision_list";

    public $PK = "id";

    public $decisionID;

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('decision'));

        $this->decisionID = @intval($_REQUEST["id"]);

        $this->_prepareDecisionFields();
    }

    private function _prepareDecisionFields()
    {
        $this->DB->table = 'decisions';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "decision");
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "Решения";
        $breadcrumps []= array('title' => $title,
            'href' => 'decision_list');

        return $breadcrumps;
    }
}

