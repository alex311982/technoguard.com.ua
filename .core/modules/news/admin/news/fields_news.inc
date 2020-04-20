<?php
$tmp_setting_img_url_setting = imgResizeSettings::getSettingsItem("newsPhotos");

return array(
    'id' => array(
        'PK' => true,
        'title' => 'Id',
        'browse' => false,
        'view' => false,
    ),

    'lang' => array(
        'title' => 'Язык',
        'type' => 'select',
        'options' => array(
            '' => DEFAULT_SELECT_NAME, 'ru' => 'Русский', 'ua' => 'Украинский',
        ),
        'required' => true,
        'browse' => true,
        'view' => true,
    ),

    'news_title' => array(
        'title' => 'Заголовок',
        'type' => 'text',
        'required' => true,
        'browse' => true,
        'view' => true,
    ),

    'direction_type' => array(
        'title' => 'Направление',
        'type' => 'select',
        'options' => array(
            '' => DEFAULT_SELECT_NAME, 'retail' => 'Ритейл', 'agroprom' => 'Агропром', 'RFID' => 'RFID'
        ),
        'required' => true,
        'browse' => true,
        'view' => true,
    ),

    'enabled' => array(
        'title' => 'Активний',
        'browse' => true,
//        'view' => true,
        'init_status' => true,
        "type" => "checkbox",
    ),

     'photo_file' => array(
        'title' => 'Фото',

        'view' => true,
        'type' => "file",
        "size_name" => "size1",
        "rewrite_img_base" =>
        $tmp_setting_img_url_setting["base_photo_path_relative"],
        //"notshow_check_delete" => true,
        "show_delete_checkbox" => true,
        "direct_image" => true,
        "width" => '120px',
        "align" => "center",
    ),

    'text_long' => array(
        'title' => 'Полный текст',
        'type' => 'html',
        'required' => false,
        //'browse' => true,
        'view' => true,
    ),

);
