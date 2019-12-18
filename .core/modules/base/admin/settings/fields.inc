<?php

	return array(
		'id'	=> array(
					'title'		=> 'Id',
					'browse'	=> false,
					'view'		=> false,
					),
		'title' => array(
					'title'		=> 'Title',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'truncate_length'		=> 250,
					'width'		=> '25%',
					'href'		=> 'edit?id=<!--{$result.records.$id.id.value}-->',
					),
		'key' => array(
					'title'		=> 'Key',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'width'		=> '25%',
					),
		'value' => array(
					'title'		=> 'Value',
					'type'	=> 'textarea',
					'rows'	=> 5,
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					),
	);
?>