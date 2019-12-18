<?php
namespace Ekv\Misc\OnAirCounter\Parse;

class PresenterOnAirCounterParseChannel
{
    private $DB;
    private $channelID;
    private $now;
    private $statDaysInterval = 14;

    function __construct($channelID)
    {
        $this->DB = \DB::getInstance();
        $this->channelID = @intval($channelID);
        $this->now = \EkvDateHelper::mysqlNow();
    }

    public function mainParseChannel()
    {
        $this->clearUsagePerChannel();

        $usageStats = $this->getUsageStats();
        $this->saveUsageStats($usageStats);
    }

    private function saveUsageStats($stats)
    {
        if (empty($stats)) {
            return false;
        }

        $sqlTpl = "
            INSERT IGNORE INTO `".\EkvGlobalHelper::TABLE_PRESENTER_ONAIR_USAGE."`
            (presenter_id, channel_id, onair_counter)
            VALUES
                %values%
        ";

        $values = array();
        foreach ($stats as $statVal) {
            $values[] = "('{$statVal["presenter_id"]}', '{$this->channelID}', '{$statVal["cnt"]}')";
        }


        $sql = strtr($sqlTpl, array('%values%' => implode(','.PHP_EOL, $values)));
        $this->DB->query($sql);
    }

    private function getUsageStats()
    {
        $sql = "
            SELECT COUNT(*) as cnt, p2pi.presenter_id
            FROM `".\EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                INNER JOIN `".\EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                    ON pi.period_id = p.id_period
                        AND p.channel_id = {$this->channelID}
                INNER JOIN `".\EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."` p2pi
                    ON p2pi.period_item_id = pi.id_item
            WHERE
                pi.item_enabled = 1
                AND pi.dt_full > DATE_SUB('{$this->now}', INTERVAL {$this->statDaysInterval} DAY)
                AND pi.dt_full <= '{$this->now}'
            GROUP BY
                p2pi.presenter_id
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    private function clearUsagePerChannel()
    {
        $sql = "
            DELETE FROM `".\EkvGlobalHelper::TABLE_PRESENTER_ONAIR_USAGE."`
            WHERE
                channel_id = '{$this->channelID}'
        ";
        $this->DB->query($sql);
    }

}
