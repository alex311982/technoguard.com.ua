<?php
namespace Ekv\Helpers\System\FileUpload;

use Ekv\Helpers\System\FileUpload\Exception\ImageUploadException;

/**
 * Creates proper image sizes from original image using resizeSettings
 * Creates images in proper sizes like 'size1', 'big' etc
 */
class ImageUpload
{
    private $_imgOriginalPathAbsolute;
    private $_imgOriginalFilename;
    //private $_resizeSettings;

    private $_basePhotoPathAbs;
    private $_sizeSettings;

    /**
     * @param $original_img_path_absolute
     * @param $original_img_filename
     * @param $resize_settings - gotten by imgResizeSettings::getSettingsItem($resizeSettingsKey)
     */
    function __construct($original_img_path_absolute, $original_img_filename, $resize_settings)
    {
        $this->_imgOriginalPathAbsolute = $original_img_path_absolute;
        $this->_imgOriginalFilename = $original_img_filename;

        ImageUploadException::ensure(
            isset($resize_settings["size_settings"])
            && isset($resize_settings["base_photo_path"]),
            "Invalid resize setting param"
        );

        $this->_basePhotoPathAbs = $resize_settings["base_photo_path"];
        $this->_sizeSettings = $resize_settings["size_settings"];
    }


    function mainPerformResize()
    {
        foreach ($this->_sizeSettings as $size_val) {
            $dir = $size_val["size_dir"];
            $size_file_path = "{$this->_basePhotoPathAbs}{$dir}/{$this->_imgOriginalFilename}";

            $creating_file_path_dirname = pathinfo($size_file_path, PATHINFO_DIRNAME);
            if (!file_exists($creating_file_path_dirname)) {
                mkdir($creating_file_path_dirname, 0755, true);
            }

            $this->_performResize($size_val, $size_file_path);

        }
    }

    private function _performResize($size_val, $size_file_path)
    {
        $simg_obj = new \Simage();
        $path_original = $this->_imgOriginalPathAbsolute;

        switch (@$size_val["resize_method"]) {

            case "resize_proportional_by_width":
                $simg_obj->resize_proportional_by_width($path_original, $size_file_path,
                    $size_val["size_single"], $new_h = null, $sharped = false);
                break;

//                case "resize_proportional_by_height":
//                    $simg_obj->resize_proportional_by_height($path_original, $size_file_path, $size_val["size_single"]);
//                    break;
//                case "resize_smaller_side":
//                    $simg_obj->resize_smaller_side($path_original, $size_file_path,
//                        $size_val["size_single"], $new_h = null, $sharped = false);
//                    break;
//
//                case "resize_proportional_and_cut_by_width":
//                    $simg_obj->resize_proportional_and_cut_by_width($path_original, $size_file_path,
//                        $size_val["size_width"], $size_val["size_height"]);
//                    break;
//                case "resize_toch_outside":
//                    $simg_obj->resize_toch_outside($path_original, $size_file_path,
//                        $size_val["size_width"], $size_val["size_height"]);
//                    break;
//                case "resize_propotional_exact":
//                    $simg_obj->ResizeProportionalExact($path_original,
//                        pathinfo($size_file_path, PATHINFO_DIRNAME), $size_file_path,
//                        $size_val["size_width"], $size_val["size_height"]);
//                    break;
//                case "resizeByWidthAndCropByHeight":
//                    $simg_obj->resizeByWidthAndCropByHeight($path_original, $size_file_path,
//                        $size_val["size_width"], $size_val["size_height"]);
//                    break;

            default:
                $simg_obj->resize_biggest_side($path_original, $size_file_path, $size_val["size_single"]);
        }
    }
}
