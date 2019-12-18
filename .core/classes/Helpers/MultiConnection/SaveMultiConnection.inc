<?php
namespace Ekv\Helpers\MultiConnection;
use EkvGlobalHelper, DB;

/**
 * Used to avoid all the time duplicating code of saving multple connections
 *
 * MASTER - to which items are connected (one of conn table keys)
 * - staff connected to progs, prog is master
 * MULTI (the reset of vars, multi prefix is omitted)
 * - many items are connected to master (many employees or presenters are connected to prog)
 *
 * ----------------- EXAMPLE
 * `tbl_progs`
 * id_prog | prog_name
 *
 * `tbl_staff`
 * id_staff | staff_name
 *
 * `tbl_prog2staff`
 * id | prog_id | staff_id
 *
 * set connections for prog_id = 23
 */
abstract class SaveMultiConnection
{
    protected $DB;

    // ---->>> this must be set manually as  input params

    private $_MASTER_KEY_NAME; //prog_id (the other conn key from conn table)
    private $_MASTER_KEY_VALUE; // prog_id=23

    private $ORIG_KEY_NAME; // id_staff - PK of staff table
    private $_CONN_KEY_NAME; // staff_id - key in staff2prog table

    protected $_TABLE_ORIG; //staff
    protected $_TABLE_CONN; //staff2prog

    // <<<------------

    //#------------------- below are for internal use -------------------#//
    private $_MASTER_KEY_VALUE_SAFE;
    protected $_ORIG_KEY_ALIAS = 'item_id';
    protected $_CONN_KEY_ALIAS = 'linked_item_id';

    /**
     * the selected ids from $_REQUEST which are being saved
     * have to be taken from $_REQUEST manually
     * @var
     */
    protected $_requestSelectedItems = array();

    protected $_itemsConnected;
    protected $_deleteIds;
    protected $_insertIds;

    /**
     * @param $master_key_value
     * @param null $selected_items - may be passed directly or set via defineAndSetRequestSelectedItems()
     * @see defineAndSetRequestSelectedItems
     */
    function __construct($master_key_value, $selected_items = null)
    {
        $this->DB = DB::getInstance();

        $this->_MASTER_KEY_VALUE = $master_key_value;
        $this->_MASTER_KEY_VALUE_SAFE = mysqli_real_escape_string($this->DB->connection_id, $master_key_value);

        if(
            !empty($selected_items)
            && is_array($selected_items)
        ){
            $this->_requestSelectedItems;
        }
    }

    function init($master_key_name, $orig_key_name, $conn_key_name, $tbl_orig, $tbl_conn)
    {
        $this->_MASTER_KEY_NAME = $master_key_name;
        $this->ORIG_KEY_NAME = $orig_key_name;
        $this->_CONN_KEY_NAME = $conn_key_name;
        $this->_TABLE_ORIG = $tbl_orig;
        $this->_TABLE_CONN = $tbl_conn;
    }

    function mainSave()
    {
        $this->_itemsConnected = $this->_getItemsConnected();

        $this->_fillDeleteAndInsertIds();

        $this->_deleteRequiredIds();
        $this->_insertRequiredIds();
    }

    function defineAndSetRequestSelectedItems($checkbox_name)
    {
        if(isset($_REQUEST[$checkbox_name])){
            $this->_requestSelectedItems = $_REQUEST[$checkbox_name];
            if(!is_array($this->_requestSelectedItems)){
                $this->_requestSelectedItems = array();
            }
        }
    }

    protected function _getItemsConnected()
    {
        //return array();

//        $sql = "
//            SELECT tb1.id_staff as item_id, tb2.staff_id as linked_item_id, tb1.fio
//            FROM `".EkvGlobalHelper::TABLE_STAFF."` tb1
//                LEFT JOIN `".$TABLE_LINKER."` tb2
//                    ON tb1.id_staff = tb2.staff_id
//                        AND tb2.prog_id = '$prog_id'
//        ";

        $sql = "
            SELECT
                tb1.{$this->ORIG_KEY_NAME} as `{$this->_ORIG_KEY_ALIAS}`,
                tb2.{$this->_CONN_KEY_NAME} as `{$this->_CONN_KEY_ALIAS}`
            FROM `{$this->_TABLE_ORIG}` tb1
                LEFT JOIN `{$this->_TABLE_CONN}` tb2
                    ON tb1.{$this->ORIG_KEY_NAME} = tb2.{$this->_CONN_KEY_NAME}
                        AND tb2.{$this->_MASTER_KEY_NAME} = '{$this->_MASTER_KEY_VALUE_SAFE}'
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";

        //echo "\n <pre> sql = $sql </pre> <br/>\n";exit;
        $items = $this->DB->RunSql($sql, true);
        return $items;
    }

    private function _fillDeleteAndInsertIds()
    {
        if(empty($this->_itemsConnected)){
            return false;
        }

        $items_checked = array();
        // TODO fix
//        if(isset($_REQUEST[$this->_CHEKBOX_NAME])){
//            $items_checked = $_REQUEST[$this->_CHEKBOX_NAME];// selected checkboxes values for specific prog
//        }

        if(!empty($this->_requestSelectedItems)){
            $items_checked = $this->_requestSelectedItems;
        }

        foreach($this->_itemsConnected as $val){
            $linked_item_id = $val[$this->_CONN_KEY_ALIAS];
            $item_id = $val[$this->_ORIG_KEY_ALIAS];
            //--- was checked but now not checked ---//
            if(
                $linked_item_id > 0
                && in_array($linked_item_id, $items_checked) == false
            ){
                $this->_deleteIds[] = $item_id;
            }
            elseif(
                $linked_item_id > 0 == false
                && in_array($item_id, $items_checked)
            ){
                $this->_insertIds[] = $item_id;
            }
        }
        //pa($this->_insertIds, $this->_deleteIds);exit;

        return true;
    }

    private function _deleteRequiredIds()
    {
        //--- delete only if it's required ---//
        if(!empty($this->_deleteIds)){
            $delete_ids_str = implode(',', $this->_deleteIds);

            $sql = "
                DELETE FROM `".$this->_TABLE_CONN."`
                WHERE
                    `{$this->_MASTER_KEY_NAME}` = '{$this->_MASTER_KEY_VALUE_SAFE}'
                    AND `{$this->_CONN_KEY_NAME}` IN({$delete_ids_str})
            ";
            $this->DB->query($sql);
        }
    }

    private function _insertRequiredIds()
    {
        if(!empty($this->_insertIds)){
            $ins_parts = array();
            foreach($this->_insertIds as $ins_val){
                $ins_parts[] = "('{$this->_MASTER_KEY_VALUE_SAFE}', $ins_val)";
            }
            $ins_parts_str = implode(',', $ins_parts);
            $sql = "
                INSERT INTO `{$this->_TABLE_CONN}`
                (`{$this->_MASTER_KEY_NAME}`, `{$this->_CONN_KEY_NAME}`)
                VALUES
                    {$ins_parts_str}
            ";
            $this->DB->query($sql);
        }
    }



}
