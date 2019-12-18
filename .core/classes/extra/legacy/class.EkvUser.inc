<?php
/**
 * User: sunsey
 * Date: 28.03.2011
 */
 
class EkvUser
{
    const PWD_SALT = "ekv";
    const USER_SALT = "ekv";
    const TABLE_USER = "ekv_user";
    const PROFILE_MIN_PWD_LENGTH = 3;
    const PROFILE_TMP_PWD_LENGTH = 6;
    const FIELDS_USER_BASE = "id, email, fio, phone, password, enabled";
    const SESSION_USER_KEY = "user_frontend";
    protected $DB;
    function __construct(DB $db = null)
    {
        $this->DB = DB::getInstance();
    }
    function initDB(){}

    function isUserEmailFree($email_new, $email_current = "")
    {
        $email_new_safe = mysql_real_escape_string($email_new);
        $SQL_CURRENT_EMAIL = "";
        if(!empty($email_current)){
            $email_current_safe = mysql_real_escape_string($email_current);
            $SQL_CURRENT_EMAIL = " AND `email` != '$email_current_safe' ";
        }
        $sql = "
            SELECT *
            FROM `".self::TABLE_USER."`
            WHERE
                `email` = '$email_new_safe'
                $SQL_CURRENT_EMAIL
        ";

        $this->DB->query($sql);
        if($this->DB->get_num_rows() > 0)return false;
        else return true;
    }

    function isPasswordValid($pwd)
    {
        $return = false;
        if(strlen($pwd) >= self::PROFILE_MIN_PWD_LENGTH){
            $return = true;
        }
        return $return;
    }

    function generatePwdHash($pwd)
    {
        $src = $pwd . self::PWD_SALT;
        $res = md5($src);
        return $res;
    }

    function generateUserHash($email)
    {
        $src = $email . self::USER_SALT;
        $res = md5($src);
        return $res;
    }

    function activateUserByHash($hash)
    {
        $uid = -1;
        $hash_safe = mysql_real_escape_string($hash);
        $user_info = $this->DB->bGetInfoById(self::TABLE_USER, $hash_safe, "hash");
        if(!empty($user_info)){
            $uid = $user_info["id"];
            $sql = "
                UPDATE `".self::TABLE_USER."`
                SET
                    `enabled` = '1'
            ";
            $this->DB->query($sql);

            $_SESSION["user_frontend"] = $user_info;
        }
        return $uid;
    }

