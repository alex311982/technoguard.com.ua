<?php
namespace Ekv\Schedule\Formatting;

/**
 * Used to support old exisitng urls till they would be shut down
 */
class PeriodItemFormatterOld extends PeriodItemFormatter
{
    protected function _urlListenPopup($res)
    {
        $url_listen_popup = "/grid/channel/period/item-listen-popup.html?periodItemID={$res["id_item"]}";
        \EkvCommonHelper::appendUrlWithLangPrefixRef($url_listen_popup);
        return $url_listen_popup;
    }
}
