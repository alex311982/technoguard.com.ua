<?php

	return array(
		'id'	=> array(
					'title'		=> 'ID',
					'browse'	=> false,
					'view'		=> false,
                    "send_email" => true,
					),
		'fio' => array(
					'title'		=> 'ФИО',
					'required'	=> true,
                    "send_email" => true,
					),
		'password' => array(
					'title'		=> 'Пароль',
					'required'	=> true,
					),
		'email' => array(
					'title'		=> 'E-mail',
					'required'	=> true,
                    "send_email" => true,
					),
		'phone' => array(
					'title'		=> 'Телефон',
					'required'	=> true,
                    "send_email" => true,
					),

        'user_country_id' =>
            array(
                'title' => "страна",
                "send_email" => true,
            ),
        'user_city_id' => array(),
        'user_city' =>
            array(
                'title' => "город",
                "send_email" => true,
            ),


		'reg_date' => array(
					'title'		=> 'дата',
					//'required'	=> true,
                    "send_email" => true,
					),
		'hash' => array(
					'title'		=> 'hash',
					//'required'	=> true,
					),
		'captcha' => array(
					'title'		=> 'Защитный код',
					'required'	=> true,
					"calculable" => 1
					),
		'enabled' => array(
					'title'		=> 'Активен',
					'browse'	=> true,
					'view'		=> true,
					'type'		=> 'select',
                    //"send_email" => true,
					),
	);

?>
