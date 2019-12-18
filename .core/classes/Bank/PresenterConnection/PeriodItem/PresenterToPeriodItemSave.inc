<?php
namespace Ekv\Bank\PresenterConnection\PeriodItem;
use Ekv\Helpers\MultiConnection\SaveMultiConnection;
use EkvGlobalHelper, DB;

class PresenterToPeriodItemSave extends SaveMultiConnection
{
    function __construct($master_key_value, $selected_items = null)
    {
        parent::__construct($master_key_value, $selected_items);

        $this->init(
            "period_item_id",
            "id_presenter",
            "presenter_id",
            EkvGlobalHelper::TABLE_PROG_PRESENTER,
            EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM
        );
    }



}
