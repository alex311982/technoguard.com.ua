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
					'width'		=> '200',
					'href'		=> 'edit?id=<!--{$result.records.$id.id.value}-->'
					),
		'description' => array(
					'title'		=> 'Description',
					'type'		=> 'text',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					),
		'view' => array(
					'title'		=> 'View',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'select',
					'width'		=> '120',
					'align'		=> 'center',
					'options'	=> array(
								''		=> '- Select one -',
								'frontend'	=> 'frontend',
								'backend'	=> SITE_BACKEND,
							),
					),
		'tablename' => array(
					'title'		=> "Table's name",
					'type'		=> 'text',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					),

	);

?>