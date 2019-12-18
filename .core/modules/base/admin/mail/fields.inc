<?php

	return array(
		'id'	=> array(
					'title'		=> 'Id',
					'browse'	=> false,
					'view'		=> false,
					),
		'key' => array(
					'title'		=> 'Key',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'width'		=> '25%',
					'href'		=> 'edit?id=<!--{$result.records.$id.id.value}-->',
					),
		'subject' => array(
					'title'		=> 'Subject',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'width'		=> '25%',
					
					),
		'from' => array(
					'title'		=> 'From',
					'type'		=> 'text',
					'browse'	=> true,
					'view'		=> true,
					'width'		=> '25%',
					
					),

		'text' => array(
					'title'		=> 'Text',
					'type'		=> 'textarea',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'rows'		=> 15,
					),
		'has_html' => array(
					'title'		=> 'html содержимое?',
					'type'		=> 'checkbox',
					//'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					),
	);
?>
