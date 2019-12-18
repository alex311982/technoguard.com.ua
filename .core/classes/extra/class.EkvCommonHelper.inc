<?php
use Ekv\Helpers\System\Lang;

/**
 * User: sunsey
 * Date: 18.03.2011
 */
 
class EkvCommonHelper
{
    static private $DB, $_site_core_view_fake;
    //static private $_menu_rubrics;
    const SITE_BACKEND = "admin",
        SITE_FRONTEND = "frontend";

    static function smartyEscapeEmulator($string, $charset = "utf-8")
    {
        return htmlspecialchars($string, ENT_QUOTES, $charset);
    }

    static function getSiteHostFromConf($use_http = true, $use_www = false)
    {
        $host  = SITE_HOST;
        if($use_http){
            if($use_www){
                $host = "www.{$host}";
            }
            $host = "http://{$host}";
        }
        return $host;
    }
    static function appendUrlWithSiteHostConf($url)
    {
        $full = self::getSiteHostFromConf($use_http = true);
        $full .= $url;
        return $full;
    }
    static function appendUrlWithSiteHostServer($url)
    {
        $full = self::getSiteHostFromServer($use_http = true);
        $full .= $url;
        return $full;
    }

    static function appendUrlWithLangPrefix($url_without_lang)
    {
        $lang_code = Lang::getInstance()->getLangCode(true);

        //--- if nondefault langcode append it ---//
        $url_final = $url_without_lang;
        if(!empty($lang_code)){
            $url_final = "/{$lang_code}{$url_final}";
        }
        return $url_final;
    }

    static function appendUrlWithLangPrefixRef(&$url_without_lang)
    {
        $url_without_lang = self::appendUrlWithLangPrefix($url_without_lang);
    }

    static function getSiteHostFromServer($use_http = true)
    {
        $host  = $_SERVER["HTTP_HOST"];
        if($use_http){
            $host = "http://{$host}";
        }
        return $host;
    }

    /**
     * Used for local mobile device testing, where domain equals ip and cookies settings problem has previously occured
     * @return string
     */
    static function getCookieDomain()
    {
        $domainFromConf = SITE_HOST;
        $domainServer = $_SERVER["HTTP_HOST"];

        $cookieDomain = COOKIE_DOMAIN;

        /*
         * If settings domain and real domain mismatch then check whether it's just a subdomain of settings domain
         * or completly different domain (maybe ip domain)
         * so if 'sub.sched.dev' then still get cookie domain from settings
         * if '192.168.*.*' - use it as cookie domain
         */
        if($domainServer != $domainFromConf){
            $pattern = '#[^\.]*\.*'.$domainFromConf.'$#';
            $matchRes = preg_match($pattern, $domainServer);
            //--- is not subdomain ---//
            if(!$matchRes){
                //$cookieDomain = ".{$domainServer}"; //for example .192.168.1.4 - didn't worked in chromium browsers
                $cookieDomain = NULL; // http://stackoverflow.com/a/1188145
            }
        }

        return $cookieDomain;
    }

    //#------------------- use tpl theme -------------------#//
    static function setUseTplTheme($val)
    {
        $set_val = @(bool)$val;
        $_SESSION["ekv_tpl_use_scheme"] = $set_val;
        //self::$_useTplTheme = $int_val;
    }

    static function getUseTplTheme()
    {
		//return true;
        $use_theme = false;
        if(isset($_SESSION["ekv_tpl_use_scheme"])){
            $use_theme = (bool)$_SESSION["ekv_tpl_use_scheme"];
        }
        //pa($use_theme);
        return $use_theme;
    }

    
    static function generateAutoStrForEmailTemplate($fields, $vars)
    {
        $auto_str = "";
        foreach($fields as $fk=>$fval){
            if(isset($fval["send_email"])){
                @ $auto_str .= "{$fval["title"]}:\t" . $vars[$fk] . "\n";
            }
        }
        return $auto_str;
    }

    static function autoStrFromPreparedArray($prepared_ar)
    {
        $auto_str = "";
        foreach($prepared_ar as $pval){
            @ $auto_str .= "{$pval[0]}:\t" . $pval[1] . "\n";
        }
        return $auto_str;        
    }

    /**
     * @static
     * @return DB
     */
    public static function getDB()
    {
        return self::$DB;
    }

    public static function setDB($DB)
    {
        self::$DB = $DB;
    }

