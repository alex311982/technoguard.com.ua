<?php

$tmp_setting_img_url_setting = imgResizeSettings::getSettingsItem("bannerPhotos");

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

    'text' => array(
        'title'		=> 'Текст',
        'type'		=> 'textarea',
        'required'	=> true,
        'browse'	=> true,
        'view'		=> true,
        'rows'		=> 15,
    ),

    'enabled' => array(
        'title' => 'Активний',
        'type' => 'checkbox',
        'view' => true,
        'browse' => true,
        'init_status' => 'checked'
    ),
);