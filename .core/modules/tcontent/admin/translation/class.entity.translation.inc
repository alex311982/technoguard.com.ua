<?php

use Ekv\Helpers\System\Lang;
use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontHelper;
use Ekv\TContent\Banner\BannerRoot;
use Ekv\TContent\Slide\SlideRoot;

class TcontentTranslation extends Entity
{
    public $browseActionTranslation = "translation_list";

    public $PK = "id";

    public $translationID;

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('translation'));

        $this->translationID = @intval($_REQUEST["id"]);

        $this->_prepareBannerFields();
    }

    private function _prepareBannerFields()
    {
        $this->DB->table = 'translations';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "translation");
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "Переводы";
        $breadcrumps []= array('title' => $title,
            'href' => 'translation_list');

        return $breadcrumps;
    }

    /**
     * @return SortOrder
     */
    public function createSortOrderObj()
    {
        return new SortOrder("translations");
    }
}
