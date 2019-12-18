<?php

return array(
    'news_id' => array(
        'PK' => true,
        'title' => 'newsID',
    ),

    'calc_news_id' => array(
        'title' => 'newsID',
        'browse' => true,
        'calculable' => '<!--{$result.records.$id.news_id.value}-->'
    ),

    'man_translations' => array(
        'title' => '***',
        'calculable' => true,
        'browse' => true,
        'view' => true,
        "manual_field" => "common:manual_fields/news/man_transl.tpl",
        'data' => array()
    ),

);
