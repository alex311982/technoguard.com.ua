<?php
namespace Ekv\Helpers\System;

class SortOrder
{
    private $DB;

    private $_table;
    private $_sortOrderOffset = 2;
    private $sqlExtra;

    function __construct($table, $sqlExtra = "")
    {
        $this->DB = \DB::getInstance();
        $this->_table = $table;
        $this->sqlExtra = $sqlExtra;
    }

    private function chooseProperSqlExtra(&$sqlExtra)
    {
        if (empty($sqlExtra) && !empty($this->sqlExtra)) {
            $sqlExtra = $this->sqlExtra;
        }
    }

    function getNextMaxOrder($SQL_EXTRA = "")
    {
        $this->chooseProperSqlExtra($SQL_EXTRA);

        $sql = "
            SELECT MAX(`sort_order`) as `max_order`
            FROM `{$this->_table}`
            WHERE
                1
                $SQL_EXTRA
        ";
        $row = $this->DB->RunSql($sql);

        return $row['max_order'] + $this->_sortOrderOffset;
    }

    //--------
    // $direction  - up | down
    // $sort_order - ASC | DESC(invert $direction in case of DESC)
    //--------
    private function _getExchangedIds($record_id, $direction, $sort_order = "ASC", $pk_name = "id", $SQL_EXTRA = "")
    {
        $this->chooseProperSqlExtra($SQL_EXTRA);

        $str_fields = " `$pk_name`, `sort_order` ";

        //--- get current item info ---//
        $sql = "
            SELECT $str_fields
            FROM `$this->_table`
            WHERE
                `$pk_name` = '$record_id'
        ";
        $current_item = $this->DB->RunSql($sql);
        //pa($current_item);

        $ret_val = array(
            "from" => "",
            "to" => ""
        );

        //--- if sort_order is DESC - invert $direction AND SIGN to do correct searching adjacent item ---//
        $COMPARE_SIGN = "";
        if($sort_order == "DESC"){
            if($direction == "up"){
                $direction = "down";
                $COMPARE_SIGN = " > ";
            }elseif($direction == "down"){
                $direction = "up";
                $COMPARE_SIGN = " < ";
            }
        }

        $SQL_SORT_ORDER_SIGN = null;
        $SQL_SORT_ORDER_TYPE = null;

        switch ($direction){
            case "up":
                $SQL_SORT_ORDER_SIGN = (!empty($COMPARE_SIGN))? $COMPARE_SIGN : " < " ;
                $SQL_SORT_ORDER_TYPE = "DESC";
            break;
            case "down":
                $SQL_SORT_ORDER_SIGN = (!empty($COMPARE_SIGN))? $COMPARE_SIGN : " > ";
                $SQL_SORT_ORDER_TYPE = "ASC";
            break;
        }

        //--- get record with bigger sort_order ---//
        $sql = "
            SELECT $str_fields
            FROM `{$this->_table}`
            WHERE
                1=1 AND
                `sort_order` $SQL_SORT_ORDER_SIGN '{$current_item["sort_order"]}'
                $SQL_EXTRA
            ORDER BY
                `sort_order` $SQL_SORT_ORDER_TYPE
            LIMIT 1
        ";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $changing_item = $this->DB->RunSql($sql);

        //pa($changing_item);
        if(!empty($changing_item)){
            $ret_val["from"] = $current_item[$pk_name];
            $ret_val["to"] = $changing_item[$pk_name];
            return $ret_val;
        }else{
            return "none";
        }
    }

    //--------
    // pk_name - primary key
    //--------
    private function _doExchange($exchange_ids, $pk_name = "id")
    {
        //$pk_name = "id_channel";
        $tmp = $this->DB->BGetInfoById($this->_table,  $exchange_ids["from"], $pk_name);
        $from_order = $tmp["sort_order"];
        $tmp1 = $this->DB->BGetInfoById($this->_table,  $exchange_ids["to"], $pk_name);
        $to_order = $tmp1["sort_order"];

        ///from
        $sql = "
            UPDATE `{$this->_table}`
            SET `sort_order` = '$from_order'
            WHERE
                `$pk_name` = '".$exchange_ids["to"]."' ";
        $this->DB->query($sql);

        ///to
        $sql = "
            UPDATE `{$this->_table}`
            SET `sort_order` = '$to_order'
            WHERE
                `$pk_name` = '".$exchange_ids["from"]."' ";
        $this->DB->query ($sql);
    }

    function performExchange($record_id, $direction, $sort_order = "ASC", $pk_name = "id", $SQL_EXTRA = "")
    {
        $this->chooseProperSqlExtra($SQL_EXTRA);

        $exchanged_ids = $this->_getExchangedIds(
            intval($record_id),
            $direction,
            $sort_order,
            $pk_name,
            $SQL_EXTRA
        );

        if($exchanged_ids != "none"){
            $this->_doExchange($exchanged_ids, $pk_name);
            return true;
        }

        return false;
    }


}
 