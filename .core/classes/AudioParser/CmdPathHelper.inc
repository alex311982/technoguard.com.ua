<?php
namespace Ekv\AudioParser;
use EkvGlobalHelper, DB, EkvCommonHelper;

class CmdPathHelper
{
    private $_periodFullInfo;

    private $_datePrefix;
    private $_channelPrefix;

    function __construct($period_full_info)
    {
        $this->_periodFullInfo = $period_full_info;

        ParseException::ensure(isset($period_full_info["ch_path_prefix"]) && isset($period_full_info["per_date"]),
            "Some required path helper fields can't be found");

        $this->_channelPrefix = $period_full_info["ch_path_prefix"];
        $this->_datePrefix = $this->_prepareDatePrefix($period_full_info["per_date"]);

    }

    private function _prepareDatePrefix($date_raw)
    {
        $date_prefix = str_replace('-', '', $date_raw);
        return $date_prefix;
    }

    function composeSrcBasePath()
    {
        $path = "/audio/{$this->_channelPrefix}/{$this->_datePrefix}/";
        return $path;
    }

    function composeDestPath()
    {
        //$path = "/audio_slice/{$this->_channelPrefix}/{$this->_datePrefix}/generation_13589449899956/";
        $path_relative = "/audio_slice/{$this->_channelPrefix}/{$this->_datePrefix}/";

        $this->ensureDestPathExistence($path_relative);

        return $path_relative;
    }

    function ensureDestPathExistence($dest_path_relative)
    {
        $dest_path_full = EkvCommonHelper::getDocRootPath($dest_path_relative);
        @mkdir($dest_path_full, 0755, true);
        ParseException::ensure(file_exists($dest_path_full), "Destination path doesn't exist");
    }

    //private function _createGenerationPrefix(){}
}
