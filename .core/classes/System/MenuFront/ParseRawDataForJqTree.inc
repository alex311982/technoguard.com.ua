<?php
namespace Ekv\System\MenuFront;

class ParseRawDataForJqTree
{
    public $hierarchialData;
    public $finishedData;

    private $_menuType;
    private $_langID;

    private $requiredFields =
        array(
            "id", "title", "parent_id",
            "url",
//            "enabled"
        );

    function __construct($hierarchial, $menuType, $langID)
    {
        $this->hierarchialData = $hierarchial;

        $this->_menuType = intval($menuType);
        $this->_langID = intval($langID);
    }

    function parse(&$data)
    {
        foreach ($data as $dk => $dval) {
            $finished_row = $this->saveOnlyRequiredFields($dval);

            $finished_row["js_info"] = array(
                "node" => "node-{$dval["id"]}",
                "child_of_node" => ($dval["parent_id"] > 0) ? "child-of-node-{$dval["parent_id"]}" : "",
            );
            $finished_row["urls"] = $this->formUrlsData($dval["id"]);
            //$finished_row["urls"]["frontend_url"] = MenuFront::returnCorrectMenuUrl($dval);
            $this->finishedData[] = $finished_row;

            if (isset($dval["children"])) {
                $this->parse($dval["children"]);
            }
        }
    }

    function formUrlsData($id)
    {
        $commonSuffixUrl = "&menuType={$this->_menuType}&langID={$this->_langID}";

        $arr = array(
            "new_sibling" => "/admin/base/menuFrontend/new.html?create_for_id={$id}&add_type=sibling{$commonSuffixUrl}",
            "new_child" => "/admin/base/menuFrontend/new.html?create_for_id={$id}&add_type=child{$commonSuffixUrl}",
            "edit" => "/admin/base/menuFrontend/edit.html?_input_id={$id}{$commonSuffixUrl}",
            "delete" => "/admin/base/menuFrontend/delete.html?_input_id={$id}{$commonSuffixUrl}",
            "move_up" => "/admin/base/menuFrontend/exchange_order.html?item_id={$id}&direction=up{$commonSuffixUrl}",
            "move_down" => "/admin/base/menuFrontend/exchange_order.html?item_id={$id}&direction=down{$commonSuffixUrl}",
        );
        return $arr;
    }

    function saveOnlyRequiredFields(&$row)
    {
        $ret = array();
        foreach ($this->requiredFields as $val) {
            $ret[$val] = $row[$val];
        }
        return $ret;
    }

    function processConvertation()
    {
        $this->parse($this->hierarchialData);
        return $this->finishedData;
    }
}
 