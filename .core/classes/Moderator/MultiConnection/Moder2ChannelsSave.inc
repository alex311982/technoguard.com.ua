<?php
namespace Ekv\Moderator\MultiConnection;

use DB;
use Ekv\Helpers\MultiConnection\SaveMultiConnection;

class Moder2ChannelsSave extends SaveMultiConnection
{
    function __construct($master_key_value, $selected_items = null)
    {
        parent::__construct($master_key_value, $selected_items);

        $this->init(
            "moder_id",
            "id_channel",
            "channel_id",
            \EkvGlobalHelper::TABLE_CHANNEL,
            \EkvGlobalHelper::TABLE_MODER_CHANNEL_ACCESS
        );
    }

}
