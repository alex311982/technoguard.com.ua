<?php

	return array(
		'id_task'	=> array(
					'title'		=> 'Id',
                    'PK' => true,
					'browse'	=> false,
					'view'		=> false,
					),

		'dispatch_id' => array(
					'title'		=> 'событие',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'select',
					'options'	=> array(),
                    //'href'		=> 'edit.html?id=<!--{$result.records.$id.id_task.value}-->',
					),
		'calc_extra' => array(
					'title'		=> 'адресаты',
                    'calculable' => '[адресаты]',
					'browse'	=> true,
					'view'		=> true,
                    'href'		=> 'q_extra_browse.html?id=<!--{$result.records.$id.id_task.value}-->',
					),
		'in_progress' => array(
					'title'		=> 'in_progress',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'checkbox',
					//'options'	=> array(),
					),
		'is_active' => array(
					'title'		=> 'is_active',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'checkbox',
					//'options'	=> array(),
					),
		'is_finished' => array(
					'title'		=> 'закончен',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'checkbox',
					//'options'	=> array(),
					),
					
		'date_creation' => array(
					'title'		=> 'Дата',
					'type'	=> 'date',
					'browse'	=> true,
					'dateformat'	=>	BACKEND_DATE_FORMAT,
					//'view'	=> true,
					//'required' => true
					),
	);
?>