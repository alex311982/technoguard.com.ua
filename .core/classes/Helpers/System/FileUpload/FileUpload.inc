<?php
namespace Ekv\Helpers\System\FileUpload;

use Ekv\Helpers\System\FileUpload\Exception\FileUploadException;

/**
 * Responsive for file uploading only, checks file type, max size.
 * If you are uploading image, then only original image will be copied here to the path provided
 */
class FileUpload
{
    const ERR_MAX_SIZE_OVERFLOW = "Максимальний розмір файла %max_size% Кб";
    const ERR_TYPE_INVALID = "Некорректный тип файла";
    const ERR_UPLOAD_FAILED = "Неможливо завантажити файл";
    const ERR_INIT_UPLOAD_FAILED = "Помилка при ініціалізації завантаження файла";

    /**
     * Full copy of  $_FILES
     */
    private $_FILES;

    /**
     * input name like "photo_file" NOT "_input_photo_file"
     */
    private $_inputNamePlain;
    private $_inputNameFull;

    private $_destPath;
    private $_fileNamePrefix;

    private $_fileTypesAllowed = array('jpg', 'png', 'gif');
    /**
     * @var int - in bytes
     */
    private $_maxFileSize = 0;

    private $_isUploading = false;
    private $_fileTypeCurrent;

    /**
     * @param $FILES
     * @param $input_name_plain - "photo_file"
     * @param $dest_path_absolute - "/home/sched.dev/www\images/news/original/"
     * @param string $filename_prefix - "news" - prefix for uploaded filename
     */
    function __construct($FILES, $input_name_plain, $dest_path_absolute, $filename_prefix = "prefix")
    {
        $this->_FILES = $FILES;
        $this->_inputNamePlain = $input_name_plain;
        $this->_inputNameFull = $this->_getInputName($input_name_plain);

        if(!file_exists($dest_path_absolute)){
            @mkdir($dest_path_absolute, 0775, true);
        }
        $this->_destPath = $dest_path_absolute;

        $this->_fileNamePrefix = $filename_prefix;
    }

    private function _getInputName($name_plain)
    {
        return "_input_{$name_plain}";
    }

    private function _hasFilesItem($key)
    {
        return isset($this->_FILES[$this->_inputNameFull][$key]);
    }

    private function _getFilesItem($key)
    {
        //FileUploadException::ensure($this->_hasFilesItem($key), "Unabled to find \$_FILES key '{$key}'");
        if($this->_hasFilesItem($key)){
            return $this->_FILES[$this->_inputNameFull][$key];
        }
        return null;
    }

    /**
     * @param bool $isUploadRequired - is upload required or optional
     * @return array|bool
     */
    function mainUpload($isUploadRequired = false)
    {
        if(
            !$isUploadRequired
            && $this->_getFilesItem('error') == 4 //no uploading is being performed
        ){
            return false;
        }

        FileUploadException::ensure(
            $this->_getFilesItem('error') == 0 && $this->_hasFilesItem("tmp_name"),
            self::ERR_INIT_UPLOAD_FAILED
        );

        $this->_isUploading = true;

        $this->_fileTypeCurrent = DefineFileType($this->_getFilesItem("type"));
        $this->_checkType($this->_fileTypeCurrent);

        $this->_checkMaxFileSize();

        $upload_res = $this->_moveUploadedFile();

        return $upload_res;
    }

    private function _createAbsoluteDestPath(&$out_file_name)
    {
        $file_prefix_original = "{$this->_fileNamePrefix}_".uniqid(time()."_");
        $filename = "{$file_prefix_original}.{$this->_fileTypeCurrent}";
        $path_new = "{$this->_destPath}/$filename";

        $out_file_name = $filename;
        return $path_new;
    }

    private function _moveUploadedFile()
    {
        $filename_clear = "";
        $dest_path_full = $this->_createAbsoluteDestPath($filename_clear);

        $src_file = $this->_getFilesItem('tmp_name');

        $upload_res = copy($src_file, $dest_path_full);
        FileUploadException::ensure($upload_res, self::ERR_UPLOAD_FAILED);

        return array(
            'dest_path_absolute' => $dest_path_full,
            'filename_clear' => $filename_clear
        );
    }

    private function _checkMaxFileSize()
    {
        /*
         * Ignore max size check when no max value is set
         */
        if($this->_maxFileSize == 0){
            return false;
        }

        $current_size = $this->_getFilesItem('size');

        $err_msg = str_replace("%max_size%", $current_size/1024, self::ERR_MAX_SIZE_OVERFLOW);

        FileUploadException::ensure(
            $current_size <= $this->_maxFileSize,
            $err_msg
        );

        return true;
    }

    private function _checkType($type)
    {
        FileUploadException::ensure(
            in_array($type, $this->_fileTypesAllowed),
            self::ERR_TYPE_INVALID
        );
    }
}
