<?php

$tmp_setting_img_url_setting = imgResizeSettings::getSettingsItem("decisionPhotos");

return array(
    'id' => array(
        'title' => 'Id',
        'browse' => false,
        'view' => false,
        'PK' => true
    ),

    'code' => array(
        'title' => 'Код',
        'type' => 'text',
        'browse' => true,
        'view' => true,
        'required' => true,
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

    'title' => array(
        'title' => 'Название',
        'type' => 'text',
        //'required' => true,
        'browse' => true,
        'view' => true,
    ),

    'text' => array(
        'title' => 'Краткий текст',
        'type' => 'textarea',
        'rows'	=> 5,
        'required' => false,
        'view' => true,
    ),

    'long_text' => array(
        'title' => 'Полный текст',
        'type' => 'html',
        'required' => false,
        //'browse' => true,
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
        'type' => 'checkbox',
        'view' => true,
        'browse' => true,
        'init_status' => 'checked'
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

    'sort_order' => array(
        'title' => 'Порядок',
        "sort_order" => true,
        'browse' => true,
        "width" => "60px",
        "moving_item_id" => '<!--{$result.records.$id.id.value}-->',
        "extra_params_str" => ''
    ),

);