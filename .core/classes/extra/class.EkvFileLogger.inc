<?php
/**
 * User: sunsey
 * Date: 17 ���� 2011
 */

class EkvFileLogger
{
    private $_fname;
    private $_logDir;
    private $_userNameAppended;

    function __construct($fname = "common", $log_dir = null)
    {
        $this->_fname = $this->_createFilenameFull($fname);
        $this->_logDir = $log_dir;
    }

    private function _createFilenameFull($fname)
    {
        $res = $fname.".log";
        return $res;
    }

	/**
	 * PA Write to file
     * endless number of arguments output by print_r function
	 */
	function paw()
	{
		$args = func_get_args();
//		$backtrace = debug_backtrace();
//		$matches = array();
//		preg_match('|.*[\/\\\](.+)$|', $backtrace[0]['file'], $matches);
//		$res = array(date('Y-m-d H:i:s'), $matches[1].': '.$backtrace[0]['line'], $args);
        $s = "";

        $res = $this->_getCallerData();
        array_push($res, $args);



        $this->_appendDelim($s);
		//$s="\n".str_repeat("-",80)."\n";

		ob_start();
		print_r($res);
		$s.=ob_get_contents();
		ob_end_clean();

        //$s.="\n".str_repeat("-",80)."\n";
        $this->_appendDelim($s);

        $this->_writeFile($s);
	}

    private function _getCallerData()
    {
		$backtrace = debug_backtrace();
		$matches = array();
		preg_match('|.*[\/\\\](.+)$|', $backtrace[1]['file'], $matches);
		$res = array(date('Y-m-d H:i:s'), $matches[1].': '.$backtrace[1]['line']);
        return $res;
    }
    private function _appendDelim(&$src_str)
    {
        $src_str .= "\n".str_repeat("-",80)."\n";
        //return $src_str;
    }
    private function _writeFile($str_prepared)
    {
		$f=@ fopen($this->_getFilePathFull(), "a");
		if(!empty($f))
		{
			fwrite($f, $str_prepared);
			fclose($f);
		}
    }

    private function _getFilePathFull()
    {
        $base_log_path = PATH_TEMP . DIR_SEP . 'logs' . DIR_SEP;
        $dir_path = $base_log_path;
        if(!is_null($this->_logDir)){
            $dir_path = $base_log_path . $this->_logDir . DIR_SEP;
            if(file_exists($dir_path) == false){
                mkdir($dir_path);
            }
        }

        $final_path = $dir_path . $this->_fname;
        return $final_path;
    }

    function writeString($str)
    {
        $caller_data = $this->_getCallerData();

        $str_final = "";
        $this->_appendDelim($str_final);

        $caller_str = implode('   |   ', $caller_data) ."\n";
        $str_final .= $caller_str;
        $str_final .= $str;

        $this->_appendDelim($str_final);
        $this->_writeFile($str_final);
    }

    function writeStringPlainDense($str)
    {
        return $this->writeStringPlain($str, 1);
    }

    function writeStringPlain($str, $eol_num = 2)
    {
        $date = EkvDateHelper::mysqlNow();
        $str = $str ."\t[{$date}]";
        if(!is_null($this->_userNameAppended)){
            $str .= "\t[{$this->_userNameAppended}]";
        }

        $str .= str_repeat(PHP_EOL, $eol_num);

        $this->_writeFile($str);
    }

    function writeDelimiter($str_data = "")
    {
        $final_str = PHP_EOL . "--------------------------%str_data%---------------------------------";
        $replace = "";
        if(!empty($str_data)){
            $replace = "|{$str_data}|";
        }

        $final_str = str_replace('%str_data%', $replace, $final_str);
        $this->writeStringPlain($final_str, 1);
    }

    function writeStringsArray($str_ar)
    {
        if(is_array($str_ar) && !empty($str_ar)){
            $date = EkvDateHelper::mysqlNow();
            foreach($str_ar as &$val){
                $val = $val ."\t[{$date}]";
            }
            $str_final = implode(PHP_EOL, $str_ar);
            $str_final .= PHP_EOL;
            $this->_writeFile($str_final);
            return true;
        }
        return false;
    }

    function writePackedArray($array, $do_sanitize = true)
    {
        $sanitize_keys = array("view", "page_id", "module", "entity", "action");
        if($do_sanitize){
            foreach($sanitize_keys as $san_key){
                if(isset($array[$san_key])){
                    unset($array[$san_key]);
                }
            }
        }

        $packed_str = EkvCommonHelper::packArray2Text($array);
        $this->writeStringPlain($packed_str);
        return $packed_str;
    }

    function writeDebugPoint($point_name, $extra_str = "")
    {
        $str = "[micro: ".EkvCommonHelper::microtimePeriod()."] #{$point_name}#  {$extra_str}";
        $this->writeStringPlain($str);
    }

    public function setFilename($fname)
    {
        $this->_fname = $this->_createFilenameFull($fname);
    }

    public function setUserNameAppended($userNameAppended)
    {
        $this->_userNameAppended = $userNameAppended;
    }

    //#------------------- factories -------------------#//
    static function getInstanceApiSubmit()
    {
        $fLoggerObj = new EkvFileLogger("submitBooking", "api");
        return $fLoggerObj;
    }

    static function getInstanceApiNative($file_name)
    {
        return new EkvFileLogger($file_name, "api");
    }

    static function getInstanceKarabas($file_name)
    {
        $fLoggerObj = new EkvFileLogger($file_name, "karabas");
        return $fLoggerObj;
    }

    static function getInstanceBasketClear()
    {
        $fLoggerObj = new EkvFileLogger("basket_clear");
        return $fLoggerObj;
    }

}
