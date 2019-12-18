<?php

class EkvCache{
	const TABLE = "ekv_cache";
    static public $DB = null;
 	static function initDb($db = null)
 	{
        if(is_null($db)){
            self::$DB = EkvCommonHelper::getDB();
        }else{
            self::$DB = $db;
        }
	}
	
	static function setCacheVal($key, $value)
	{
        self::initDb();
		//echo "<h2>make cache</h2>";		
		//$value = serialize($value);
        $value = EkvCommonHelper::packArray2Text($value);
		$value_safe = mysql_real_escape_string($value);
		$sql = "
			REPLACE INTO `".self::TABLE."`
			SET
				`key` = '$key',
				`value` = '$value_safe',
				`date` = NOW()
		";
		self::$DB->query($sql);
	}

    /**
     * if data is expired or doesn't exist ret false else array
     * @static
     * @param  $key
     * @param  $CACHE_PERIOD_SEC
     * @return bool|array 
     */
	static function getCacheVal($key, $CACHE_PERIOD_SEC = null)
	{
        if(is_null($CACHE_PERIOD_SEC)){
            $CACHE_PERIOD_SEC = 5*60;
        }
        self::initDb();
		//echo "<h2>get cache</h2>";
		$sql = "
			SELECT `key`, `value`, UNIX_TIMESTAMP(`date`) as `date_tstmp`
			FROM `".self::TABLE."`
			WHERE
				`key` = '$key'
		";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
		$tmp = self::$DB->RunSql($sql);
		//$res = unserialize($tmp["value"]);
		$res = EkvCommonHelper::unpackArray2Text($tmp["value"]);
		if (!is_array($res)) {
			return false;
		}else{
			$return = array(
						"data" => $res,
						"cache_date" => $tmp["date_tstmp"]	
					);
			$return["is_actual"]  = ( ($tmp["date_tstmp"] + $CACHE_PERIOD_SEC) > time() ) ? true : false;

            if($return["is_actual"] == false){
                $return = false;
            }			
			return $return ;
		}
	}
	
}

?>