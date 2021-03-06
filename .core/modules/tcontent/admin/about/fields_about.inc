<?php

return array(
    'id' => array(
        'title' => 'Id',
        'browse' => false,
        'view' => false,
        'PK' => true
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

    'admin_short_name' => array(
        'title' => 'Заголовок',
        'type' => 'text',
        'required' => true,
        'browse' => true,
        'view' => true,
    ),

    'text' => array(
        'title' => 'Полный текст',
        'type' => 'html',
        'required' => false,
        //'browse' => true,
        'view' => true,
    ),

    'enabled' => array(
        'title' => 'Активний',
        'type' => 'checkbox',
        'view' => true,
        'browse' => true,
        'init_status' => 'checked'
    ),
);