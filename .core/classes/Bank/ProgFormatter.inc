<?php
namespace Ekv\Bank;

use Ekv\Helpers\System\Lang;

class ProgFormatter
{
    private $DB;
    private $_langID;
    private $_imgSize = 'size1';

    const PROG_IMG_URL_KEY = "url_img";

    function __construct($langID = 0)
    {
        $this->DB = \DB::getInstance();
        if($langID > 0){
            $this->_langID = intval($langID);
        }else{
            $this->_langID = Lang::getLangIDStatic();
        }

        //\EkvException::ensure($this->_langID > 0, "Invalid langID");
    }

    function setImgSize($size_name)
    {
        $this->_imgSize = $size_name;
    }

    function getUrl($prog_id)
    {

        if (Lang::getInstance()->getLangCode() != 'uk') $url = "/".Lang::getInstance()->getLangCode(); else $url = '';

        return $url."/prog.html?id={$prog_id}";
    }

    function getImageUrl($photo_file, $size = "size1")
    {
        $url = "";
        if(!empty($photo_file)){
            $res = \imgResizeSettings::getSettingsItem("progPhotos");

            $url = "{$res["base_photo_path_relative"]}{$size}/{$photo_file}";
        }
        return $url;
    }

    function prepareSingleItem(&$raw)
    {
        if(Lang::getInstance()->isLangIDMain($this->_langID) OR $raw["prog_latin_name"] == ''){
            $prog_name = $raw["prog_name"];
            $prog_descr = $raw["prog_descr"];
        }else{
            $prog_name = $raw["prog_latin_name"];
            $prog_descr = $raw["prog_latin_descr"];
        }

        $display = array(
            "prog_name" => $prog_name,
            'url' => $this->getUrl($raw["id_prog"]),
            self::PROG_IMG_URL_KEY => $this->getImageUrl($raw["photo_file"], $this->_imgSize),
            "prog_descr" => $prog_descr
        );

        $raw["display"] = $display;
    }

    public function prepareMultipleItems(&$items)
    {
        foreach($items as &$item_val){
            $item_val = $this->prepareSingleItem($item_val);
        }
        //return $items;
    }

}
