<?php
namespace Ekv\Schedule\Formatting;
use DateTime;
use Ekv\Bank\PresenterFormatter;
use Ekv\Bank\ProgFormatter;
use Ekv\Helpers\System\Lang;
use Ekv\News\NewsFormatter;
use EkvGlobalHelper, EkvCommonHelper, EkvDateHelper;

class PeriodItemFormatter
{
    private $DB;

    private $_periodItemsRaw;

    private $_bankProgIds = array();
    private $_bankPresenterIds = array();
    private $_bankPresentersByPeriodItemId = array();
    private $_periodItemIds = array();

    private $_isLangMain;
    private $_langID;

    private $_extProgs = array();
    private $_extPresenters = array();

    private $_imgSize = "size1";

    const PI_IMG_URL_KEY = "url_prog_img";
    const PI_IMG_ORIGIN = "pi_image_origin";

    const IMG_ORIGIN_CHANNEL_LOGO = "logo";
    const IMG_ORIGIN_PROG_IMG = "prog_img";

    function __construct($lang_id = null)
    {
        $this->DB = \DB::getInstance();

        if(is_null($lang_id)){
            $lang_id = ekvLangID();
        }
        $this->_isLangMain = Lang::getInstance()->isLangIDMain($lang_id);
        $this->_langID = $lang_id;
    }

    function setImgSize($size_name)
    {
        $this->_imgSize = $size_name;
    }

    /**
     * @param $pi_raw - period item raw
     */
    function prepareBasicSingleItem(&$pi_raw)
    {
        //$audio_url_ext_data = array();
        $audio_relative_url = $this->_urlAudio($pi_raw, $audio_url_ext_data);
//        print_r($pi_raw);
        $pi_raw["has_audio"] = $audio_url_ext_data["file_exists"];
        $pi_raw["item_time_formatted"] = PeriodItemFormatter::formatTimeHHMin($pi_raw["item_time"]);
        $pi_raw["url_audio_relative"] = $audio_relative_url;
        $pi_raw["url_audio_domain"] = EkvCommonHelper::appendUrlWithSiteHostConf($pi_raw["url_audio_relative"]);
        $pi_raw["url_listen_popup"] = $this->_urlListenPopup($pi_raw);
        $pi_raw["is_show_listen_button"] = true;

        $newsId = $this->_getNews($pi_raw);
        $pi_raw["url_read_news"] = $newsId ? (new NewsFormatter)->getUrlPreview($newsId) : null;

        $this->_appendProgImg($pi_raw, $this->_imgSize);

        if (isset($pi_raw['date'])) {
            $timestampEnd = strtotime($pi_raw['date'] . ' ' . $pi_raw["item_time_end"]);

            if (time() - $timestampEnd < 1200) {
                $pi_raw["is_show_listen_button"] = false;
            }
        }
    }

    /**
     * @overridable
     * @param $res
     * @return string
     */
    protected function _urlListenPopup($res)
    {
        //$url_listen_popup = "/grid/channel/period/item-listen-popup.html?periodItemID={$res["id_item"]}";
        $url_listen_popup = "/schedule/play-archive.html?periodItemID={$res["id_item"]}";
        EkvCommonHelper::appendUrlWithLangPrefixRef($url_listen_popup);
        return $url_listen_popup;
    }

    /**
     * appends current prog img path or if img is abscent adds channel img path
     * @param $res
     * @param string $size
     */
    private function _appendProgImg(&$res, $size = "size2")
    {
        $img_url = "";
        $img_origin = self::IMG_ORIGIN_PROG_IMG;

        //--- photo from prog info ---//
        if(
            isset($res["prog_photo"])
            && !empty($res["prog_photo"])
        ){
            $img_url = "/images/grid/{$size}/{$res["prog_photo"]}";
        }elseif(isset($res["ch_logo"])){
            //--- photo from channel logo ---//
            $img_url = "/images/channel/logo/{$res["ch_logo"]}";
            $img_origin = self::IMG_ORIGIN_CHANNEL_LOGO;
        }

        $res[self::PI_IMG_URL_KEY] = $img_url;
        $res[self::PI_IMG_ORIGIN] = $img_origin;
    }

    /**
     * First check is there audio substitute uploaded via backend.
     * And then check auto generated file
     * If no audio found null is resturned
     * @param $item - period item
     * @param null $out_ext_data
     * @return string - audio relative url or null
     */
    private function _urlAudio($item, &$out_ext_data = null)
    {
        \EkvException::ensure(isset($item["audio_file_auto"]) && isset($item["audio_link"]),
            "Not all audio file fields are presented");

        $audio_url = null;

        if(!empty($item["audio_link"])){
            $audio_url = "/files/audio_manual/{$item["audio_link"]}";
        }elseif(!empty($item["audio_file_auto"])){
            $audio_url = $item["audio_file_auto"];
        }

        $file_exists = false;
        if(!empty($audio_url)){
            $path_full = PATH_ROOT . "{$audio_url}";
            $file_exists = file_exists($path_full);

            if(
                $file_exists
                /*
                 * There is possibly some cutting parser bug which creates tiny useless files (don't wanna fix it now)
                 * so just fix as a crutch by ignoring such small files
                 */
                && filesize($path_full) < 1000
            ){
                $file_exists = false;
            }
        }

        if(!isVersionLive()){
            $file_exists = true;
        }

        $out_ext_data = array(
            'file_exists' => $file_exists
        );

        return $audio_url;
    }

