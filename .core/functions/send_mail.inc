<?php
/*
 * ============================================================
 * File name: send_mail.inc
 *      Date: 26/12/2006
 *   Purpose:
 * ============================================================
 */

	/**
	 * send mail function
	 * @param	(string)$templateId -> template Id from backend "mail templates"
	 * @param	(string)$email -> E-mail to send
	 * @param	(array)$anchor -> replacement keys. %key% replace to value
	 * @param	(string)$add_headers -> additional mail headers
	 * @param	(array)$attach_params -> additional attachment params
	 			[0]
				 	full_path	 
	 * @return	(bool) -> mail sended?
	 */
	function sendmail($templateId, $email, $anchor, $add_headers="", $attach_params = array() )
	{
        $CONTENT_TYPE = "text/plain";
        //$CONTENT_TYPE = "text/html";
		// get fields from template ID
		$entity = load_entity_class('base', 'mail');
		$result = $entity->DB->RetrieveRecord($templateId, 'key', '');

        if( $result["record"]["has_html"]["value"] == 1 ){
            $CONTENT_TYPE = "text/html";
        }

		$subject = $result ['record']['subject']['value'];
		$subject = '=?utf-8?B?'.base64_encode($subject).'?=';
		$message = $result ['record']['text']['value'];
		//$message = convert_cyr_string($message, "w","k");
		$from    = $result ['record']['from']['value'];
		// if filed "from" is empty then load "email_from" value from base site settings
		if (empty ($from))
		{
			$entity->DB->fields = array (
					'id'	=> array(),
					'title' => array(),
					'key' => array(),
					'value' => array(),
					);
			$result = $entity->DB->RetrieveRecord('email_from', 'key', 'base_settings');
			$from    = $result ['record']['value']['value'];
		}
		// parsing headers
		if (!empty ($from))
		{
			//$from = '=?koi8-r?B?'.base64_encode(convert_cyr_string($from, "w","k")).'?=';
			$from = '=?utf-8?B?'.base64_encode($from).'?=';
			$from = 'From: '.$from."<".SITE_MAIL_ADDRESS.">\n";
			//--- if empty atach params set CORRECT message encoding here ---//
			if (empty($attach_params)){
				$from .= "Content-Type: {$CONTENT_TYPE}; charset=utf-8";
			}
		}
		$headers = $from;
		if (!empty ($add_headers))
		{
			$headers.="\n".$add_headers;
		}
		
		// attaching file
		if ( !empty($attach_params) ){
			$unique_delimiter  = strtoupper(uniqid(time()));
		    $attach_headers     = "Content-Type:multipart/mixed;"; 
		    $attach_headers     .= "boundary=\"----------".$unique_delimiter."\"\n\n";

		    $attach_message = "------------".$unique_delimiter."\nContent-Type:{$CONTENT_TYPE}; charset=utf-8\n"; 
		    $attach_message .= "Content-Transfer-Encoding: 8bit\n\n$message\n\n";
			//$attach_message .= "charset=windows-1251;\n\n$message\n\n";
		    //$attach_message .= "------------".$unique_delimiter."\n"; 

			foreach($attach_params as $val){
				$full_path = $val['full_path'];
				$mime_type = GetMimeTypeByExtension($full_path);
				$file_handler = fopen($full_path, "rb"); 
			    $attach_message      .= "------------".$unique_delimiter."\n"; 
				$attach_message      .= "Content-type: $mime_type;";
			    $attach_message      .= "name=\"".basename($full_path)."\"\n"; 
			    $attach_message      .= "Content-Transfer-Encoding:base64\n"; 
			    $attach_message      .= "Content-Disposition:attachment;"; 
			    $attach_message      .= "filename=\"".basename($full_path)."\"\n\n"; 
			    $attach_message      .= chunk_split(base64_encode(fread($file_handler, filesize($full_path))))."\n";
			}
			//$attach_headers .= "------------".$unique_delimiter."\n";
			$headers .= $attach_headers;
			$message = $attach_message;
		}
		

		// replace message
		foreach ($anchor as $key => $value)
		{
			$message = str_replace("%$key%", $value, $message);
		}
		//echo "<h2> EMAIL=$email SUBJ=$subject MESSAGE = $message </h2>"; exit;
		return mail ($email, $subject, $message, $headers);
	}
	
	
	/**
	 * send mail dynamic - flexible function
	 * @param	(string)$templateId -> template Id from backend "mail templates"
	 * @param	(string)$params-> array
	 			receiver
	 			subject
	 			from_name
	 			from_email
	 * @param	(array)$anchor -> replacement keys. %key% replace to value
	 * @param	(string)$add_headers -> additional mail headers
	 * @return	(bool) -> mail sended?
	 */
	function sendmail_dynamic($templateId, $params, $anchor, $add_headers="")
	{
		// get fields from template ID
		$entity = load_entity_class('base', 'mail');
		$result = $entity->DB->RetrieveRecord($templateId, 'key', '');

		// check params->subject
		$subject = $result ['record']['subject']['value'];
		if (@!empty($params['subject'])){
			$subject = $params['subject'];
		}
		$subject = '=?koi8-r?B?'.base64_encode(convert_cyr_string($subject, "w","k")).'?=';
		$message = $result ['record']['text']['value'];
		//$message = convert_cyr_string($message, "w","k");

		$from_name = $result ['record']['from']['value'];		
		// check up for params -> from_name
		if (@!empty($params['from_name'])){
			$from_name = $params['from_name'];
		}
		// if filed "from" is empty then load "email_from" value from base site settings
		if (empty ($from_name))
		{
			$entity->DB->fields = array (
					'id'	=> array(),
					'title' => array(),
					'key' => array(),
					'value' => array(),
					);
			$result = $entity->DB->RetrieveRecord('email_from', 'key', 'base_settings');
			$from_name    = $result ['record']['value']['value'];
		}
		// parsing headers
		if (!empty ($from_name))
		{
			$from_name = '=?koi8-r?B?'.base64_encode(convert_cyr_string($from_name, "w","k")).'?=';
			$from_email = SITE_MAIL_ADDRESS;
			if (!@empty($params['from_email'])){
				$from_email = $params['from_email'];
			}
			$from = 'From: '.$from_name."<".$from_email.">\n";
			$from .= "Content-Type: text/plain; charset=utf-8";
		}
		$headers = $from;
		if (!empty ($add_headers))
		{
			$headers.="\n".$add_headers;
		}

		// replace message
		foreach ($anchor as $key => $value)
		{
			$message = str_replace("%$key%", $value, $message);
		}
		//echo "<h2> EMAIL=$email SUBJ=$subject MESSAGE = $message </h2>"; exit;
		return mail ($params['receiver'], $subject, $message, $headers);
	}
	
	function FuncCorrectEmail($email)
	{
		if(!eregi("^[\._a-zA-Z0-9-]+@[\._a-zA-Z0-9-]+\.[a-z]{2,6}$", $email)){
			return false;
		}
		return true;
	}	

?>
