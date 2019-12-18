<?php
namespace Ekv\AudioParser;
use EkvGlobalHelper, DB, EkvCommonHelper, EkvFileLogger;
use \Ekv\Schedule\ScheduleRoot;

class CmdComposer
{
    private $DB;
    /**
     * @var EkvFileLogger
     */
    private $_loggerObj;

    private $_periodID;
    private $_periodInfo;

    private $_srcPath;
    private $_destinationPath;
    private $_progCmdPath;
    //private $_datePrefix = "2013-01-09";

    private $_hoursToFiles;

    private $_periodItems;

    //private $_cmdBuffer;

    function __construct($period_id)
    {
        $this->DB = DB::getInstance();
        $this->_periodID = $period_id;

        $this->_initPeriodInfo();
        $this->_initVars();

        $this->_loggerObj = new EkvFileLogger("ffmpeg_sys");
    }

    private function _initPeriodInfo()
    {
        $schedRootObj = new ScheduleRoot();
        $this->_periodInfo = $schedRootObj->getPeriodInfoExtended($this->_periodID);
        ParseException::ensure(!empty($this->_periodInfo), "Incrorrect grid period data");
    }

    private function _initVars()
    {
        $conf_file = PATH_ROOT ."/.conf/audio_parse.inc";
        ParseException::ensure(file_exists($conf_file), "Can't find '.conf/audio_parse.inc' config");
        $conf_contents = parse_ini_file($conf_file);

//        $this->_progCmdPath = "/usr/local/bin/ffmpeg1";
//        $this->_srcPath = PATH_ROOT . "/audio/UR2/20130109/";
//        $this->_destinationPath = PATH_ROOT . "/audio_slice/UR2/20130109/generation_13589449899956/";
//        $this->_srcPath = $conf_contents["AUDIO_SRC_PATH"];
//        $this->_destinationPath = $conf_contents["AUDIO_DEST_PATH"];
        $cmdPathHelperObj = new CmdPathHelper($this->_periodInfo);

        $this->_progCmdPath = $conf_contents["AUDIO_PROG_CMD_PATH"];
        $this->_srcPath = $cmdPathHelperObj->composeSrcBasePath();
        $this->_destinationPath = $cmdPathHelperObj->composeDestPath();

        //pa($this->_progCmdPath, $this->_srcPath, $this->_destinationPath);
    }

    function main()
    {
        $this->mainPreparePeriodItemsWithCommands();

        /**
         * close db to avoid "server gone away" trouble because of timeout
         * cuase sys command running may be long
         */
        $this->DB->close_db();

        $this->mainRunCommands();

        $this->updateDbWithGeneratedFiles();
    }

    function mainPreparePeriodItemsWithCommands()
    {
        $this->_hoursToFiles = $this->_mapFilesToHours();
        $this->_periodItems = $this->_fetchDbPeriodItems();
        $this->_appendCommands();

        return $this->_periodItems;
    }

    function mainRunCommands()
    {
        $loggerObj = $this->_loggerObj;
        $loggerObj->writeStringPlainDense("----->>> sys began");

        $i = 0;
        foreach($this->_periodItems as $per_val){
            $i++;

            //--- for test ---//
            if($i == 5){
                //break; //!!!!
            }

            $cmd_str = $per_val["cmd_info"]["ffmpeg_cmd"];
            if(!empty($cmd_str)){
                $output_ar = array();
                $cmd_res = null;

                //#------------------- SYS CALL -------------------#//
                exec($cmd_str, $output_ar, $cmd_res);
                //pa($output_ar, $cmd_res);
            }else{
                //--- just for test ---//
                $cmd_str = "[EMPTY] period_item_id: {$per_val["id_item"]} ";
            }
            $loggerObj->writeStringPlainDense($cmd_str);
        }
        $loggerObj->writeStringPlainDense('sys done <<<-----');
    }

