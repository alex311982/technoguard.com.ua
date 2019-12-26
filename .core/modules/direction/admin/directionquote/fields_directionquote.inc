<?php

return array(
    'id' => array(
        'title' => 'Id',
        'browse' => false,
        'view' => false,
        'PK' => true
    ),

    'text' => array(
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
            'retail' => 'Ритейл', 'agroprom' => 'Агропром', 'RFID' => 'RFID'
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
);