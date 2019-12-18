<?php
namespace Ekv\Schedule\Formatting;

use EkvGlobalHelper, EkvDateHelper, EkvCommonHelper;

class PeriodFormatter
{

    /**
     * @param $date_mysql
     * @return mixed - '23 серпня'
     */
    function periodHumanDate($date_mysql)
    {
        $res = EkvDateHelper::getDateWithTranslatedMonth_DateOnly(
            EkvDateHelper::mysqlDate2PhpDate($date_mysql)
        );

        return $res;
    }

    function periodUrl($channel_id, $date_mysql = null)
    {

        $url = "/schedule/period-items.html?channelID={$channel_id}";
        if(!empty($date_mysql)){
            $url .= "&date={$date_mysql}";
        }
        EkvCommonHelper::appendUrlWithLangPrefixRef($url);
        return $url;
    }
}
