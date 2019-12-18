<?php
namespace Ekv\Misc;

class SkinRoot
{
    const TYPE_ORIGINAL = 2;
    const TYPE_MOURNING = 3;
    const TYPE_HOLIDAY = 4;

    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    public function calculateFrontCurrentSkinType()
    {
        $now = \EkvDateHelper::mysqlNow();

        $sql = "
            SELECT *
            FROM `".\EkvGlobalHelper::TABLE_SKIN_SCHEDULE."`
            WHERE
                enabled = 1
                AND dt_begin <= '{$now}'
                AND dt_end >= '{$now}'
            ORDER BY
                id_row DESC
            LIMIT 1
        ";
        $res = $this->DB->RunSql($sql);

        $skinType = '';
        if (!empty($res)) {
            switch ($res["skin_type"]) {
                case self::TYPE_MOURNING:
                    $skinType = 'mourning';
                    break;
                case self::TYPE_HOLIDAY:
                    $skinType = 'holiday';
                    break;
            }
        }

        return $skinType;
    }
}