    //#------------------- / moved from PeriodItem /  -------------------#//


    /**
     * Formatting SINGLE period items, expanding bank fields
     * @param $period_item_single
     * @return mixed
     */
    function mainFormatSinglePeriodItemWithBankFields($period_item_single)
    {
        //var_dump($period_item_single);exit;
        $input = array($period_item_single);
        $multi_formated = $this->mainFormatPeriodItemsWithBankFields($input);
        return $multi_formated[0];
    }

    /**
     * Formatting MULTIPLE items at once, expanding bank fields (prog and presenters)
     * @param $period_items
     * @return array
     */
    function mainFormatPeriodItemsWithBankFields($period_items)
    {
        if(empty($period_items)){
            return array();
        }

        foreach($period_items as &$pi_val){
            $this->prepareBasicSingleItem($pi_val);
        }

        $this->_periodItemsRaw = $period_items;

        $this->_preProcessPeriodItems();

        $this->_setBankPresentersConnected();

        $this->_extProgs = $this->_expandProgs();
        $this->_extPresenters = $this->_expandPresenters();

        $period_items_prepared = $this->_purifyResponse();

        //pa($period_items_prepared);
        return $period_items_prepared;
    }

    /**
     * set unique prog_ids from bank connected and prepare period_items_ids
     */
    private function _preProcessPeriodItems()
    {
        $prog_ids = array();
        $period_items_ids = array();

        foreach($this->_periodItemsRaw as $val){
            $prog_id = intval($val["prog_id"]);
            $period_item_id = intval($val["id_item"]);

            //--- save only progs connected from bank ---//
            if($prog_id > 0){
                $prog_ids[$prog_id] = $prog_id;
            }

            $period_items_ids[] = $period_item_id;
        }

        $this->_bankProgIds = $prog_ids;
        $this->_periodItemIds = $period_items_ids;
    }

    /**
     * set unique presenter ids connected from bank
     */
    private function _setBankPresentersConnected()
    {
        $period_item_ids_str = \EkvCommonHelper::implodeWithComma($this->_periodItemIds);
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."`
            WHERE
                period_item_id IN ({$period_item_ids_str})
        ";
        $res = $this->DB->RunSql($sql, true);

        $presenter_ids = array();
        $presenters_by_period_item = array();
        if(!empty($res)){
            foreach($res as $rval){
                $presenter_id = intval($rval["presenter_id"]);
                $period_item_id = intval($rval["period_item_id"]);

                //--- unique presenters list ---//
                $presenter_ids[$presenter_id] = $presenter_id;

                //--- PI can have many presenters ---//
                $presenters_by_period_item[$period_item_id][] = $presenter_id;
            }
        }

        $this->_bankPresenterIds = $presenter_ids;
        $this->_bankPresentersByPeriodItemId = $presenters_by_period_item;
    }

    private function _expandProgs()
    {
        if(empty($this->_bankProgIds)){
            return array();
        }

        $prog_ids_str = EkvCommonHelper::implodeWithComma($this->_bankProgIds);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_PROG."`
            WHERE
                id_prog IN({$prog_ids_str})
        ";
        $res = $this->DB->RunSql($sql, true, "id_prog");
        return $res;
    }

    private function _expandPresenters()
    {
        if(empty($this->_bankPresenterIds)){
            return array();
        }

        $presenter_ids_str = EkvCommonHelper::implodeWithComma($this->_bankPresenterIds);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER."`
            WHERE
                id_presenter IN ({$presenter_ids_str})
        ";
        $res = $this->DB->RunSql($sql, true, "id_presenter");
        return $res;
    }

    private function _getNews($row)
    {
        $periodId = $row['id_item'];

        $sql = "
            SELECT news_id
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."`
            WHERE
                period_item_track_id = {$periodId}
                AND enabled = 1
                AND is_published = 1
            LIMIT 1
        ";

        $res = $this->DB->RunSql($sql);

        return current($res);
    }

    private function _purifyResponse()
    {
        $purified_period_items = array();
        foreach($this->_periodItemsRaw as $k => $pi_val){
            $tmp_period_item = $this->_purifyProg($pi_val);

            $tmp_period_item = $this->_purifyPresenters($tmp_period_item);

            $purified_period_items[$k] = $tmp_period_item;
        }

        return $purified_period_items;
    }

