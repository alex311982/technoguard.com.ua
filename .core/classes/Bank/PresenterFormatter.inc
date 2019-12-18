<?php
namespace Ekv\Bank;

/**
 * only bank presenter formatter
 */
class PresenterFormatter
{
    private $_isLangMain;
    private $_langKeyMaps;
    private $_presInfo;

    function __construct($is_lang_main, $presenter_info)
    {
        $this->_isLangMain = $is_lang_main;
        $this->_defineLangKeyMaps();

        \EkvException::ensure(!empty($presenter_info), "Empty presenter info for formatting");
        $this->_presInfo = $presenter_info;
    }

    private function _defineLangKeyMaps()
    {
        $this->_langKeyMaps = array(
            "fio" => "fio",
            "bio" => "bio"
        );

        if($this->_isLangMain == false){
            $this->_langKeyMaps = array(
                "fio" => "latin_fio",
                "bio" => "latin_bio"
            );
        }
    }

    private function _presenterPageUrl($pres_id)
    {
        $url = "/prog-presenter.html?id={$pres_id}";
        return $url;
    }

    /**
     * @param bool $show_extra_fields - used as true for presenters page only
     * @return array
     */
    function mainFormat($show_extra_fields = false)
    {
        $formatted = array(
            "id" => $this->_presInfo["id_presenter"],
            "fio" => $this->_presInfo[$this->_langKeyMaps["fio"]],
            "url" => \EkvCommonHelper::appendUrlWithLangPrefix($this->_presenterPageUrl($this->_presInfo["id_presenter"])),
            "bio" => $this->_presInfo[$this->_langKeyMaps["bio"]],
        );

        if($show_extra_fields){
            $formatted_ext = array(
                'image' => $this->_formatImages()
            );

            $formatted = array_merge($formatted, $formatted_ext);
        }
        return $formatted;
    }

    private function _formatImages()
    {
        $photo_file = isset($this->_presInfo["photo_file"]) ? $this->_presInfo["photo_file"] : null;
        $final_res = array(
            'has_image' => false,
            'sizes' => array()
        );

        if(!empty($photo_file)){
            $final_res["has_image"] = true;

            $final_res["sizes"] = \imgResizeSettings::getFullSizesList(
                "presenterPhotos",
                $this->_presInfo["photo_file"],
                "relative"
            );
        }
        return $final_res;
    }
}
