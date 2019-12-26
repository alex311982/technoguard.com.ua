<?php

use Ekv\Helpers\System\SortOrder;

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

    /**
     * @param string $type
     * @return SortOrder
     */
    public function createSortOrderObj($type)
    {
        $sqlExtra = " AND `direction_type` = '{$type}' ";

        return new SortOrder("decisions", $sqlExtra);
    }
}

