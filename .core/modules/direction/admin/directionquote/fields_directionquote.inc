<?php

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
        'title'		=> 'Цитата',
        'type'		=> 'textarea',
        'required'	=> true,
        'browse'	=> true,
        'view'		=> true,
        'rows'		=> 3,
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

    'is_special' => array(
        'title' => 'Специальная цитата',
        'type' => 'checkbox',
        'view' => true,
        'browse' => true
    ),
);