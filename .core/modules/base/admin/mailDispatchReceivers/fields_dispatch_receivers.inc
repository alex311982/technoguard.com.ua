<?php

	return array(
		'id'	=> array(
					'title'		=> 'Id',
					'browse'	=> false,
					'view'		=> false,
					),
		'email' => array(
					'title'		=> 'Email',
					'browse'	=> true,
					'view'	=> true,
					'required' => true,
					'href'		=> 'edit.html?id=<!--{$result.records.$id.id.value}-->',
					),
		'name' => array(
					'title'		=> 'Имя',
					'browse'	=> true,
					'view'	=> true,
					//'required' => true,
                ),
		'organisation' => array(
					'title'		=> 'Организация',
					'browse'	=> true,
					'view'	=> true,
					//'required' => true,
                ),
		'hash' => array(
					'title'		=> 'hash',
					//'browse'	=> true,
					//'view'	=> true,
					'required' => true,
					),
		'date' => array(
					'title'		=> 'Дата',
					'type'	=> 'date',
					'browse'	=> true,
					'dateformat'	=>	BACKEND_DATE_FORMAT,
					//'view'	=> true,
					//'required' => true
					),
	);
?>