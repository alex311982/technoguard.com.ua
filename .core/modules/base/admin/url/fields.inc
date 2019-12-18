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
					'required'	=> false,
					'browse'	=> false,
					'view'		=> true,
					),
		'url' => array(
					'title'		=> 'Url',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'href'		=> 'edit?id=<!--{$result.records.$id.id.value}-->'
					),
		'module' => array(
					'title'		=> 'Module',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
//					'_table'	=> 'base_module',
//					'_ids'		=> 'id',
//					'_values'	=> 'name',
					),
		'entity' => array(
					'title'		=> 'Entity',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					),
		'action' => array(
					'title'		=> 'Action',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					),
	);

?>