    static function buildHtmlKeywords($events_limit = 5)
    {
        $eventObj = new EkvRoomEvent();
        $events = $eventObj->getLastEventsSuperBase($events_limit);
        $keywords_str = HTML_KEYWORDS;
        $delim = " ";
        if(!empty($events)){
            foreach($events as $eval){
                $keywords_str .= $delim . $eval["title"]; 
                $delim = ", ";
            }
        }

        return $keywords_str;
    }

//    public static function getMenuRubrics()
//    {
//        return self::$_menu_rubrics;
//    }
//
//    public static function setMenuRubrics($menu_rubrics)
//    {
//        self::$_menu_rubrics = $menu_rubrics;
//    }

    static function logConvertDataToCorrectEncoding($val)
    {
        //$val = iconv("windows-1251", "utf-8//IGNORE", $val);
        return $val;
    }

    static function packArray2Text($raw_array)
    {
        $b64_conv = base64_encode(self::logConvertDataToCorrectEncoding(serialize($raw_array)));
        return $b64_conv;
    }
    static function unpackArray2Text($str_packed)
    {
        $array_unpacked = array();
        $tmp_unpacked = base64_decode($str_packed);
        if($tmp_unpacked !== FALSE){
            $tmp_unpacked = unserialize($tmp_unpacked);
            if($tmp_unpacked !== FALSE){
                $array_unpacked = $tmp_unpacked;
            }

        }
        return $array_unpacked;
    }

    static function getSuperUniqueSuffix()
    {
        $str = uniqid(time()."_");
        return $str;
    }

