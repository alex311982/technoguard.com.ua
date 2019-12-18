<?php
namespace Ekv\Bank\PresenterConnection\TplItem;
use Ekv\Bank\PresenterConnection\Common\PresenterToCommonItemDisplay;
use EkvGlobalHelper, DB;

class PresenterToTplItemDisplay extends PresenterToCommonItemDisplay
{
    protected function _init()
    {
        parent::_init();

        $this->_jsIdBase = "idPresenter2TplItem";
        //$this->_checkBoxName = "prog_presenters";
    }

    protected function _getLinkedItemsRawSpecific()
    {
        return $this->_presenterObj->getPresenterToTplItemConnections($this->_linkedToItemID);
    }

}
