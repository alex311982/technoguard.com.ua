<?php

/**
 * ============================================================
 * File name: xxx.xxx
 * Date: xx/xx/2010
 * Author: Yegor Konoval
 * Url:
 * Purpose:
 * ============================================================
 */
class imgResizeSettings
{
    // !!using!! imgResizeSettings::getSettingsItem("posterImg");
    public static function getSettingsItem($key, $path_root_prefix_param = "PATH_IMAGE")
    {
        //--- path root prefix definition ---//
        $path_root_prefix = PATH_IMAGE;
        $path_root_prefix_relative = "/images";
        if ($path_root_prefix_param == "PATH_FILES") {
            $path_root_prefix = PATH_FILES;
            $path_root_prefix_relative = "/files";
        }

        $res = self::$$key;
        $res["base_photo_path_relative"] = $path_root_prefix_relative . $res["base_photo_path"];
        $res["base_photo_path"] = $path_root_prefix . $res["base_photo_path"];
        return $res;
    }

    public static function getFullSizesList($key, $filename, $type = "absolute")
    {
        $pathes = array();
        $res = self::getSettingsItem($key);
        if (!empty($res)) {
            $base_path = $res["base_photo_path"];
            if ($type == "relative") {
                $base_path = $res["base_photo_path_relative"];
            }

            foreach ($res["size_settings"] as $val) {
                $pathes[$val["size_dir"]] = $base_path . $val["size_dir"] . "/" . $filename;
            }
            $pathes['original'] = $base_path . "original/" . $filename;
        }
        return $pathes;
    }

    //#------------------- settings below -------------------#//
    static public $galleryHeaderPhoto =
        array(
            "base_photo_path" => "/galleries/header_photos/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "size1",
                        "size_single" => 150,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_biggest_side",
                    ),
                ),
        );

    static public $galleryPhotos =
        array(
            "base_photo_path" => "/galleries/gallery_photos/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "big",
                        "size_single" => 1200,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_biggest_side",
                    ),
                    array(
                        "size_dir" => "size1",
                        "size_single" => 115,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_biggest_side",
                    ),
                ),
        );

    static public $newsPhotos =
        array(
            "base_photo_path" => "/news/",
            "size_settings" =>
                array(
                    0 => array(
                        "size_dir" => "size1",
                        "size_single" => 250,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    )
                ),
        );

    static public $statPagePhotos =
        array(
            "base_photo_path" => "/stat-page/",
            "size_settings" =>
                array(
                    0 => array(
                        "size_dir" => "size1",
                        "size_single" => 140,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_biggest_side",
                    ),
                    1 => array(
                        "size_dir" => "size2",
                        "size_single" => 230,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_biggest_side",
                    ),
                    2 => array(
                        "size_dir" => "big",
                        "size_single" => 600,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_biggest_side",
                    ),
                ),
        );

    static public $progPhotos =
        array(
            "base_photo_path" => "/bank/prog/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "size1",
                        "size_single" => 115,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                    array(
                        "size_dir" => "size2",
                        "size_single" => 230,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                    array(
                        "size_dir" => "big",
                        "size_single" => 1024,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                ),
        );

    static public $gridItemPhotos =
        array(
            "base_photo_path" => "/grid/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "size1",
                        "size_single" => 115,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                    array(
                        "size_dir" => "size2",
                        "size_single" => 230,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                ),
        );

    static public $presenterPhotos =
        array(
            "base_photo_path" => "/bank/presenter/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "size1",
                        "size_single" => 170,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                    array(
                        "size_dir" => "size2",
                        "size_single" => 230,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                    array(
                        "size_dir" => "big",
                        "size_single" => 1024,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                ),
        );

    static public $slidePhotos =
        array(
            "base_photo_path" => "/slide/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "size1",
                        "size_single" => 115,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                    array(
                        "size_dir" => "size2",
                        "size_single" => 250,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                ),
        );

    static public $slidePhotosBottom =
        array(
            "base_photo_path" => "/slide-bottom/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "size1",
                        "size_single" => 169,
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                ),
        );

    static public $slidePhotosSpecialProjects =
        array(
            "base_photo_path" => "/slide-special-projs/",
            "size_settings" =>
                array(
                    array(
                        "size_dir" => "size1",
                        "size_single" => 229, // x141
                        "size_width" => 0,
                        "size_height" => 0,
                        "resize_method" => "resize_proportional_by_width",
                    ),
                ),
        );

    static public $bannerPhotos = array(
        "base_photo_path" => "/banners/",
        "size_settings" => array(
            array(
                "size_dir" => "size1",
                "size_single" => 150, // x141
                "size_width" => 0,
                "size_height" => 0,
                "resize_method" => "resize_proportional_by_width",
            ),
        ),
    );

    static public $decisionPhotos = array(
        "base_photo_path" => "/decisions/",
        "size_settings" => array(
            array(
                "size_dir" => "size1",
                "size_single" => 150, // x141
                "size_width" => 0,
                "size_height" => 0,
                "resize_method" => "resize_proportional_by_width",
            ),
        ),
    );

    static public $directionheaderPhotos = array(
        "base_photo_path" => "/directionheaders/",
        "size_settings" => array(
            array(
                "size_dir" => "size1",
                "size_single" => 150, // x141
                "size_width" => 0,
                "size_height" => 0,
                "resize_method" => "resize_proportional_by_width",
            ),
        ),
    );

}
