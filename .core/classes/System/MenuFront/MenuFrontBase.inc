<?php
namespace Ekv\System\MenuFront;

abstract class MenuFrontBase
{
    const MENU_TABLE = \EkvGlobalHelper::TABLE_MENU_FRONT;

    protected $_shortenMenuFields = " id, parent_id, title, url";

    protected $_displayMenuFields = " id, parent_id, title, url, text_id, hint";

    const SORT_ORDER_DIRECTION = "ASC";

    protected $DB;

    const MENU_TYPE_TOP = 2;
    const MENU_TYPE_MIDDLE = 3;

    protected $_menuType;
    protected $_langID;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    function initPrams($menuType, $langID)
    {
        $this->_menuType = $menuType;
        $this->_langID = $langID;
    }

    function getMenuItem($item_id)
    {
        $item_id = intval($item_id);
        $res = $this->DB->bGetInfoById(self::MENU_TABLE, $item_id);

        return $res;
    }

    private function _getExtCondition()
    {
        $sql_part = "
            AND menu_type = '{$this->_menuType}'
            AND lang_id = '{$this->_langID}'
        ";
        return $sql_part;
    }

    protected function _getPlainRawTreeData($activeOnly = false)
    {
        $sqlIsActive = "";
        if ($activeOnly) {
            $sqlIsActive = " AND enabled = 1 ";
        }

        $sql = "
            SELECT " . $this->_displayMenuFields . "
            FROM `" . self::MENU_TABLE . "`
            WHERE
                1=1
                {$sqlIsActive}
                ".$this->_getExtCondition()."
            ORDER BY
                `sort_order` " . self::SORT_ORDER_DIRECTION . "
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    function getFullTree($filter_is_active = false)
    {
        $SQL_IS_ACTIVE = "";
        if ($filter_is_active) {
            $SQL_IS_ACTIVE = self::getIsActiveSqlConstraint();
        }

        $sql = "
            SELECT " . $this->_displayMenuFields . "
            FROM `" . self::MENU_TABLE . "`
            WHERE
                1=1
                {$SQL_IS_ACTIVE}
                ".$this->_getExtCondition()."
            ORDER BY
                `sort_order` " . self::SORT_ORDER_DIRECTION . "
        ";
        $res = $this->DB->RunSql($sql, true);

        //--- hierarchial ---//
        $tree_hierar = $this->_parseTreeUnified($res, 0, "parent_id", "id");

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

    //--- $rescs - simple plain records for the tree ---//
    protected function _parseTreeUnified(&$recs, $pk_val = 0, $key_parent = "parent_id", $pk_key = "id")
    {
        static $calls_number = 0;
        $calls_number++;
        //echo "<h2>$calls_number</h2>";
        $tree = array();
        foreach ($recs as $key => $rec) {
            if ($rec[$key_parent] == $pk_val) {
                $tree[$key] = $rec;
                unset($recs[$key]);
            }
        }
        foreach ($tree as $key => $rec) {
            $subitems = $this->_parseTreeUnified($recs, $rec[$pk_key], $key_parent, $pk_key);
            if (!empty($subitems)) {
                $tree[$key]['children'] = $subitems;
            }
        }
        return $tree;
    }

    protected function _getIdsFromResult($src_ar)
    {
        $ret = array();
        foreach($src_ar as $val){
            $ret[] = $val["id"];
        }
        return $ret;
    }

    /*
     * $current_id is the most upper node (top_level -> first level -> second level)
     */
	protected function _getDeeperLevelsBranchPath($current_id)
	{
		static $branch = array(); //todo
		//--- get child level ---//
		$nested = $this->DB->bGetInfoById(self::MENU_TABLE, $current_id, "parent_id", true);
		//pa($nested);
		if(!empty($nested)){
			foreach ($nested as $k=>$val){
				$branch[$val["id"]] = $val; //save node
				$this->_getDeeperLevelsBranchPath($val["id"]); //recursive call for nested nodes
			}
		}
		return $branch;
	}

	/*
	 * $current_id - deeppest active node (non top_level > 0)
	 */
	protected function _getHigherLevelBranchPath($current_id, $fields = "")
	{
		if(empty($fields)){
			$fields = $this->_shortenMenuFields;
		}
		static $branch = array(); //todo
		$current_item = $this->DB->BFieldsGetInfoById(
            self::MENU_TABLE, $current_id,
            $key="id", $fields
        );

		if(!empty($current_item)){
			//$current_item["full_url"] = self::returnCorrectMenuUrl($current_item);
			$branch[$current_item["id"]] = $current_item;
			$this->_getHigherLevelBranchPath($current_item["parent_id"], $fields);
		}

		return $branch;
	}


}
 