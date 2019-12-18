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
					),
        'is_restricted' => array(
					'title'		=> 'is_restricted',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					),
		'url' => array(
					'title'		=> 'Url',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					),
		'body' => array(
					'title'		=> 'Body',
					'type'		=> 'textarea',
					'required'	=> false,
					'browse'	=> false,
					'view'		=> true,
					'rows'		=> 10,
					),
		'description' => array(
					'title'		=> 'Description',
					'type'		=> 'textarea',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					'rows'		=> 3,
					),
		'keywords' => array(
					'title'		=> 'Keywords',
					'type'		=> 'textarea',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					'rows'		=> 3,
					),

	);

?>