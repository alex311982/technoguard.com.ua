<?php

class EkvFileUpload
{
    private $_filesKey;
    private $_uploadRequired;
    private $_savePath;
    private $_errors;

    private $_extension;
    private $_extAllowed;

    private $_FILES;

    function __construct($files_key, $save_path, $ext_allowed, $upload_required = false)
    {
        $this->_filesKey = $files_key;
        $this->_uploadRequired = (bool)$upload_required;
        $this->_savePath = $save_path;
        $this->_extAllowed = $ext_allowed;
        @mkdir($this->_savePath, 0777, true);

        $this->_FILES = $_FILES;
    }

    private function _errorAdd($err_msg)
    {
        $this->_errors[] = $err_msg;
    }

    function getErrors()
    {
        return $this->_errors;
    }

    function mainUpload()
    {
        $fname = null;
        if(isset($this->_FILES[$this->_filesKey])){
            $this->_calcExtension();
            if($this->_checkExt()){
                $fname = $this->_generateUniqueFilename();
                $dest_path = $this->_getDestPath($fname);
                move_uploaded_file($this->_getFilesArrayItem("tmp_name"), $dest_path);
            }
        }

        return $fname;
    }

    function deletePrevious($old_fname)
    {
        $old_path = $this->_getDestPath($old_fname);
        @unlink($old_path);
    }

    private function _getDestPath($fname)
    {
        $res = $this->_savePath . $fname;
        return $res;
    }

    private function _checkExt()
    {
        $res = false;

        if(in_array($this->_extension, $this->_extAllowed)){
            $res = true;
        }else{
            $this->_errorAdd("Некоректний файл (extension)");
        }
        return $res;
    }

    private function _getFilesArrayItem($key)
    {
        $res = null;
        if(isset($this->_FILES[$this->_filesKey][$key])){
            $res = $this->_FILES[$this->_filesKey][$key];
        }
        return $res;
    }

    private function _calcExtension()
    {
        $ext = pathinfo($this->_getFilesArrayItem('name'), PATHINFO_EXTENSION);
        $this->_extension = $ext;

        return $ext;
    }

    private function _generateUniqueFilename()
    {
        $res = uniqid("", true);
        $res .= ".{$this->_extension}";
        return $res;
    }
}
