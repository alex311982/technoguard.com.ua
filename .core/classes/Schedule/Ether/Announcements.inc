<?php
namespace Ekv\Schedule\Ether;
use Ekv\Schedule\Formatting\PeriodFormatter;
use Ekv\Schedule\Formatting\PeriodItemFormatter;
use Ekv\Schedule\PeriodItem;
use EkvGlobalHelper, EkvDateHelper;

/**
 * Announcements for main page.
 * First try retrieve next anns (todays, or next days) when not all channels have them,
 * then retrieve also previous (todays but with less time or past days) and then combine them for all channels
 * providing priority to next anns.
 * For getting results for all channels at once use unions, the most of code prepares sqls
 */
class Announcements
{
    private $DB;
    private $_date;
    private $_time;
    private $_annLimitPerChannel = 1;

    /**
     * Restriction to week has been applied to imporve query performance
     * @var int
     */
    private $_daysPeriodLookForAnn = 21;

    function __construct()
    {
        $this->DB = \DB::getInstance();
        $this->_date = EkvDateHelper::mysqlNow(false);
        $this->_time = EkvDateHelper::mysqlNowTime();
    }

    function mainChannelsAnns()
    {
        $channelObj = new \EkvChannel();
        $channels = $channelObj->getChannels();

    	if (ekvLangID() != 1) {

    		unset($channels[1], $channels[2], $channels[3]);
    		$this->_annLimitPerChannel = 4;
    	}
	
        $channel_announcements = $this->_getAllChannelsAnnouncements($channels);

        $channel_announcements = $this->_formatChannelAnns($channel_announcements);


        $final_res = $this->_annsToChannels($channels, $channel_announcements);

        return $final_res;
    }

    private function _formatChannelAnns($channel_announcements)
    {
        foreach($channel_announcements as $channel_id => &$period_items){
            if(!empty($period_items)){
                $periodItemObj = new PeriodItem();
                $piFormatterObj = new PeriodItemFormatter();
                $piFormatterObj->setImgSize('size2');

                $periodFormatterObj = new PeriodFormatter();
                foreach($period_items as &$pval){
                    $channel_id = $pval["channel_id"];
                    //$periodItemObj->prepareSingleItemPlusBankFields($pval);
                    $pval = $piFormatterObj->mainFormatSinglePeriodItemWithBankFields($pval);

                    //$pval["channel_url"] = \EkvChannel::getChannelScheduleUrl($channel_id, $pval["period_id"]);
                    $pval["channel_url"] = $periodFormatterObj->periodUrl($channel_id, $pval["per_date"]);
                    //$pval["period_item"] = $period_item_formatted;

                    //--- date-time format ---//
                    $date_symbolic = PeriodItemFormatter::formatDateAsMonthName($pval["per_date"]);
                    $time_formatted = PeriodItemFormatter::formatTimeHHMin($pval["item_time"]);
                    $pval["date_formatted"] = array(
                        'date' => $date_symbolic,
                        'time' => $time_formatted
                    );
                    //$channel_announcements[$channel_id][] = $pval;
                }
            }
        }
        
        return $channel_announcements;
    }

    private function _annsToChannels($channels, $anns_by_channel)
    {

        foreach($channels as &$ch_val){
            $channel_id = $ch_val["id_channel"];
            if (ekvLangCode() != 'uk') $u_ = '/'.ekvLangCode(); else $u_ = '';
            $ch_val["url_ann_all"] = $u_."/grid/ann.html?channelID={$channel_id}";
            $ch_val["anns"] = array();

            if(isset($anns_by_channel[$channel_id])){
                $ch_val["anns"] = $anns_by_channel[$channel_id];
            }
        }

        return $channels;
    }

    /**
     * Get finals anns containing of combined anns of future and past (if required)
     * @param $channels
     * @return array
     */
    private function _getAllChannelsAnnouncements($channels)
    {
        $anns_next_raw = $this->_getNextAnnounces($channels);
        $anns_next_groupped = $this->_groupAnnByChannel($anns_next_raw);
        //pa($anns_next_groupped);exit;
        
        /*
         * Check whether all channels have next anns
         * If not - fetch previous ones
         */
        $diff = array_diff(array_keys($channels),
            array_keys($anns_next_groupped)
        );

        $anns_final = $anns_next_groupped;

        if(!empty($diff)){
            $anns_prev = $this->_getPrevAnnounces($channels);
            $anns_prev_groupped = $this->_groupAnnByChannel($anns_prev);
            //pa($anns_next_groupped, $anns_prev_groupped);exit;

            //--- combine both next and prev sources, priority is for next only ---//
            if(!empty($anns_prev_groupped)){
                $anns_final = $this->_combinePrevAndNext($channels, $anns_next_groupped, $anns_prev_groupped);
            }
        }

       return $anns_final;
    }

