<?php
namespace Ekv\Moderator\MultiConnection;

use Ekv\Helpers\MultiConnection\MultiConnectionDisplay;
use Ekv\Moderator\Moderator;

class Moder2ChannelsDisplay extends MultiConnectionDisplay
{
    const CHECKBOX_NAME = "channels_conn";

    protected function _init()
    {
        parent::_init();
        $this->_jsIdBase = "idModer2Channels";
        $this->_checkBoxName = self::CHECKBOX_NAME;
    }

    /**
     * The result have to be array ('item_id' => "item_name")
     * Possible channels
     * @return array
     */
    protected function _getItemsPossibleRaw()
    {
        $channelObj = new \EkvChannel();
        $channel_options = $channelObj->getChannelOptions();
        return $channel_options;
    }

    /**
     * result have to be palin array of already connected items
     * like array("item1_id", "item2_id")
     * @return array
     */
    protected function _getLinkedItemsRaw()
    {
        $moderObj = new Moderator();
        $conn_raw = $moderObj->getModer2ChannelsConn($this->_linkedToItemID);

        $res = array();
        if(!empty($conn_raw)){
            foreach($conn_raw as $conn_val){
                $res[] = $conn_val["channel_id"];
            }
        }

        return $res;
    }


}