    function getUserInfoByHash($hash, $fields = "*")
    {
        $hash_safe = mysql_real_escape_string($hash);
        $sql = "
            SELECT {$fields}
            FROM `".self::TABLE_USER."`
            WHERE
                `hash` = '$hash_safe'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

    function setUserDispatchSubscriptionField($uid, $val = 0)
    {
        $sql = "
            UPDATE `".self::TABLE_USER."`
            SET
                `dispatch_subscriber` = '$val'
            WHERE
                `id` = '$uid'
        ";
        $this->DB->query($sql);
    }

    function setUserDispatchAllSubscriptionFields($uid, $email_flag, $sms_flag)
    {
        $sql = "
            UPDATE `".self::TABLE_USER."`
            SET
                `dispatch_subscriber` = '$email_flag',
                `sms_dispatch_subscriber` = '$sms_flag'
            WHERE
                `id` = '$uid'
        ";
        $this->DB->query($sql);
    }


    function getBaseUserInfo($uid, $add_fields = "")
    {
        $full_fields = self::FIELDS_USER_BASE . $add_fields;
        $sql = "
            SELECT {$full_fields}
            FROM `".self::TABLE_USER."`
            WHERE
                `id` = '$uid'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

    function getFullUserInfo($uid)
    {
        $res = $this->DB->bGetInfoById(self::TABLE_USER, $uid);
        return $res;
    }

    function getBaseUserInfoByEmail($email, $add_fields = "")
    {
        $email = trim($email);
        $full_fields = self::FIELDS_USER_BASE . $add_fields;
        $email_safe = mysql_real_escape_string($email);
        $sql = "
            SELECT {$full_fields}
            FROM `".self::TABLE_USER."`
            WHERE
                `email` = '$email_safe'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }
    
    //--------
    // checks whether current referer is outer and return it or empty string
    //--------
    function checkOuterReferer($referer)
    {
        $base_referer = "user/login.html";
        $return = "";
        //--- no matches ---//
        if(strpos($referer, $base_referer) === false){
            $return = $referer;
        }
        return $return;
    }

    function tryLogin($email, $password, &$user_info_param_ref = null, $password_as_hashed_password = false)
    {
        $return = false;
        $email = trim($email);
        $email_safe = mysql_real_escape_string($email);
        if($password_as_hashed_password == false){
            $pass_encrypted = $this->generatePwdHash($password);
        }else{
            $pass_encrypted = $password;
        }
        $user_info = $this->getBaseUserInfoByEmail($email_safe, ", password");

        if(!empty($user_info)
            && $user_info["password"] == $pass_encrypted
            && $user_info["enabled"] == 1
          )
        {
            $_SESSION[self::SESSION_USER_KEY] = $user_info;
            $return = true;
            $user_info_param_ref = $user_info;
        }

        return $return;
    }

    function tryDoAutologin($email, $pwd_hash, &$user_info_param_ref = null)
    {
        $return = false;
        $email = trim($email);
        $email_safe = mysql_real_escape_string($email);
        $user_info = $this->getBaseUserInfoByEmail($email_safe, ", password");

        if(!empty($user_info)
            && $user_info["password"] == $pwd_hash
            && $user_info["enabled"] == 1
          )
        {
            $_SESSION[self::SESSION_USER_KEY] = $user_info;
            $return = true;
            $user_info_param_ref = $user_info;
        }

        return $return;
    }

    function setAutologinCookie($user_info)
    {
        $period = time()+60*60*24*365;
        setcookie("cook_user", $user_info["email"], $period, "/", COOKIE_DOMAIN);
        setcookie("cook_password", $user_info["password"], $period, "/", COOKIE_DOMAIN);
    }
    static function removeAutologinCookie()
    {
        setcookie("cook_user", "", time(), "/", COOKIE_DOMAIN);
        setcookie("cook_password", "", time(), "/", COOKIE_DOMAIN);        
    }

    static function getUserUidFromSession()
    {
        $ret = null;
        if(isset($_SESSION[self::SESSION_USER_KEY])){
            $ret = $_SESSION[self::SESSION_USER_KEY]["id"]; 
        }
        return $ret;
    }

    static function getUserInfoFromSession()
    {
        $ret = null;
        if(isset($_SESSION[self::SESSION_USER_KEY])){
            $ret = $_SESSION[self::SESSION_USER_KEY];
        }
        return $ret;
    }

    static function doLogout()
    {
        unset($_SESSION[self::SESSION_USER_KEY]);
        self::removeAutologinCookie();

    }

    function doAutoRegistration($VARS)
    {
        if(!empty($VARS["email"])){
            EkvCommonHelper::varsStripTags($VARS);
            $email_safe = mysql_real_escape_string($VARS["email"]);
            $fio_safe = mysql_real_escape_string($VARS["fio"]);
            $phone_safe = mysql_real_escape_string($VARS["phone"]);

            $user_info_check = $this->getBaseUserInfoByEmail($VARS["email"]);
            if( !empty( $user_info_check ) ){
                return false;
            }

            $enabled = 1;
            $pwd_rand = $this->generateTmpPassword(6);
            $pwd_hash = $this->generatePwdHash($pwd_rand);
            $user_hash = $this->generateUserHash($VARS["email"]);

            //--- location ---//
            $userLocationObj = new EkvUserLocation();
            $userLocationObj->maintainCorrectCityField($VARS);

            $country_id = (isset($VARS["user_country_id"]))? intval($VARS["user_country_id"]) : 0;
            $city_id = (isset($VARS["user_city_id"])) ? intval($VARS["user_city_id"]) : 0;
            $city_plain = isset($VARS["user_city"]) ? @mysql_real_escape_string($VARS["user_city"]) :  '';
            $city_name = $userLocationObj->getCityNameFromInputDataWithExtraQuery($VARS);

            $adr_street = isset($VARS["adr_street"]) ? mysql_real_escape_string($VARS["adr_street"]) : '';
            $adr_house = isset($VARS["adr_house"]) ? mysql_real_escape_string($VARS["adr_house"]) : '';
            $adr_apt = isset($VARS["adr_apt"]) ? mysql_real_escape_string($VARS["adr_apt"]) : '';

            $sql = "
                INSERT INTO `".self::TABLE_USER."`
                SET
                    `email` = '{$email_safe}',
                    `fio` = '{$fio_safe}',
                    `phone` = '{$phone_safe}',

                    `user_country_id` = $country_id,
                    `user_city_id` = $city_id,
                    `user_city` = '{$city_plain}',

                    `adr_street` = '{$adr_street}',
                    `adr_house` = '{$adr_house}',
                    `adr_apt` = '{$adr_apt}',

                    `enabled` = '{$enabled}',
                    `reg_date` = NOW(),
                    `password` = '{$pwd_hash}',
                    `hash` = '{$user_hash}'
            ";
            $this->DB->query($sql);
            
            $uid_inserted = $this->DB->get_insert_id();
            if($uid_inserted > 0){
                $user_info = $this->getFullUserInfo($uid_inserted);
                if(!empty($user_info)){
                    $this->setAutologinCookie($user_info);
                }
                //--- send email to admin ---//
                $prepared_ar =
                    array(
                        array("E-mail", $VARS["email"]),
                        array("Ф�?О", $VARS["fio"]),
                        array("Телефон", $VARS["phone"]),
                        array("Город", $city_name)
                    );
                $auto_str = EkvCommonHelper::autoStrFromPreparedArray($prepared_ar);
                $anchor = array("auto_str" => $auto_str);
                sendmail("REG_ADMIN_MAIL", ADMIN_EMAIL, $anchor);

                //--- send to user ---//

                $anchor =
                    array(
                        "email" => $VARS["email"],
                        "password" => $pwd_rand,
                    );
                sendmail("USER_REG_TO_USER", $VARS["email"], $anchor);
                
            }else{
                mail(ADMIN_EMAIL, "Internet-bilet. Auto-signup failed", print_r($VARS, true));
            }

            return $uid_inserted;
        }
    }

    function updateUserIdInOrder($base_order_id, $uid)
    {
        if($base_order_id > 0 && $uid > 0){
            $sql = "
                UPDATE `".EkvRoomHelper::TABLE_ORDER_BASE."`
                SET
                    `uid` = '$uid'
                WHERE
                    `id_order` = '$base_order_id'
            ";
            $this->DB->query($sql);
        }
    }

    function generateTmpPassword($pwd_length = null)
    {
        if(is_null($pwd_length)){
            $pwd_length = self::PROFILE_TMP_PWD_LENGTH;
        }
        list($usec, $sec) = explode(' ', microtime());
        srand((float) $sec + ((float) $usec * 100000));
        $n = rand(0, 999999);
        $n = md5($n);
        $n = str_shuffle($n);
        return substr($n, 0, $pwd_length);
    }

    function generateTmpPasswordHash()
    {
        $pwd_rand = $this->generateTmpPassword();
        $hash = $this->generatePwdHash($pwd_rand);
        return $hash;
    }

    function updateUserPassword($uid, $new_pwd_hash)
    {
        $sql = "
            UPDATE `".self::TABLE_USER."`
            SET
                `password` = '{$new_pwd_hash}'
            WHERE
                `id` = '$uid'
        ";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $this->DB->query($sql);
    }

    function getUserOptions($type = "fio_id", $single_uid = null, $multiple_uids = array())
    {
        $SQL_UID_FILTER = "";
        //--- single uid ---//
        if($single_uid > 0){
            $single_uid = intval($single_uid);
            $SQL_UID_FILTER = " AND id = '{$single_uid}' ";
        }

        //--- multiple uids ---//
        $SQL_UID_MULTI = "";
        if(
            !empty($multiple_uids)
            && is_array($multiple_uids)
        ){
            $multiple_uids_str = EkvCommonHelper::implodeWithComma($multiple_uids);
            $SQL_UID_MULTI = " AND id IN({$multiple_uids_str}) ";
        }

        $sql = "
            SELECT ".self::FIELDS_USER_BASE."
            FROM `".self::TABLE_USER."`
            WHERE
                `enabled` = 1
                {$SQL_UID_FILTER}
                {$SQL_UID_MULTI}
            ORDER BY
                `fio` ASC
        ";
        $this->DB->query($sql);
        $res = array();
        $pattern = 'return "{$row["fio"]}[{$row["id"]}]";';
        if($type == 'fio_only'){
            $pattern = 'return "{$row["fio"]}";';
        }

        while($row = $this->DB->fetch_row()){
            //$res[$row["id"]] = "{$row["fio"]}[{$row["id"]}]";
            $res[$row["id"]] = eval($pattern);
        }

        return $res;
    }

    function appendDefaultOption(&$options)
    {
        $options = array('' => DEFAULT_SELECT_NAME) + $options;
        return $options;
    }

    function prepareVarsForLoginPerforming(&$VARS, $email, $password_hash, &$USE_PASSWORD_HASH_INSTEAD_PLAIN_PASSWORD)
    {
        $VARS["email"] = $email;
        $VARS["password"] = $password_hash;
        $VARS["is_form_submited"] = 1;
        $VARS["autologin"] = 1;
        $VARS["return_url"] = "/";
        $USE_PASSWORD_HASH_INSTEAD_PLAIN_PASSWORD = true;
    }

}

class EkvUserOpenid extends EkvUser
{
    const OPENID_TYPE_FB = 1,
        OPENID_TYPE_VK = 2,
        OPENID_TYPE_YANDEX = 3,
        OPENID_TYPE_MAILRU = 5,
        OPENID_TYPE_GOOGLE = 4
    ;

    static function getOpenidProviderIntType($provider_str)
    {
        $int_type = 0;
        if($provider_str == "http://www.facebook.com/"){
            $int_type = self::OPENID_TYPE_FB;
        }
        else if($provider_str == "http://vkontakte.ru/"){
            $int_type = self::OPENID_TYPE_VK;
        }
        else if($provider_str == "https://www.google.com/accounts/o8/ud"){
            $int_type = self::OPENID_TYPE_GOOGLE;
        }
        else if($provider_str == "http://openid.yandex.ru/server/"){
            $int_type = self::OPENID_TYPE_YANDEX;
        }
        else if($provider_str == "http://mail.ru/"){
            $int_type = self::OPENID_TYPE_MAILRU;
        }


        return $int_type;
    }

    function getUserInfoByOpenidUid($openid_uid)
    {
        $openid_uid_safe = mysql_real_escape_string($openid_uid);
        $sql = "
            SELECT *
            FROM `".EkvRoomHelper::TABLE_USERS."` u
            WHERE
                u.id IN
                (
                    SELECT oid.uid_local
                    FROM `".EkvRoomHelper::TABLE_USERS_OPENIDS."` oid
                    WHERE
                        oid.uid_openid = '$openid_uid_safe'
                )
            LIMIT 1
        ";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql);
        //pa($res);
        return $res;
    }

    function registerUserByOpenidData($openid_data)
    {
        $prepared_reg_data = array();
        //--- email ---//
        $prepared_reg_data["email"] = $openid_data["email"];
        //--- fio ---//
        $fio = "";
        $openid_name_data = $openid_data["name"];
        if(isset($openid_name_data["full_name"])){
            $fio = $openid_name_data["full_name"];
        }
        else if(isset($openid_name_data["last_name"])){
            $fio = $openid_name_data["last_name"];
            if(isset($openid_name_data["first_name"])){
                $fio .= " ".$openid_name_data["first_name"];
            }
        }
        else if(isset($openid_name_data["first_name"])){
            $fio = $openid_name_data["first_name"];
            if(isset($openid_name_data["last_name"])){
                $fio .= " ".$openid_name_data["last_name"];
            }
        }
        $prepared_reg_data["fio"] = $fio;
        //--- phone ---//
        $prepared_reg_data["phone"] = "";

        //--- do autoreg ---//
        $new_user_id = $this->doAutoRegistration($prepared_reg_data);
        if($new_user_id > 0){
            $openid_int_type = self::getOpenidProviderIntType($openid_data["provider"]);
            $this->connectOpenidData2RealUser($new_user_id, $openid_data["uid"], $openid_int_type);
        }
        return $new_user_id;
    }

    function connectOpenidData2RealUser($real_uid, $openid_uid, $openid_type)
    {
        $real_uid = (int)$real_uid;
        $openid_uid_safe = mysql_real_escape_string($openid_uid);
        $openid_type = (int)$openid_type;

        $sql = "
            INSERT IGNORE INTO `".EkvRoomHelper::TABLE_USERS_OPENIDS."`
            SET
                `uid_local` = $real_uid,
                `uid_openid` = '$openid_uid_safe',
                `openid_type` = $openid_type
        ";
        $this->DB->query($sql);
        return $this->DB->get_insert_id();
    }

    static function logOpenidData($data)
    {
        $log_fname = PATH_ROOT . "/.temp/logs/openid.log";

        $str2file = "\n-----------------".EkvDateHelper::phpDate2MysqlDate(time())."------------------\n";
        $str2file .= EkvCommonHelper::packArray2Text($data);
        $str2file .= "\n";
        $str2file .= print_r($data, true);

		$f = @fopen($log_fname, "a");
		if(!empty($f))
		{
			fwrite($f, $str2file);
			fclose($f);
		}        
    }

    function getUserInfoByTokenApiCall($token)
    {
		$res = EkvCommonHelper::fileGetContents("http://loginza.ru/api/authinfo?token=".$token);
		$jsondata = json_decode($res, true);
        return $jsondata;
    }

    /**
     * sometimes uid is not passed so we should parse it from identity
     * @param  string $identity - openid_json_data["identity"]
     * @return identity or false
     */
    function getTextUidFromIdentity($identity)
    {
		// шаблоны по которым выцепляем ник из identity
		$patterns = array(
			'([^\.]+)\.ya\.ru',
			'openid\.mail\.ru\/[^\/]+\/([^\/?]+)',
			'openid\.yandex\.ru\/([^\/?]+)',
			'([^\.]+)\.myopenid\.com'
		);
		foreach ($patterns as $pattern) {
			if (preg_match('/^https?\:\/\/'.$pattern.'/i', $identity, $result)) {
                $final_uid = $result[1];
                $final_uid = str_replace("uid:", "", $final_uid);
                $final_uid = trim($final_uid);
				return $final_uid;
			}
		}
		return false;
    }

    
}
