<?php
namespace Ekv\System\MenuFront;

class MenuFrontBackend extends MenuFrontBase
{
    function correctMenuItemDelete($menu_id)
    {
        $current_item = $this->DB->bFieldsGetInfoById(self::MENU_TABLE, $menu_id);

        if(!empty($current_item)){
            $child_ids = $this->_getIdsFromResult(
                    $this->_getDeeperLevelsBranchPath($menu_id)
                );
            $child_ids[] = $menu_id;

            $ids_str = implode(",", $child_ids);

            //delete
            $sql = "
                DELETE FROM `".self::MENU_TABLE."`
                WHERE
                    `id` IN ({$ids_str})
            ";
            $this->DB->query($sql);
        }
    }

    function synchronizeIsActiveField($current_menu_id, $enabled)
    {
        $levels_to_process = array();
        // became unchecked -> unchek deeper levels
        if($enabled == 0){
            $levels_to_process = $this->_getDeeperLevelsBranchPath($current_menu_id);
        }
        // became checked -> chek upper levels
        else{
            $levels_to_process = $this->_getHigherLevelBranchPath($current_menu_id);
        }

        if(!empty($levels_to_process)){
            $level_ids = $this->_getIdsFromResult($levels_to_process);
            $level_ids_str = implode(",", $level_ids);

            $sql = "
                UPDATE `".self::MENU_TABLE."`
                SET
                    `enabled` = '$enabled'
                WHERE
                    `id` IN ({$level_ids_str})
            ";

            $this->DB->query($sql);
        }
    }

    function getFullTree($filter_is_active = false)
    {
        $plainTree = $this->_getPlainRawTreeData(false);

        //--- hierarchial ---//
        $tree_hierar = $this->_parseTreeUnified($plainTree, 0, "parent_id", "id");

        //--- get formated js tree table ---//
        $jq_tree_obj = new ParseRawDataForJqTree($tree_hierar, $this->_menuType, $this->_langID);
        $jq_data = $jq_tree_obj->processConvertation();

        $full_tree =
            array(
                "hierarchial" => $tree_hierar,
                "jq" => $jq_data,
            );

        return $full_tree;
    }
}
 