    static function fileGetContents($url)
    {
        $curl_handler = curl_init($url);
        curl_setopt($curl_handler, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($curl_handler, CURLOPT_FOLLOWLOCATION, 1); // required for vticket
        $response = curl_exec($curl_handler);
        curl_close($curl_handler);
        return $response;
    }

    static function appendUrlWithParam($url, $param_name, $param_val)
    {
        $delim = "?";
        if(preg_match('#\?#', $url)){
            $delim = "&";
        }
        $final_url = "{$url}{$delim}{$param_name}={$param_val}";
        return $final_url;
    }

    static function implodeWithComma($array)
    {
        return implode(",", $array);
    }

    static function simmulateBetweenStatementForInt($from, $to, $return_csv = false)
    {
        $from = @intval($from);
        $to = @intval($to);        
        $ar = array();
        $csv_str = "";
        if($to > $from && $from >= 0){
            for($i = $from; $i <= $to; $i++){
                $ar[$i] = $i;
            }
            $csv_str = self::implodeWithComma($ar);
        }
        $return = ($return_csv == true)? $csv_str : $ar;
        return $return;
    }

    static function simmulateDateBounds($date_pure_mysql)
    {
        $date_from = "{$date_pure_mysql} 00:00:00";
        $date_to = "{$date_pure_mysql} 23:59:59";
        return
            array(
                'from' => $date_from,
                'to' => $date_to
            );
    }

    static function getEscapedStringForJs($raw_str)
    {
        return htmlspecialchars($raw_str,  ENT_QUOTES);
    }
    /**
     * sometimes it's required to process frontend tpls from backend environment so we need to make fake core
     * to make proper path for including
     */
    public static function getSiteCoreViewFake()
    {
        return self::$_site_core_view_fake;
    }
    public static function setSiteCoreViewFake($site_core_view_fake)
    {
        self::$_site_core_view_fake = $site_core_view_fake;
    }

    static function varsStripTags(&$vars)
    {
        foreach ($vars as $k=>&$val){
            if(is_array($val) == false){
                $val = strip_tags($val);
            }
        }
        return $vars;
    }
    static function getCurrentIp()
    {
        $ip = isset($_SERVER["HTTP_X_REAL_IP"]) ? $_SERVER["HTTP_X_REAL_IP"] : $_SERVER["REMOTE_ADDR"];
        //$ip = self::generateRandomIp();
        return $ip;
    }

    static function cutExessiveDebugInfo($debug_backtrace_arrray)
    {
        $finals_res = array();
        $save_debug_keys = array("file", "line", "function", "class");
        if(!empty($debug_backtrace_arrray)){
            foreach($debug_backtrace_arrray as $val){
                $new_debug_val = array();
                foreach($save_debug_keys as $debug_key){
                    if(isset($val[$debug_key])){
                        $new_debug_val[$debug_key] = $val[$debug_key]; 
                    }
                }
                $finals_res[] = $new_debug_val;
            }
        }
        return $finals_res;
    }

    static function validatePriceValue(&$price_param)
    {
        $price = floatval($price_param);
        if($price > 0){
            $price_param = $price;
            return true;
        }else{
            return false;
        }
    }

    static function microtimePeriod($time_start = null)
    {
        if(is_null($time_start)){
            $time_start = DEBUG_TIME_START;
        }
        $time = substr(microtime_float(microtime()) - DEBUG_TIME_START, 0, 5);
        return $time;
    }

    static function header($encoding = 'utf-8', $content_type = "text/html")
    {
        if(empty($encoding)){
            $encoding = 'utf-8';
        }
        $cmd = "content-type: {$content_type}; charset={$encoding}";
        header($cmd);
    }

    static function headerJson()
    {
        self::header('', 'application/json');
    }

    /**
     * @static
     * @param int $limit_sec - 0 means no time limit
     */
    static function setScriptRunningTime($limit_sec = 0)
    {
        set_time_limit($limit_sec);
    }

    static function isLiveVersionNow()
    {
        $is_live = false;
        $const_name = "LIVE_VERSION";
        if(defined($const_name) && constant($const_name) == 1){
            $is_live = true;
        }
        return $is_live;
    }

    static function isDebugVersionNow()
    {
        return false;
    }

    static function strRemoveFirstSymbol($src_str, $delete_symbol)
    {
        $final_str = $src_str;
        if(strpos($src_str, $delete_symbol) === 0){
            $final_str = substr($src_str, 1);
        }
        return $final_str;
    }

    /**
     * used for int validating of IN range
     * remains only >0 int numbers
     * @static
     * @param $csv_str - '23, "w2", 25 '
     * @return string '23, 25'
     */
    static function validateIntValuesInImplodedStr($csv_str)
    {
        $delim = ',';
        $final_res = array();
        $final_str = "";

        if(!empty($csv_str)){
            $parts = explode($delim, $csv_str);
            if(!empty($parts)){
                foreach($parts as $part_val){
                    $int_val = intval($part_val);
                    if($int_val > 0){
                        $final_res[] = $int_val;
                    }
                }

                $final_str = implode($delim, $final_res);
            }
        }

        return $final_str;
    }

    /**
     * @static
     * @param $input_val
     * @param $percent_val
     * @param int $precision - if null result value is not rounded (may stay still as int)
     * @return float
     */
    static function getPercentOfValue($input_val, $percent_val, $precision = 2)
    {
        $res = $input_val / 100 * $percent_val;

        if(!is_null($precision)){
            $res = round($res, $precision);
        }

        return $res;
    }

    //--- base64 safe url ---//
    static function base64UrlEncode($input) {
        return strtr(base64_encode($input), '+/=', '-_,');
    }

    static function base64UrlDecode($input) {
        return base64_decode(strtr($input, '-_,', '+/='));
    }

    static function arrayToUrlParam($array_input)
    {
        $final_str = "";
        if(!empty($array_input) && is_array($array_input)){
            $json_encoded = json_encode($array_input);
            $final_str = self::base64UrlEncode($json_encoded);
        }

        return $final_str;
    }

    static function arrayFromUrlParam($string_packed)
    {
        $res = array();
        if(!empty($string_packed)){
            $json_encoded = self::base64UrlDecode($string_packed);

            $json_DECODED = json_decode($json_encoded, true);
            //pa($json_encoded, $json_DECODED);

            if(is_array($json_DECODED)){
                $res = $json_DECODED;
            }
        }

        return $res;
    }

    static function getIntOrZero($input_raw)
    {
        $input_int = filter_var($input_raw, FILTER_VALIDATE_INT, array('default' => 0));
        return $input_int;
    }

    static function createOptionsSimple($sql, $field_key, $field_val)
    {
        $DB = DB::getInstance();
        $options = array();
        $DB->query($sql);
        while($row = $DB->fetch_row()){
            $options[$row[$field_key]] = $row[$field_val];
        }

        return $options;
    }

    static function getDocRootPath($relative_path)
    {
        return PATH_ROOT . $relative_path;
    }

    static function implodeForSqlInCondition($input_ar)
    {
        $imploded_str = self::validateIntValuesInImplodedStr(self::implodeWithComma($input_ar));
        EkvException::ensure(!empty($imploded_str),
            "Empty emploded str for 'SQL IN' condition");
        return $imploded_str;
    }
}
