<?php
	return array(
		'id'	=> array(
					'title'		=> 'Id',
					'browse'	=> false,
					'view'		=> false,
					),
		'parent_id'	=> array(
					'title'		=> 'Parent Id',
					'browse'	=> false,
					'view'		=> false,
					),
		'title' => array(
					'title'		=> 'Title',
					'type'		=> 'text',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'href'		=> 'browse?ParentId=<!--{$result.records.$id.id.value}-->'
					),
		'module' => array(
					'title'		=> 'Module',
					'type'		=> 'text',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					//'width'		=> '25%',
					),
		'entity' => array(
					'title'		=> 'Entity',
					'type'		=> 'text',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					//'width'		=> '25%',
					),
		'action' => array(
					'title'		=> 'Action',
					'type'		=> 'text',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					//'width'		=> '25%',
					),
		'order_no' => array(
					'title'		=> 'Order no',
					'type'		=> 'text',
					'required'	=> false,
					'browse'	=> true,
					'view'		=> true,
					),
		'is_active' => array(
					'title'		=> 'Активен',
					'type'		=> 'checkbox',
					'view'		=> true,
					'browse'	=> true,
                    'init_status' => 'checked',

                ), 
/*
    'super_admin' => array(
					'title'		=> 'Super Admin Menu',
					'type'		=> 'select',
					'required'	=> true,
					'browse'	=> true,
					'view'		=> true,
					'options' => array(
              'yes'=>'yes',
              'no'=>'no',
            ),
					),
*/
	);

?>
