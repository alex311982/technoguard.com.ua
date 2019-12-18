<?php
namespace Ekv\B\Base\MenuFrontend;

use Ekv\System\MenuFront\MenuFrontBackend;

class MMenuFrontEdit extends MMenuFrontSaveCommon
{
    private $_oldInfo;

    private $_itemID;

    public function setItemID($itemID)
    {
        $this->_itemID = $itemID;
    }

    function mainSave()
    {
        parent::mainSave();

        $this->_oldInfo = $this->DB->BGetInfoById($this->DB->table, $this->_itemID);

        if ($this->_oldInfo["title"] != $this->vars["title"]) {
            $textIdObj = new \TextIdGenerator($this->DB, $this->DB->table, "id");
            $this->vars["text_id"] = $textIdObj->getTextIdInUtf8WithDash($this->vars["title"]);
        }

        $id = $this->DB->UpdateRecord($this->_itemID, $this->vars);
        //--- synchronize required menu levels (is_active) ---//
        if (
            $id > 0
            && $this->_oldInfo["enabled"] != $this->vars["enabled"]
        ){

            $menuObj = new MenuFrontBackend();
            $menuObj->synchronizeIsActiveField($id, $this->vars["enabled"]);
            //\MenuFrontend::synchronizeIsActiveField($id, $VARS["is_active"]);
        }

        return $id;
    }



}
 