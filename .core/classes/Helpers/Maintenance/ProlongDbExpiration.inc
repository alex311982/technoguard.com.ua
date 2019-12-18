<?php
namespace Ekv\Helpers\Maintenance;

use EkvGlobalHelper, DB;

class ProlongDbExpiration
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    function mainProlongExpiration($days_amount = 30)
    {
        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            SET
                `per_date` = DATE_ADD(`per_date`, INTERVAL {$days_amount} DAY),
                `per_name` = DATE_FORMAT(`per_date`,'%d.%m.%Y')
            WHERE
                per_enabled = 1
        ";
        $this->DB->query($sql);

        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` opi
            INNER JOIN (
                SELECT pi.id_item, pi.item_time, p.per_date
                FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                    INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                        ON p.id_period = pi.period_id
                ORDER BY
                    id_item ASC
                -- LIMIT 100
            ) tmp
                ON tmp.id_item = opi.id_item
            SET
                opi.dt_full = CONCAT(tmp.per_date, ' ', tmp.item_time)
        ";
        $this->DB->query($sql);
    }
}
