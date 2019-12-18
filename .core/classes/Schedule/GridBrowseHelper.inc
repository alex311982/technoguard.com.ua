<?php
namespace Ekv\Schedule;

use Ekv\B\ScheduleMod\Grid\GridBrowsePresentersFix;
use Ekv\B\ScheduleMod\Tpl\TplBrowsePresenters;

class GridBrowseHelper
{
    private $type;

    /**
     * @param $type - 'tpl' | 'grid'
     */
    function __construct($type)
    {
        $this->type = $type;
    }

    function main($result)
    {
        if (!empty($result["records"])) {
            if ($this->type == 'tpl') {
                $presApplier = new TplBrowsePresenters($result["records"]);
                $presApplier->mainApplyPresenters();
            } elseif ($this->type == 'grid') {
                $presApplier = new GridBrowsePresentersFix($result["records"]);
                $presApplier->mainApplyPresenters();
            }

            foreach ($result["records"] as &$rec_val) {
                $res = array(
                    'has_descr' => !empty($rec_val["prog_descr"]["value"]),
                    'has_photo' => !empty($rec_val["prog_photo"]["value"]),
                    'period_item_id' => $rec_val["id_item"]["value"],
                );

                $rec_val["man_existence"]["extra"] = $res;
            }
        }

        return $result;
    }

    static function getPeriodPreviewUrl($channel_id, $period_id)
    {
        $url = "/grid/channel/period/items/list.html?periodID={$period_id}&channelID={$channel_id}";
        return $url;
    }
}
