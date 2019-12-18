<?php
use Ekv\Misc\SkinRoot;

return array(
    'id_row' => array(
        'title' => 'Id',
        'PK' => true,
        'browse' => false,
        'view' => false,
    ),

    'name' => array(
        'title' => 'Назва',
        'type' => 'text',
        //'required' => true,
        'browse' => true,
        'view' => true,
        'width' => '50%',
    ),


    'skin_type' => array(
        'title' => 'Тип теми',
        'type' => 'select',
        'options' => array(
            '' => DEFAULT_SELECT_NAME,
            SkinRoot::TYPE_HOLIDAY => 'свято',
            SkinRoot::TYPE_MOURNING => 'день жалоби',
        ),
        'required' => true,
        'browse' => true,
        'view' => true,
    ),

    'dt_begin' => array(
        'title' => 'дата початок',
        'browse' => true,
    ),

    'dt_end' => array(
        'title' => 'дата закінчення',
        'browse' => true,
        //'view' => true,
        //"type" => "datetime",
    ),

    'man_dt_begin' => array(
        'title' => 'час початку',
        'view' => true,
        'required' => true,
        "calculable" => "+++",
        "manual_field" => "common:manual_fields/jq_datetime_picker/item.tpl", // for edit view
        "align" => "center",
        "readonly" => true,
        //'required' => true,
        'show_reset_btn' => true
    ),

    'man_dt_end' => array(
        'title' => 'час закінчення',
        'required' => true,
        'view' => true,
        "calculable" => "+++",
        "manual_field" => "common:manual_fields/jq_datetime_picker/item.tpl", // for edit view
        "align" => "center",
        "readonly" => true,
        'show_reset_btn' => true
    ),

    'enabled' => array(
        'title' => 'Активний',
        'type' => 'checkbox',
        'browse' => true,
        'view' => true,
        'init_status' => 'checked'
    ),

);