    function updateDbWithGeneratedFiles()
    {
        $this->DB->connect();
        $this->_loggerObj->writeStringPlainDense("DB connection restored");

        //--- the only way to update multiple values in single query ---//
        $sql_tpl = "
            INSERT INTO `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
                (`id_item`, `audio_file_auto`)
            VALUES
                %1\$s
            ON DUPLICATE KEY UPDATE
                `audio_file_auto` = VALUES(`audio_file_auto`)
        ";

        $values = array();
        foreach($this->_periodItems as $per_val){
            $fname = '';
            //--- if no src mp3 file provided set empty path ---//
            if(!empty($per_val["cmd_info"]["ffmpeg_cmd"])){
                $fname = mysqli_escape_string($this->DB->connection_id, $per_val["cmd_info"]["output_path_relative"]);
            }

            $values[] = "({$per_val["id_item"]}, '{$fname}')";
        }

        $values_str = EkvCommonHelper::implodeWithComma($values);

        $sql = sprintf($sql_tpl, $values_str);
        $this->DB->query($sql);

        $this->_loggerObj->writeStringPlainDense("Db filenames updated");

        $this->_updatePeriodFileGenerationCurrent();
    }

    private function _updatePeriodFileGenerationCurrent()
    {
        $gen = "generation_13589449899956"; //TODO generation real
        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            SET
                `per_file_generation_current` = '{$gen}'
            WHERE
                `id_period` = '{$this->_periodID}'
        ";
        $this->DB->query($sql);
        $this->_loggerObj->writeStringPlainDense("period generation updated");
    }

    private function _appendCommands()
    {
        //pa($period_items);exit;

        foreach($this->_periodItems as &$per_val){
            $output_path_relative = $this->_destinationPath . $per_val["id_item"] . ".mp3";
            $cmd_info = array(
                'time_begin' => $this->_convertBeginTimeToCmdTime($per_val["time_begin"]),
                'duration_sec' => $per_val["duration"],
                'output_path_relative' => $output_path_relative,
                'output_path' => EkvCommonHelper::getDocRootPath($output_path_relative),
            );

            $cmd_info["ffmpeg_cmd"] = $this->_createFfmpegCmd($per_val, $cmd_info);
            $per_val["cmd_info"] = $cmd_info;
        }

        //pa($this->_periodItems);exit;
    }

    private function _createFfmpegCmd($period_item, $cmd_info)
    {
        //$cmd = "/usr/local/bin/ffmpeg1 -i 10.mp3 -ss 00:50:00.000 -t 00:10:00.000 -c:a copy -y out1.mp3";
        $input_file = $this->_hoursToFiles[$period_item["hour_begin"]]["audio_src_path"];

        $cmd_full = null;
        //--- no sense to create and run command when there is no appropiate hourly src file ---//
        if(!empty($input_file)){
            $cmd_full = "{$this->_progCmdPath} -i {$input_file} ".
                " -ss {$cmd_info["time_begin"]} -t {$cmd_info["duration_sec"]} ".
                " -c:a copy -y {$cmd_info["output_path"]}"
            ;
        }
        return $cmd_full;
    }

    private function _convertBeginTimeToCmdTime($begin_time)
    {
        $parts = explode(':', $begin_time);
        ParseException::ensure(sizeof($parts) == 3, "Incorrect time");
        $parts[0] = '00';

        $cmd_time = implode(':', $parts);
        return $cmd_time;
    }

    private function _fetchDbPeriodItems()
    {
        $sql = "
            SELECT id_item,
                item_time as `time_begin`,
                item_time_end as `time_end`,
                HOUR(`item_time`) as `hour_begin`,
                HOUR(`item_time_end`) as `hour_end`,
                duration
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                period_id = '{$this->_periodID}'
                AND item_enabled = 1
                AND playback_type != '".EkvGlobalHelper::PLAYBACK_TYPE_IN_EDITION."'
            ORDER BY
                item_time ASC
        ";
        $res = $this->DB->RunSql($sql, true);

        return $res;
    }

    private function _checkHasArrayHoursDifference($period_items)
    {
        $different_hours_res = null;
        foreach($period_items as $rval){
            if($rval["hour_begin"] != $rval["hour_end"] ){
                $different_hours_res[] = $rval;
            }
        }
        return $different_hours_res;
    }

    /**
     * create map corresponding hour to exisitng sourcefilename
     * @return array
     */
    private function _mapFilesToHours()
    {
        //--- find all files with path in specific source folder ---//
        $path_src_find = EkvCommonHelper::getDocRootPath($this->_srcPath) . "*.mp3";
        $files_found = glob($path_src_find);
        //pa($files_found);

        //--- init formated hours array ---//
        $hours_formated = $this->_initHoursFormated();

        //--- period date perfix in src filename ---//
        $date_prefix = str_replace('-', '\-', $this->_periodInfo["per_date"]);
        foreach($files_found as $file_val){
            $matches = array();
            //--- get current hour from filename ---//
            preg_match('#'.$date_prefix.'_([0-9]{2})#', $file_val, $matches);

            //--- link file with defined hour ---//
            if(isset($matches["1"])){
                $current_hour = sprintf("%d", $matches["1"]); // remove leading zero
                $hours_formated[$current_hour]["audio_src_path"] = $file_val;
            }
        }

        //pa($hours_formated);
        return $hours_formated;
    }

    private function _initHoursFormated()
    {
        $hours_raw = range(0, 23);
        $hours_formated = array();
        foreach($hours_raw as $hval){
            $hours_formated[$hval] = array(
                'hour_int' => $hval,
                'hours_leading_zero' => sprintf("%02d", $hval),
                'audio_src_path' => ''
            );
        }
        //pa($hours_formated);
        return $hours_formated;
    }
}
