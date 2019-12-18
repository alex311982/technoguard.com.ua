<?php
namespace Ekv\Bank\PresenterConnection\PeriodItem;

use Ekv\Bank\PresenterConnection\Common\PresenterToCommonItemDisplay;
use EkvGlobalHelper, DB;

class PresenterToPeriodItemDisplay extends PresenterToCommonItemDisplay
{
    protected function _init()
    {
        parent::_init();

        $this->_jsIdBase = "idPresenter2PeriodItem";
    }

    protected function _getLinkedItemsRawSpecific()
    {
        return $this->_presenterObj->getPresenterToPeriodItemConnections($this->_linkedToItemID);
    }

}
