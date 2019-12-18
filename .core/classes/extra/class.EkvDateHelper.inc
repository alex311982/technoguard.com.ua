<?php
/**
 * User: sunsey
 * Date: 07.12.2010
 */
 
class EkvDateHelper
{
    const DEFAULT_BACKEND_DATE_FORMAT = 'd.m.Y H:i';
    const FORMAT_JQ_DATE_PICKER = 'd.m.Y H:i';
    const FORMAT_JQ_DATE_PICKER_NO_TIME = 'd.m.Y';
    const MYSQL_NULL_DATE = "0000-00-00 00:00:00";
    const PHP_DATE_ONLY = "j.m.Y";

    const MYSQL_DATE_ONLY = "Y-m-d";
    const MYSQL_TIME_ONLY = "H:i:s";
    const MYSQL_DATE_TIME = "Y-m-d H:i:s";

    static function convert2DefaultBackendDateFormat($src_date)
    {
        return date( self::DEFAULT_BACKEND_DATE_FORMAT,
                self::mysqlDate2PhpDate($src_date)
            );
    }
    
    static function checkIsMysqlDateZero($mysql_date)
    {
        if($mysql_date == self::MYSQL_NULL_DATE){
            return true;
        }else{
            return false;
        }
    }

    static function mysqlNow($use_time = true)
    {
        $now = self::phpDate2MysqlDate(time());
        if($use_time == false){
            $tmp = explode(" ", $now);
            $now = trim($tmp[0]);
        }
        return $now;
    }

    static function mysqlNowDateOnly()
    {
        return self::mysqlNow(false);
    }

    static function mysqlNowTime()
    {
        return date('H:i:s');
    }

	//--- Convert php timestamp to Mysql query format string. Will work with time also ---//
	static function phpDate2MysqlDate($phpdate){
		return date( 'Y-m-d H:i:s', $phpdate );		
	}

	//--- Convert 2009-06-15 13:16:4 to timestamp ---//
	static function  mysqlDate2PhpDate($mysqldate){
		$phpdate = strtotime( $mysqldate );
		return $phpdate;
	}

    static function getJqDatePickerFormatedDate($mysql_date, $show_time = true)
    {
        $date_format = self::FORMAT_JQ_DATE_PICKER;
        if($show_time == false){
            $date_format = self::FORMAT_JQ_DATE_PICKER_NO_TIME;
        }
        $return_date = "";
        $php_date = self::mysqlDate2PhpDate($mysql_date);
        if($php_date > 0){
            $return_date = date( $date_format, $php_date );
        }
        return $return_date;
    }

    /**
     * @static
     * @param $jq_picker_date
     * @param bool $has_time
     * @return bool|string
     */
    static function convertJqDatePickerDate2MysqlDate($jq_picker_date, $has_time = true)
    {
        //--- convert datepicker date to proper mysql formated date ---//
        $tstmp = strtotime($jq_picker_date);
        $mysql_date = "";
        if($tstmp !== FALSE){
            $mysql_date = self::phpDate2MysqlDate($tstmp);
            if($has_time == false){
                $mysql_date = self::getDateFromMysqlDatetime($mysql_date);
            }
        }
        return $mysql_date;
    }

    static function getDateFromMysqlDatetime($mysql_datetime)
    {
        $res = explode(" ", $mysql_datetime);
        if(count($res) == 2){
            return $res[0];
        }
        return false;
    }

    static function convertJqDatePickerDate2Timestamp($jq_picker_date)
    {
        $tstmp = strtotime($jq_picker_date);
        return $tstmp;
    }