    /**
     * prepare prog info structure to be easily used in tpl. Whether frontend or email reports
     * @param $period_item
     * @return mixed
     */
    private function _purifyProg($period_item)
    {
        $prog_id = intval($period_item["prog_id"]);

        $from_bank = false;
        $display = array();
        $raw = array();


        //--- prog from bank connected ---//
        if($prog_id > 0){
            $from_bank = true;
            PeriodItemFormatterException::ensure( isset($this->_extProgs[$prog_id]),
                "No prog_info found in bank for ID#{$prog_id}" );

            $prog_info = $this->_extProgs[$prog_id];

            $progFormatterObj = new ProgFormatter($this->_langID);
            $progFormatterObj->prepareSingleItem($prog_info);
            $display = $prog_info["display"];

            /*
             * If the prog is connected to the network but the network item has its own description then keep net descr
             */

            $prog_descr_from_net = trim($period_item["prog_descr"]);
            if(!empty($prog_descr_from_net)){
                $display["prog_descr"] = $prog_descr_from_net;
            }
            $raw = $this->_extProgs[$prog_id];
        }else{
            //--- direct prog from period_item fields ---//
            $display = array(
                "prog_name" => $period_item["prog_name"],
//                "prog_latin_name" => $prog_info["prog_latin_name"],
                "prog_descr" => $period_item["prog_descr"]
            );
        }

        $pi_image_url = @$period_item[PeriodItemFormatter::PI_IMG_URL_KEY];
        $bank_image_url = isset($display[ProgFormatter::PROG_IMG_URL_KEY]) ? isset($display[ProgFormatter::PROG_IMG_URL_KEY]) : "";

        /*
         * If period item has image - keep it, if it doesn't have image and there is a prog linked which has it -
         * take image from the bank prog
         */
        if(
            empty($pi_image_url) // pi doesn't have image
            ||
            // url contains logo (direct image is absent)
            $period_item[PeriodItemFormatter::PI_IMG_ORIGIN] == PeriodItemFormatter::IMG_ORIGIN_CHANNEL_LOGO
        ){
            //--- replace img url ---//
            if(
                isset($display[ProgFormatter::PROG_IMG_URL_KEY])
                && !empty($display[ProgFormatter::PROG_IMG_URL_KEY]) // there is bank prog image url
            ){
                $period_item[PeriodItemFormatter::PI_IMG_URL_KEY] = $display[ProgFormatter::PROG_IMG_URL_KEY];
            }
        }

        $period_item["prog_name_display"] = $display["prog_name"];
        //if (ekvLangCode() != 'uk') $period_item["prog_name_display"] = $display["prog_latin_name"];
        $period_item["prog_info"] = array(
            "from_bank" => $from_bank,//bool
            "display" => $display,  // used in tpl to build net item markup
            "raw" => $raw, //i guees it's not used at the moment
        );

        return $period_item;
    }

    private function _purifyPresenters($period_item)
    {
        $pi_id = intval($period_item["id_item"]);

        $display = array();
        $raw = array();
        $has_presenters = true;
        $from_bank = true;

        //--- PI has presenters from bank ---//
        if(array_key_exists($pi_id, $this->_bankPresentersByPeriodItemId)){
            $presenters_connected_ids = $this->_bankPresentersByPeriodItemId[$pi_id];

            foreach($presenters_connected_ids as $pres_id){
                PeriodItemFormatterException::ensure(isset($this->_extPresenters[$pres_id]),
                    "Presenter info not found while formatting");

                $pres_info = $this->_extPresenters[$pres_id];

                $presFormatter = new PresenterFormatter($this->_isLangMain, $pres_info);
                $display[$pres_id] = $presFormatter->mainFormat();
                $presFormatter = null;

                $raw[$pres_id] = $pres_info;
            }
        }else{
            $from_bank = false;
            //--- show presenters from direct period item fields ---//
            $display[] = array(
                "id" => 0,
                "fio" => explode(',', $period_item["prog_presenter"]),
                "bio" => "",
                "url" => ""
            );

            if(empty($period_item["prog_presenter"])){
                $has_presenters = false;
            }
        }

        $period_item["presenter_info"] = array(
            "has_presenters" => $has_presenters,
            "from_bank" => $from_bank,
            "display" => $display,
            "raw" => $raw,
        );

        return $period_item;
    }

    static function formatDateAsMonthName($mysql_date)
    {
        $php_timestamp  = EkvDateHelper::mysqlDate2PhpDate($mysql_date);
        $raw = EkvDateHelper::getDateWithTranslatedMonth_DateOnly($php_timestamp);
        $parts = explode(' ', $raw);

//        $res = array(
//            'day_month' => "{$parts[0]} {$parts[1]}",
//            'year' => $parts[2]
//        );
//        $day_month = "{$parts[0]} {$parts[1]}";

        return $raw;
//        return $day_month;
    }

    static function formatTimeHHMin($time_full)
    {
        $res = null;
        $match_res = preg_match('#^([\d]{2}:[\d]{2})#', $time_full, $matches);
        if($match_res){
            $res = $matches[1];
        }
        return $res;
    }

}
