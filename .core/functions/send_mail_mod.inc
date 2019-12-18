<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */

	/**
	 * send mail function
	 * @param	(string)$templateId -> template Id from backend "mail templates"
	 * @param	(string)$email -> E-mail to send
	 * @param	(array)$anchor -> replacement keys. %key% replace to value
	 * @param	(string)$add_headers -> additional mail headers
	 * @return	(bool) -> mail sended?
	 */
	function sendmail_mod ($templateId, $email, $anchor, $add_headers="")
	{
		$entity=load_entity_class('base','mail',SITE_BACKEND);
		$result=$entity->DB->simply_get_record($templateId,'key');

		$subject='=?koi8-r?B?'.base64_encode(convert_cyr_string($result['subject'], "w","k")).'?=';
		$message=$result['text'];
		$from   =$result['from'];
		// if filed "from" is empty then load "email_from" value from base site settings
		if(empty($from) && defined('EMAIL_FROM'))
		{
			$from=constant('EMAIL_FROM');
		}
		// parsing headers
		if(!empty($from)){$from='From:'.uri_to_translit($from);}
		$headers=
			"MIME-Versin: 1.0\r\n" .
			"Content-type: text/html; charset=windows-1251\r\n" .
			"Content-Transfer-Encoding: 8bit\r\n".
			$from."\r\n".
//			"Subject: ".$subject."\r\n".
			$add_headers;
		$m=array();
		preg_match_all("/%([\s\S]+)%/iU",$message,$m);
		if(!empty($m) && !empty($anchor))
		{
			foreach($anchor as $k=>$v)
			{
				$anchor[strtoupper($k)]=$v;
			}

			foreach($m[1] as $v)
			{
				$tpl_v=$v;
				$v=strtoupper($v);
				if(empty($anchor[$v]) && !defined($v))
				{
					paw('sendmail::anchor_name='.$v.'; ERR_NO_ANCHOR');
					$anchor[$v]='';
				}
				elseif(defined($v))
				{
					$anchor[$v]=constant($v);
				}
				$message=str_replace("%".$tpl_v."%",$anchor[$v],$message);
			}
		}
		//pa($message);
		return mail($email, $subject, $message, $headers);
	}

?>
