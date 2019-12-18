<?php
namespace Ekv\Helpers\Maintenance;
use Ekv\Schedule\Ether\Ether;
use EkvGlobalHelper;

class UsefullQueries
{
    static function linkRandomPresenters()
    {
        //--- link to passed progs ---//
        $sql = "
            INSERT INTO `".EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."`
            (presenter_id, period_item_id)
            SELECT 9, pi.id_item
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                    ON p.id_period = pi.period_id
            WHERE
                p.per_date > DATE_SUB(NOW(), INTERVAL 14 DAY)
                AND p.per_date <= NOW()
                ".Ether::conditionInEditionSt()."
            ORDER BY
                RAND()
            LIMIT 10
        ";

        //--- link to next progs ---//
        $sql = "
            INSERT INTO `".EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."`
            (presenter_id, period_item_id)

            SELECT 9, pi.id_item
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                    ON p.id_period = pi.period_id
            WHERE
                p.per_date <= DATE_ADD(NOW(), INTERVAL 5 DAY)
                AND p.per_date >= NOW()
                ".Ether::conditionInEditionSt()."
            ORDER BY
                RAND()
            LIMIT 10
        ";
    }

    static function linkBankProgToPiItem()
    {
        //--- passed ---//
        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` opi
                INNER JOIN
                (
                    SELECT pi.id_item
                    FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                        INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                            ON p.id_period = pi.period_id
                    WHERE
                        p.per_date > DATE_SUB(NOW(), INTERVAL 14 DAY)
                        AND p.per_date <= NOW()
                        ".Ether::conditionInEditionSt()."
                    ORDER BY
                        RAND()
                    LIMIT 10
                ) tmp
                    ON tmp.id_item = opi.id_item

            SET
                opi.prog_id = 79
        ";

        //--- next ---//
        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` opi
                INNER JOIN
                (
                    SELECT pi.id_item
                    FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                        INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                            ON p.id_period = pi.period_id
                    WHERE
                        p.per_date <= DATE_ADD(NOW(), INTERVAL 5 DAY)
                        AND p.per_date >= NOW()
                        ".Ether::conditionInEditionSt()."
                    ORDER BY
                        RAND()
                    LIMIT 10
                ) tmp
                    ON tmp.id_item = opi.id_item

            SET
                opi.prog_id = 79
        ";

    }

    /**
     * Update dt_full field for all period items
     */
    static function updateDtPeriodItemField()
    {
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
        echo "\n <pre>$sql </pre> <br/>\n";
    }
}
 