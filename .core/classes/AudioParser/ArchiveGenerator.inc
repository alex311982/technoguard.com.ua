<?php


namespace Ekv\AudioParser;


use DateTime;
use DB;
use DirectoryIterator;
use EkvDateHelper;
use EkvFileLogger;
use EkvGlobalHelper;

class ArchiveGenerator
{
    private $DB;

    /**
     * @var EkvFileLogger
     */
    private $_fLoggerObj;

    /**
     * @var int php ts of current time
     * as relative point for calculations
     */
    private $_parseTimestamp;

    function __construct()
    {
        $this->DB = DB::getInstance();
        $this->_fLoggerObj = new EkvFileLogger("archive_generator_60");
        $this->_parseTimestamp = time();
    }

    private function _log($msg)
    {
        $this->_fLoggerObj->writeStringPlainDense($msg);
    }

    function mainAllChannels()
    {
        $this->_log(PHP_EOL . "[START] define periods");
        [$date1, $date2] = $this->_getCalculationDate();

        $progs = $this->_getChannelsWithDayPeriods($date1, $date2);
        $result = [];

        if(!empty($progs)) {
            foreach($progs as $prog) {
                $explodeResultArray = explode("/", trim($prog['audio_file_auto'], '/'));
                $result[$explodeResultArray[1]][$explodeResultArray[2]][] = $explodeResultArray[3];
            }
        }

        foreach ($result as $channel => $item) {
            if (!empty($channel)) {
                foreach ($item as $date => $itemsByDate) {
                    foreach ($itemsByDate as &$itemByDate) {
                        $itemByDate = PATH_ROOT . '/audio_slice/' . $channel . '/' . $date . '/' . $itemByDate;
                    }

                    $string = implode(PHP_EOL, $itemsByDate);
                    file_put_contents(PATH_ROOT . '/audio_slice/' . $channel . '/' . $date . '/progsnotdelete.txt', $string);
                }
            }
        }
    }

//    function rrmdir($path) {
//        $i = new DirectoryIterator($path);
//        foreach($i as $f) {
//            if ($f->isFile() && $f->getFilename() == 'progsnotdelete.txt') {
//                //unlink($f->getRealPath());
//                echo $f->getRealPath();
//            }
//        }
//    }

    /**
     * reparse for specific day
     * @param $human_readable
     */
    function setParseTimeHuman($human_readable)
    {
        //strtotime("2013-01-09 02:40:03");
        $this->_parseTimestamp = strtotime($human_readable);;
    }

    private function _getCalculationDate()
    {
        $dtObj1 = new DateTime();
        $dtObj1->setTimestamp($this->_parseTimestamp); // for test
        $dtObj1->modify("-8 day");

        $dtObj2 = new DateTime();
        $dtObj2->setTimestamp($this->_parseTimestamp); // for test
        $dtObj2->modify("-1 day");

        $date1 = $dtObj1->format(EkvDateHelper::MYSQL_DATE_ONLY);
        $date2 = $dtObj2->format(EkvDateHelper::MYSQL_DATE_ONLY);

        return [$date1, $date2];
    }

    private function _getChannelsWithDayPeriods($date1, $date2)
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                DATE(dt_full) >= '{$date1}'
                AND DATE(dt_full) <= '{$date2}'
                AND is_archivable = 0;
        ";

        $res = $this->DB->RunSql($sql, true);

        return $res;
    }
}