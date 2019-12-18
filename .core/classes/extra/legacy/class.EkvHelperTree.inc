<?php
/**
 * User: sunsey
 * Date: 26.01.2011
 */

//-------------------------
// contains common helper methods to work with trees
//-------------------------
class EkvHelperTree
{
//    static public $DB = null;
//    static function initDb($db)
//    {
//        self::$DB = $db;
//    }
    private $DB;
    private $table, $pk_field_name, $parent_field_name;
    function __construct($db, $table = "", $pk_field_name = "id", $parent_field_name = "parent_id")
    {
        $this->DB = $db;
        $this->table = $table;
        $this->pk_field_name = $pk_field_name;
        $this->parent_field_name = $parent_field_name;
    }
    
    //-------------------------
    // marks each item with level, fills options array
    // $raw_rubrics_array  - plain select * array
    // $formater_method_name i.e. buildTreeStructure_Menu
    // &$options - output array with options
    //-------------------------
    function buildTreeStructure($raw_rubrics_array, $formater_method_name, &$options = null, $parent_id = 0, $level = 0){
        static $i = 0;
        static $final_res =
            array(
                "plain_structure" => array(),
            );
        foreach($raw_rubrics_array as &$v){
            $i++;
            if($v[$this->parent_field_name] == $parent_id){
                $v["level"] = $level;
                
                $final_res["plain_structure"][$v["id"]] = $v;

                $display_item_name =  call_user_func( array($this, $formater_method_name), $v);
                $options[$v[$this->pk_field_name]] = str_repeat('&nbsp; &nbsp;', $v["level"])
                . '&bull; ' . $display_item_name;

                $this->buildTreeStructure($raw_rubrics_array, $formater_method_name, $options, $v[$this->pk_field_name], $level+1);
            }
        }
        return $final_res;
    }

    static function buildTreeStructure_Menu(&$raw_data_row)
    {
        $raw_data_row["display_name"] = $raw_data_row["title"];//. "({$raw_data_row["sort_order"]})";
        return $raw_data_row["display_name"]; 
    }

	//--------
	// $current_id - deeppest active node (non top_level > 0)
    // used to fetch breadcrumps for rubrics backend
	//--------
	static function getHigherLevelBranchPath($current_id, $table, $pk_field_name = "id", $parent_field_name = "parent_id", $fields = " * ")
	{
		static $branch = array();
		$current_item = self::$DB->BFieldsGetInfoById(
            $table, $current_id,
            $pk_field_name, $fields
        );
        //pa($current_item);

		if(!empty($current_item)){
			//$current_item["full_url"] = self::returnCorrectMenuUrl($current_item);
			$branch[$current_item["id"]] = $current_item;
			self::getHigherLevelBranchPath(
                $current_item[$parent_field_name],
                $table, $pk_field_name,
                $parent_field_name,
                $fields
            );
		}
		return $branch;
	}
    
}