    /**
     * Gets combined anns providing priority to the next ones
     * @param $channels
     * @param $next
     * @param $prev
     * @return array
     */
    private function _combinePrevAndNext($channels, $next, $prev)
    {
        $combined = array();
        foreach($channels as $channel_id => $ch_val){
            $items = array();
            if(isset($next[$channel_id])){
                $items = $next[$channel_id];
            }elseif(isset($prev[$channel_id])){
                $items = $prev[$channel_id];
            }
            $combined[$channel_id] = $items;
        }
        return $combined;
    }

    private function _groupAnnByChannel($anns_raw)
    {
        $groupped = array();
        foreach($anns_raw as $ann_val){
            $channel_id = $ann_val["channel_id"];
            $groupped[$channel_id][] = $ann_val;
        }
        return $groupped;
    }

    private function _getNextAnnounces($channels)
    {

        $sql_parts = array();
        foreach($channels as $ch_val){
            $sql_parts[] = "(".$this->_futureAnnForChannelSingle($ch_val["id_channel"]).")";
        }
        $anns = $this->_uniteAllChannelsPartialSqls($sql_parts);
        return $anns;

    }

    private function _getPrevAnnounces($channels)
    {

        $sql_parts = array();
        foreach($channels as $ch_val){
            $sql_parts[] = "(".$this->_pastAnnForChannelSingle($ch_val["id_channel"]).")";
        }
        $anns = $this->_uniteAllChannelsPartialSqls($sql_parts);
        return $anns;

    }

    /**
     * Final query for retrieving prev or next anns
     * @param $sql_parts
     * @return array
     */
    private function _uniteAllChannelsPartialSqls($sql_parts)
    {
        $sql_union = implode(PHP_EOL . " UNION " . PHP_EOL, $sql_parts);
        $sql_union .= "
            ORDER BY channel_id ASC
        ";
        //echo "\n <pre>$sql_union </pre> <br/>\n";
//var_dump($sql_union);exit;
        $anns = $this->DB->RunSql($sql_union, true);
        return $anns;
    }

    /**
     * Common template (used in union) to get prev or next anns by channel
     * @param $channel_id
     * @param $extra_cond
     * @param $sort_by
     * @return string
     */
    private function _getChannelAnnsActiveTemplate($channel_id, $extra_cond, $sort_by)
    {
        $sql = "
            SELECT p.channel_id,
                p.per_date,
                p.per_name,
                pi.*
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                    ON p.id_period = pi.period_id
            WHERE
                p.channel_id = '{$channel_id}'
                AND pi.show_on_main = 1

                {$extra_cond}
            ORDER BY
                {$sort_by}
            LIMIT {$this->_annLimitPerChannel}
        ";

        return $sql;
    }

    /**
     * Prepare sql for retrieving future anns for each channel
     * @param $channel_id
     * @return string
     */
    private function _futureAnnForChannelSingle($channel_id)
    {
        $date_next = $this->_dateNext('+');
        $exta_cond = "
            AND (
                (p.per_date = '{$this->_date}'
                    AND pi.item_time > '{$this->_time}')
                OR
                (p.per_date > '{$this->_date}'
                    AND p.per_date < '{$date_next}'
                )
            )
        ";
        $sort = "
            p.per_date ASC,
            pi.item_time ASC
        ";
        $sql_part = $this->_getChannelAnnsActiveTemplate($channel_id, $exta_cond, $sort);
        return $sql_part;
    }

    private function _pastAnnForChannelSingle($channel_id)
    {
        $date_prev = $this->_dateNext('-');
        $exta_cond = "
            AND (
                (p.per_date = '{$this->_date}'
                    AND pi.item_time < '{$this->_time}')
                OR
                (p.per_date < '{$this->_date}'
                    AND p.per_date > '{$date_prev}'
                )
            )
        ";
        $sort = "
            p.per_date DESC,
            pi.item_time DESC
        ";
        $sql_part = $this->_getChannelAnnsActiveTemplate($channel_id, $exta_cond, $sort);
        return $sql_part;
    }

    /**
     * @param string $sign
     * @return string - mysql date required
     */
    private function _dateNext($sign = '+')
    {
        /*
         * Somehow direct literal string  AND p.per_date > '2013-08-29' works much faster than
         * AND p.per_date > DATE_SUB(p.per_date, INTERVAL 7 DAY)
         */
        $dt = new \DateTime($this->_date);
        $dt->modify("{$sign}{$this->_daysPeriodLookForAnn} day");
        $date_next = $dt->format(EkvDateHelper::MYSQL_DATE_ONLY);
        return $date_next;
    }
}
