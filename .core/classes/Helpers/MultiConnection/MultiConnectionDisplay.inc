<?php
namespace Ekv\Helpers\MultiConnection;
use EkvGlobalHelper, DB;

/**
 * Base class for all specific multiple connections
 * (i.e. one tpl_item can have a lot of program presenters, or one event can have a lot of genres)
 *
 * First take all possible values which can be linked and depending on engine action take already selected items
 * and impose them on available items. On edit init action we impose already linked values from DB. On save action
 * (after edit or new item is being saved) but error occured - we need to save state for newly selected items so we need
 * to take them from the POST for example.
 *
 * Finally we will get clear structure with posiible linking items and selected items would be marked with a flag so
 * it's easy to display this action in template as html code and use enhacing js plugins.
 *
 * Can be placed into entity action to save state and reflect selected items on possible items.
 *
 * USAGE:
 * $moder2ChannelsObj = new Moder2ChannelsDisplay($this->id, $this->action, $_REQUEST, "moder");
 * $channel_options = $moder2ChannelsObj->mainGetFormatedItems();
 */
abstract class MultiConnectionDisplay
{
    //const CHECKBOX_NAME = "payment_types";

    protected $DB;

    /**
     * checkbox or select name to parse selected states by
     * @var
     */
    protected $_checkBoxName;// = "payment_types";

    /**
     * usefull for building checkbox unique ids for labels
     * @var string
     */
    protected $_jsIdBase;// = "idPaymentTypeFop_";

    /**
     * current engine action - edit, new, save
     * depending on which we define from which source we have to take already selected items (from db or request)
     * @var string
     */
    protected $_engineAction;
    protected $_engineActionsAvailable;

    /**
     * the main item id to which all linking items would be connected
     * (tpl_item_id, or event_id values if proceeding the top example)
     * @var int
     */
    protected $_linkedToItemID;

    /**
     * in most case it's $_REQUEST, but any array for testing can be applied
     * @var array
     */
    protected $_requestArray;

    /**
     * set to true if you need to mark all items as preselected on creation (new) page
     * @var bool
     */
    protected $_selectAllOnCreateAction = false;

    function __construct($linked_to_item_id, $engine_action, $request_array, $engine_action_prefix = null)
    {
        $this->DB = DB::getInstance();
        $this->_linkedToItemID = $linked_to_item_id;
        $this->_engineAction = $engine_action;

        $this->_requestArray = $request_array;

        if(!empty($engine_action_prefix)){
            $this->setEngineActionsAvailableByPrefix($engine_action_prefix);
        }

        $this->_init();
    }

    /**
     * used to avoid whole constructor override
     */
    protected function _init()
    {

    }

    function setEngineActionsAvailable($new, $edit, $save)
    {
        $this->_engineActionsAvailable = array(
            "new" => $new,
            "edit" => $edit,
            "save" => $save
        );
    }

    /**
     * usefull to easyly init actions available
     * @param $prefix
     */
    function setEngineActionsAvailableByPrefix($prefix)
    {
        $this->setEngineActionsAvailable(
            "{$prefix}_new",
            "{$prefix}_edit",
            "{$prefix}_save"
        );
    }

    /**
     * The result have to be array ('item_id' => "item_name")
     * @return array
     */
    abstract protected function _getItemsPossibleRaw();

    /**
     * Result have to be plain array of already connected items
     * like array("item1_id", "item2_id")
     * @return array
     */
    abstract protected function _getLinkedItemsRaw();

    /**
     * depending on action gets linked or request items
     * @return array
     */
    private function _getInputItemsRaw()
    {
        $input_items = array();

        if($this->_engineAction == $this->_engineActionsAvailable["edit"]){
            $input_items = $this->_getLinkedItemsRaw();
        }elseif($this->_engineAction == $this->_engineActionsAvailable["save"]){
            $request_params = isset($this->_requestArray[$this->_checkBoxName]) ? $this->_requestArray[$this->_checkBoxName] : array();
            if(is_array($request_params)){
                $input_items = $request_params;
            }
        }

        return $input_items;
    }

    private function _generateJsId($item_id)
    {
        $res = "{$this->_jsIdBase}_{$item_id}";
        return $res;
    }

    /**
     * formated items for displaying checkboxes
     * @return array
     */
    function mainGetFormatedItems()
    {
        $items_formated = array();
        $items_possible = $this->_getItemsPossibleRaw();

        $input_items = $this->_getInputItemsRaw();
        //pa($items_possible, $input_items);

        foreach($items_possible as $possible_key => $possible_val){
            $js_unique_id = $this->_generateJsId($possible_key);
            $is_checked = false;
            $checked_str = "";
            $selected_str = "";

            //--- current item was previously checked - mark as checked ---//
            if(in_array($possible_key, $input_items)){
                $is_checked = true;
            }

            //--- all checkbox selected for creating action (new) ---//
            if(
                $this->_engineAction == $this->_engineActionsAvailable["new"]
                && $this->_selectAllOnCreateAction
            ){
                $is_checked = true;
            }

            if($is_checked){
                $checked_str = " checked ";
                $selected_str = " selected ";
            }

            $items_formated[$possible_key] =
                array(
                    "id" => $possible_key,
                    "title" => $possible_val,
                    "is_checked" => $is_checked,
                    "checked_str" => $checked_str,
                    "selected_str" => $selected_str,
                    "js_unique_id" => $js_unique_id
                );
        }

        return $items_formated;
    }

}
