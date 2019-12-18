<?php
namespace Ekv\Bank\PresenterConnection\Common;
use Ekv\Bank\ProgPresenter;
use Ekv\Helpers\MultiConnection\MultiConnectionDisplay;
use EkvGlobalHelper, DB;

/**
 * base common class for displaying both presenter to tpl_item or period item connections
 */
abstract class PresenterToCommonItemDisplay extends MultiConnectionDisplay
{
    /**
     * @var ProgPresenter
     */
    protected $_presenterObj;

    function __construct($linked_to_item_id, $engine_action, $request_array, $engine_actions_prefix = null)
    {
        parent::__construct($linked_to_item_id, $engine_action, $request_array);

        if(!is_null($engine_actions_prefix)){
            $this->setEngineActionsAvailableByPrefix($engine_actions_prefix);
        }

        $this->_presenterObj = new ProgPresenter();

        $this->_init();
    }

    protected function _init()
    {
//        $this->_jsIdBase = "idPresenter2TplItem";
        $this->_checkBoxName = "prog_presenters";
    }

    protected function _getItemsPossibleRaw()
    {
        $res = $this->_presenterObj->getPresenterOptions();
        return $res;
    }

    protected function _getLinkedItemsRaw()
    {
        $conn_raw = $this->_getLinkedItemsRawSpecific();

        $conn_final = array();
        foreach($conn_raw as $val){
            $conn_final[] = $val["presenter_id"];
        }

        return $conn_final;
    }

    /**
     * descendant specific method which calls proper method of _presenterObj to not prepared linked items
     * which would be formatted in _getLinkedItemsRaw
     * @return mixed
     */
    abstract protected function _getLinkedItemsRawSpecific();
    //{return $this->_presenterObj->getPresenterToTplItemConnections($this->_linkedToItemID);}
}
