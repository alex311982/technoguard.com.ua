<?php
namespace Ekv\B\Base\MenuFrontend;

use Ekv\Helpers\System\SortOrder;

class MMenuFrontCreate extends MMenuFrontSaveCommon
{
    function setAdditionalParams($createForID, $addType)
    {
        $this->_createForID = intval($createForID);
        $this->_addType = $addType;
    }

    function mainSave()
    {
        parent::mainSave();

        $this->_calcParentID();
        $this->_calcSortOrder();

        $textIdObj = new \TextIdGenerator($this->DB, $this->DB->table, "id");
        $this->vars["text_id"] = $textIdObj->getTextIdInUtf8WithDash($this->vars["title"]);

        $this->vars["lang_id"] = $this->_langID;
        $this->vars["menu_type"] = $this->_menuType;

        $id = $this->DB->InsertRecord($this->vars);
        return $id;
    }

    protected function _calcParentID()
    {
        $parent_id = 0;
        $create_for_id = $this->_createForID;

        if ($this->_addType == "child") {
            $parent_id = $create_for_id;
        } elseif ($this->_addType == "sibling") {
            //todo fix
            $create_for_id_info = $this->DB->BGetInfoById($this->DB->table, $create_for_id);
            $parent_id = $create_for_id_info["parent_id"];
        } //--- is passed when creating the first element in the tree ---//
        elseif ($this->_addType == "initial") {
            $parent_id = 0;
        }

        $this->vars["parent_id"] = $parent_id;
        //pa($this->vars);exit;
    }

    protected function _calcSortOrder()
    {
        $SQL_EXTRA = "
            AND `parent_id` = {$this->vars["parent_id"]}

        ";//todo fix

        $sortObj = new SortOrder(\EkvGlobalHelper::TABLE_MENU_FRONT);
        $this->vars["sort_order"] = $sortObj->getNextMaxOrder($SQL_EXTRA);
    }
}
 