<?php

	return array(
		'id'	=> array(
					'title'		=> 'Id',
					'browse'	=> false,
					'view'		=> false,
					),
		'title' => array(
					'title'		=> 'Описание настройки',
					'type'		=> 'text',
					//'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'width'		=> '50%',
					'href'		=> 'edit?id=<!--{$result.records.$id.id.value}-->',
                    'truncate_length' => '200'
					),
		'key' => array(
					'title'		=> 'Key',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					),
		'text' => array(
					'title'		=> 'Текст',
					'type'		=> 'textarea',
					'required'	=> false,
					'browse'	=> false,
					'view'		=> true,
					'rows'		=> 15,
					),
	);

?>