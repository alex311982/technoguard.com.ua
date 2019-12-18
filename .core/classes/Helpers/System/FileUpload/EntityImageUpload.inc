<?php
namespace Ekv\Helpers\System\FileUpload;

use Ekv\Helpers\System\FileUpload\Exception\EntityImageUploadException;
use Ekv\Helpers\System\FileUpload\Exception\FileUploadException;
use EkvGlobalHelper, Entity, imgResizeSettings;

/**
 * Helper which facilitates image uploading and resizing.
 * Depending on errors already occured decides whether to perfrom upload or not.
 * Displays appropiate errors in terms of current backend (using Entity::AddTextError())
 */
class EntityImageUpload
{
    private $_entityObj;
    private $_isUploadRequired;
    private $_resizeSettings;
    private $_vars;

    private $_originalPathAbsolute;

    private $_fileHasBeenUploaded = false;

    private $_inputNamePlain;//without '_input_'
   	private $_filenamePrefix;
   	private $_files;

    function __construct(Entity $entityObj, $resizeSettingsKey, &$vars, $isUploadRequired = false)
    {

        $this->_entityObj = $entityObj;
        $this->_isUploadRequired = $isUploadRequired;
        $this->_resizeSettings = imgResizeSettings::getSettingsItem($resizeSettingsKey);
        //pa($this->_resizeSettings);exit;

        $this->_ensure(
            isset($this->_resizeSettings["base_photo_path"]),
            "Invalid resize settings, base_photo_path key not found"
        );
        $this->_originalPathAbsolute = "{$this->_resizeSettings["base_photo_path"]}/original/";

        $this->_vars = &$vars;
    }

    function setExtraParams($FILES, $inputNamePlain, $filenamePrefix)
    {
        $this->_inputNamePlain = $inputNamePlain;
        $this->_filenamePrefix = $filenamePrefix;
        $this->_files = $FILES;
    }

    private function _ensure($expression, $fail_msg)
    {
        EntityImageUploadException::ensure($expression, $fail_msg);
    }

    /**
     * Must be used in conjunction with setExtraParams
     * @return string
     */
    function mainUploadAndResizeSimplified()
    {
        return $this->mainUploadAndResize($this->_files, $this->_inputNamePlain, $this->_filenamePrefix);
    }

    function mainUploadAndResize($FILES, $input_name_plain, $filename_prefix)
    {
        if($this->_entityObj->IsError()){
            return "";
        }

        //--- create fileUpload object ---//
        $fileUploadObj = new FileUpload(
            $FILES,
            $input_name_plain,
            $this->_originalPathAbsolute,
            $filename_prefix
        );

        $has_upload_errors = false;
        $upload_res = null;
        //--- try upload performing and check for possible errors ---//
        try{
            $upload_res = $fileUploadObj->mainUpload($this->_isUploadRequired);
        }catch(FileUploadException $fileUpExc){
            //--- get error prefix from db fields ---//
            $err_prefix = "";
            if(isset($this->_entityObj->DB->fields[$input_name_plain]["title"])){
                $err_prefix = "{$this->_entityObj->DB->fields[$input_name_plain]["title"]}";
            }

            //---  add error message to entity ---//
            $this->_entityObj->AddTextError("{$err_prefix}: " . $fileUpExc->getMessage());
            $has_upload_errors = true;
        }

        //--- if none uploading errors has occured - try img resizing ---//
        if(
            !$has_upload_errors
            && !empty($upload_res) // file has been really uploaded
        ){
            $imgUploadObj = new ImageUpload(
                $upload_res["dest_path_absolute"],
                $upload_res["filename_clear"],
                $this->_resizeSettings
            );

            $imgUploadObj->mainPerformResize();

            $this->_vars[$input_name_plain] = $upload_res["filename_clear"];
        }

        $filename_clear = "";
        if(
            !$has_upload_errors
            && !empty($upload_res)
        ){
            $filename_clear = $upload_res["filename_clear"];
            $this->_fileHasBeenUploaded = true;
        }
        return $filename_clear;
    }

    /**
     * mainUploadAndResize() must be called before this method
     * @param $photo_name
     */
    function removeAllSizePhotosDependingOnUploading($photo_name)
    {
        if($this->_fileHasBeenUploaded){ //!!!
            $this->removeAllSizePhotos($photo_name);
        }
    }

    function removeAllSizePhotosDependingOnUploadingByOldInfo($oldInfo)
    {
        $oldPhotoName = $oldInfo[$this->_inputNamePlain];
        $this->removeAllSizePhotosDependingOnUploading($oldPhotoName);
    }

    function removeAllSizePhotos($photo_name)
    {
        if (empty($photo_name)) {
            return false;
        }

        $sizes = $this->_resizeSettings["size_settings"];
        $sizes[-1] = array("size_dir" => "original");

        $path_base = $this->_resizeSettings["base_photo_path"];

        foreach ($sizes as $val) {
            $full_path = "{$path_base}/{$val["size_dir"]}/$photo_name";
            @unlink($full_path);
        }
        return true;
    }

    /**
     * Check delete checkbox
     * @param $oldInfo
     * @return bool
     */
    function deleteImageOnEditingPageConditional($oldInfo)
    {
        $checkboxDelete = @intval($this->_vars["delete_{$this->_inputNamePlain}"]);

        $res = false;
        if($checkboxDelete){
            $oldPhotoName = @$oldInfo[$this->_inputNamePlain];
            $this->removeAllSizePhotos($oldPhotoName);
            $this->_vars[$this->_inputNamePlain] = "";
            $res = true;
        }

        return $res;
    }

    /**
     * Check delete checkbox
     * @param $oldInfo
     * @return bool
     */
    function deleteImageOnEditingPageConditionalVARS($oldInfo, &$VARS, $photo_field_name)
    {
        $checkboxDelete = @intval($VARS["delete_{$photo_field_name}"]);

        $res = false;
        if($checkboxDelete){
            $oldPhotoName = @$oldInfo[$photo_field_name];
            $this->removeAllSizePhotos($oldPhotoName);
            $VARS[$photo_field_name] = "";
            $res = true;
        }

        return $res;
    }

    /**
     * @param boolean $isUploadRequired
     */
    public function setIsUploadRequired($isUploadRequired)
    {
        $this->_isUploadRequired = $isUploadRequired;
    }

}
