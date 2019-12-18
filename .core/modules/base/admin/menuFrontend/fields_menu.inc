<?php
return array(
    'id' => array(
        'title' => 'Id',
        'browse' => false,
        'view' => false,
    ),

    'lang_id' => array(
        'title' => 'langID',
    ),
    'menu_type' => array(
        'title' => 'menuType',
    ),

    'title' => array(
        'title' => 'Назва',
        'type' => 'text',
        'required' => true,
        'browse' => true,
        'view' => true,
        'width' => '50%',
        'href' => 'browse?id_parent=<!--{$result.records.$id.id.value}-->&id_top=' . @$_REQUEST["id_top"]
    ),

    'hint' => array(
        'title' => 'Підказка',
        'type' => 'text',
        //'required'	=> true,
        'browse' => true,
        'view' => true,
    ),
    'url' => array(
        'title' => 'Url',
        'type' => 'text',
        //'required'	=> true,
        'browse' => true,
        'view' => true,
    ),

    'enabled' => array(
        'title' => 'Активний',
        'type' => 'checkbox',
        'view' => true,
        'init_status' => 'checked'
    ),

    'text_id' => array(
        'title' => 'text_id',
        'type' => 'text',
    ),

    'parent_id' => array(
        'title' => 'Знаходиться в',
        'type' => 'select',
        'required' => false,
        'view' => false,
        "options" => array()
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

?>