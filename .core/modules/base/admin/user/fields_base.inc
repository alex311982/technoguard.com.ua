<?php
	return array(
		'id'	=> array(
					'title'		=> 'Id',   
					'PK'		=> true,   
					'browse'	=> false,
					'view'		=> false,
					),
		'calc_uid'	=> array(
					'title'		=> 'ID',
					'browse'	=> true,
					//'view'	=> true,
					'readonly'	=> true,
					'calculable'	=> '<!--{$result.records.$id.id.value}-->',
                    //'href'		=> 'base_edit.html?id=<!--{$result.records.$id.id.value}-->',
					),
		'fio' => array(
					'title'		=> 'ФИО', 
					'browse'	=> true,
					'view'	=> true,
					'required' => true,
					'href'		=> 'base_edit.html?id=<!--{$result.records.$id.id.value}-->',
                ),
		'email' => array(
					'title'		=> 'email',
					'browse'	=> true,
					'view'	=> true,
					'required' => true,
					'href'		=> 'base_edit.html?id=<!--{$result.records.$id.id.value}-->',
                ),
		'phone' => array(
					'title'		=> 'тел',
					'browse'	=> true,
					'view'	=> true,
					//'required' => true,
                ),

        'user_country_id' =>
            array(
                'title'		=> 'Страна',
                'browse'	=> true,
                'view'	=> true,
                'type'	=> "select",
            ),
        'user_city_id' =>
            array(
                'title'		=> 'город по Украине',
                'browse'	=> true,
                'view'	=> true,
                'type'	=> "select",
            ),
		'user_city' => array(
					'title'		=> 'город клиента',
					'browse'	=> true,
					'view'	=> true,
                ),

        'dispatch_subscriber' => array(
      					'title'		=> 'email рассылка',
      					'browse'	=> true,
      					'view'		=> true,
      					'type'		=> 'checkbox',
      					),
        'sms_dispatch_subscriber' => array(
      					'title'		=> 'sms рассылка',
      					'browse'	=> true,
      					'view'		=> true,
      					'type'		=> 'checkbox',
      					),

		'enabled' => array(
					'title'		=> 'активен',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'checkbox',
					),
		'calc_pwd_change'	=> array(
					'title'		=> 'сменить пароль',
					'browse'	=> true,
					//'view'	=> true,
					'readonly'	=> true,
					'width'	=> "120px",
					'calculable'	=> '[сменить пароль]',
                    'href'		=> 'base_pwd_change.html?id=<!--{$result.records.$id.id.value}-->',
					),
		'calc_user_orders'	=> array(
					'title'		=> 'заказы',
					'browse'	=> true,
					//'view'	=> true,
					'readonly'	=> true,
					'width'	=> "70px",
					'align'	=> "center",
					'calculable'	=> '[заказы]',
                    'href'		=> '/admin/scheme/orders/base_browse.html?forUserId=<!--{$result.records.$id.id.value}-->',
					),

		'reg_date'	=> array(
					'title'		=> 'Дата создания',
					'browse'	=> true,
					//'view'		=> true,
					'readonly'	=> true
					),
	);
?>