    //-------------------------
    // 01 January 11 14:08:37 -> 01 січня 11 14:08:37  
    //-------------------------
    static function getDateWithTranslatedMonth($php_date_value, $date_format = "")
    {

        $aEngMonth = array('January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
        $aUkrMonth = array('сiчня', 'лютого', 'березня', 'квітня', 'травня', 'червня', 'липня', 'серпня', 'вересня', 'жовтня', 'листопада', 'грудня');
        //$aUkrMonth = array('января', 'февраля', 'марта', 'апреля', 'мая', 'июня', 'июля', 'августа', 'сентября', 'октября', 'ноября', 'декабря');

        if(empty($date_format)){
            $date_format = FRONTEND_DATE_FORMAT_PHP;
        }

        $replaceMonth = $aUkrMonth;
        $sDate = date($date_format, $php_date_value);
        $sDate = str_ireplace($aEngMonth, $replaceMonth, $sDate);
       
        return $sDate;
    }

    static function getDateWithTranslatedMonth_DateOnly($date_value)
    {
        return self::getDateWithTranslatedMonth($date_value, self::PHP_DATE_ONLY) ;
    }

    static function getWeekDayName($php_timestamp)
    {
        $wday_name = null;
        $wdays_list = array("воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота");
        $date_parsed = getdate($php_timestamp);
        //pa($date_parsed);
        if(isset($date_parsed["wday"])){
            $wday_name = @$wdays_list[$date_parsed["wday"]];
        }
        return $wday_name;
    }

    static function formatMysqlDateBound($year, $month, $day, $use_min_time = true)
    {
        $hour = 0;
        $min = 0;
        $sec = 0;
        if($use_min_time == false){
            $hour = 23;
            $min = 59;
            $sec = 59;
        }
        $tstmp = mktime($hour, $min, $sec,  $month, $day, $year);
        $mysql_date = self::phpDate2MysqlDate($tstmp);
        return $mysql_date;
    }

    /**
     * is used when date filtering with no time
     * so set time as last available for day
     * @static
     * @param $mysql_date
     * @return string - datetime of last second of the dat
     */
    static function appendDateWithTimeBoundTo($mysql_date)
    {
        $res = $mysql_date . " 23:59:59";
        return $res;
    }

    static function appendDateWithTimeBoundFrom($mysql_date)
    {
        $res = $mysql_date . " 00:00:00";
        return $res;
    }

    static function getTimeOnly($mysql_date)
    {
        $date_format = "H:i";
        $formated_date = "";
        if(!empty($mysql_date)){
            $formated_date = date($date_format, mysqldate2phpdate($mysql_date));
        }
        return $formated_date;
    }

    static function formatDate($mysql_date, $date_only = false)
    {
        $date_format = "j.d Y H:i";
        if($date_only == true){
            $date_format = "j.m.Y";
        }
        $formated_date = "";
        if(!empty($mysql_date)){
            //$formated_date = date("m.F.y H:i:s", mysqldate2phpdate($raw_date));
            
            $formated_date = EkvDateHelper::getDateWithTranslatedMonth(
                mysqldate2phpdate($mysql_date),
                $date_format
            );
            
        }
        return $formated_date;
    }

    //#------------------- jqeury datepicker -------------------#//
    static function fixDatePickerForEditAction(&$result, $field_manual, $field_db, $use_time = true)
    {
        $result["record"][$field_manual]["value"] =
                EkvDateHelper::getJqDatePickerFormatedDate($result["record"][$field_db]["value"], $use_time);
    }

    static function fixDatePickerForSaveAction(&$VARS, $field_manual, $field_db, $use_time = true)
    {
        if(
            isset($VARS[$field_manual])
            && !empty($VARS[$field_manual])
        ){
            $VARS[$field_db] = EkvDateHelper::convertJqDatePickerDate2MysqlDate($VARS[$field_manual]);
        }
    }

    //--- backend datepicker methods  ---//

    static function fixManualDateFieldOnEdit(&$result, $field_date_key, $field_date_manual_key, $show_time = true)
    {
        if(isset($result["record"])){
            $result["record"]["$field_date_manual_key"]["value"] =
                    EkvDateHelper::getJqDatePickerFormatedDate($result["record"]["$field_date_key"]["value"], $show_time);
        }
    }

    static function parseMysqlDateFromMysqlDatetime($mysql_datetime)
    {
        $tmp = explode(' ', $mysql_datetime);
        return $tmp[0];
    }
}
