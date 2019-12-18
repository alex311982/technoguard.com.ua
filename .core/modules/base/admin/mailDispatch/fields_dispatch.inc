<?php

	return array(
		'id'	=> array(
					'title'		=> 'Id',
					'browse'	=> false,
					'view'		=> false,
					),
		'subject' => array(
					'title'		=> 'Тема',
					'browse'	=> true,
					'view'	=> true,
					'required' => true,
					'href'		=> 'edit.html?id=<!--{$result.records.$id.id.value}-->',
                    'truncate_length' => 150,
                ),
		'event_id' => array(
					'title'		=> 'событие',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'select',
					'options'	=> array(),
					),
		'shipment_type' => array(
					'title'		=> 'тип доставки',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'select',
					'options'	=> array(),
					),
		'test_mode' => array(
					'title'		=> 'тестовый режим',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'checkbox',
					//'options'	=> array(),
					),
        
		'body' => array(
					'title'		=> 'Тело',
					'type'	=> 'html',
					'rows'	=> '8',
					'browse'	=> true,
					'view'	=> true,
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
		'status' => array(
					'type'	=> 'select',
					'title'		=> 'Статус',
					'options'	=> array(
											"" =>	"-- все статусы --",
											"pending" =>	"ожидание",
											"sent" =>	"отослано",
											"queue_added" => "поставл. в очередь",
					),
					'browse'	=> true,
					//'view'	=> true,
					//'required' => true,
					),
					
		'file1' => array(
					'type'	=> 'file_other',
					'title'		=> 'Файл1',
					'browse'	=> true,
					'view'	=> true,
					//'required' => true,
					),
		'file2' => array(
					'type'	=> 'file_other',
					'title'		=> 'Файл2',
					'browse'	=> true,
					'view'	=> true,
					//'required' => true,
					),
		'file3' => array(
					'type'	=> 'file_other',
					'title'		=> 'Файл3',
					'browse'	=> true,
					'view'	=> true,
					//'required' => true,
					),
	);
?>