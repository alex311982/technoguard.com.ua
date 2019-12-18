<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */
 	class basemailDispatch extends Entity
	{
		public $base_module_url = "/admin/base/mailDispatch";
		public $base_file_upload_path = "";
		function __construct()
		{
			parent::__construct();
			$this->base_file_upload_path = PATH_ROOT."/files/mail_dispatch/";
		}

        function ManualBeforeExecuteIncludeAction()
        {
            $page_type = $this->definePageType();
            if(!empty($page_type)){
                $this->TML->assign("toolbar_action_prefix", "{$page_type}_");
            }

            if(empty($page_type)){
                $payShipObj = EkvFactory::getPayShipGlobalObj();
                //$shipment_options = EkvRoomHelper::shipmentOptions(false);
                $shipment_options = $payShipObj->getShipmentOptions();

                //--- event options ---//
                $eventObj = new EkvRoomEvent();
                $event_options = $eventObj->getEventOptions();


                Entity::prependOptionsWithDefaultValue($shipment_options);
                Entity::prependOptionsWithDefaultValue($event_options);

                $this->DB->fields["shipment_type"]["options"] = $shipment_options;
                $this->DB->fields["event_id"]["options"] = $event_options;
            }
            else if($page_type == "queue"){
                $mailDispatchObj = new EkvMailDispatch();
                $this->DB->table = EkvMailDispatch::TABLE_DISPATCH_TASK;
                $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, 'queue_base');
                $dispatch_options = $mailDispatchObj->getMailDispatchOptions();
                $this->setSelectFieldOptions($dispatch_options, 'dispatch_id', "queue_browse");
            }
        }

        function definePageType()
        {
            $page_type = "";
            if(strpos($this->action, "queue_") !== false){
                $page_type = "queue";
            }
            return $page_type;
        }





        function canBeSendButtonDisplayed()
        {
            $return = true;
            return $return;
        }

        function canBeSendButtonDisplayedOnEditPages()
        {
            if( $this->canBeSendButtonDisplayed() ){
                $this->TML->assign("edit_CAN_BE_SEND_BUTTON_DISPLAYED", true );
            }
        }


		function SetBreadcrump()
		{
            $page_type = $this->definePageType();
			$breadcrumps = array();
			$title = "Рассылки";
			$breadcrumps []= array('title' => $title,
									'href' => 'browse');
            if($page_type == "queue"){
                $title = "Очередь рассылок";
                $breadcrumps []= array('title' => $title,
                                        'href' => 'queue_browse');
            }

			if ($this->id > 0){
				$details = $this->DB->BGetInfoById($this->DB->table, $this->id);
				$breadcrumps []= array('title' => $details['subject'],
										'href' => 'edit?id='.$this->id
								);
			}
			return $breadcrumps;
		}
		
		function UploadAttachingFiles(&$err = false)
		{
			$FILES_AR = $_FILES;
			$base_path = $this->base_file_upload_path;	
			$MAX_FILE_SIZE = 10*1024*1024;
			$return_names = array();
			//photo uploading settings
			$input_names = array(
									"1"	=>	"_input_file1",
									"2"	=>	"_input_file2",
									"3"	=>	"_input_file3",
							);			
			//$input_name = "_input_photo";
			
			$return_names = array(
									"file1"	=>	"",
									"file2"	=>	"",
									"file3"	=>	"",
							);

			//look over all files in form
            $i = 1;
			foreach ($input_names as $k=>$input_name){
			    $file_is_uploading = false;
				///************* defining file type
				$type = "unknown";
				if ( isset($FILES_AR [$input_name] ['tmp_name']) && !empty($FILES_AR [$input_name] ['tmp_name']) ){
					//$type = DefineFileType($FILES_AR [$input_name] ["type"]);
					$type = DefineFileExtension($FILES_AR [$input_name] ["name"]);
					$file_is_uploading = true;
				}else{
					//$err['photo'] = "Поле не может быть пустым";
					//return false;
				}
				// do uploading and resizing
				if ($file_is_uploading == true && $FILES_AR [$input_name]['size'] > $MAX_FILE_SIZE ){
					$err['$input_name'] = "Максимальный размер файла".($MAX_FILE_SIZE/1024)."Кб";
				}elseif($file_is_uploading == true ){
					$up = $FILES_AR[$input_name]['tmp_name'];
					## original size copy ##     
	  				$file_prefix_original = "delivery"."_".EkvCommonHelper::getSuperUniqueSuffix();
	  				$filename = "{$file_prefix_original}.$type";
					$path_new = $base_path."$filename";
					$move_res = move_uploaded_file($up, $path_new);
					// all ok  - make  preview
					if ($move_res == true){
						$key = str_replace("_input_", "", $input_name);
						// return file name with delimiter
						$return_names[$key] = $filename;
					}
				}
				$i++;
			}// end main foreach
			return $return_names;			

		}
		
		// starts sending delivery
		function performDispatch($id_delivery)
		{
			echo "<html><head><title>Dispatch result</title>
			<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body>";
			setlocale(LC_ALL, 'ru_RU.UTF-8');

			//--- get latest news ---//
			//$latest_news = $this->getLatestNewsForDelivery(5);
            BaseNews::initDb($this->DB);
            $latest_news = BaseNews::getMainPageNews(5);
            foreach($latest_news as &$nval){
                $nval["url_full"] = BaseNews::getFullUrl($nval, true);
            }
			
			//--- get delivery row ---//
			$delivery_info = $this->DB->BGetInfoById($this->DB->table, $id_delivery);
			if (empty($delivery_info)){
				echo "<h2>Delivery error.</h2>";
				exit;
			}
			
			// depending on delivery author define from address
			$from_name = DISPATCH_FROM_NAME;
			$from_email = DISPATCH_FROM_EMAIL;
	
			$subject = $delivery_info['subject'];
			//$subject = '=?koi8-r?B?'.base64_encode(convert_cyr_string($subject, "w","k")).'?=';
			$subject = '=?utf-8?B?'.base64_encode($subject).'?=';
			$message = $delivery_info['body'];
			//--- parsing headers ---//
			//$from_name = '=?koi8-r?B?'.base64_encode(convert_cyr_string($from_name, "w","k")).'?=';
			$from_name = '=?utf-8?B?'.base64_encode($from_name).'?=';
			$from = "From: $from_name <{$from_email}> \n";
			$headers = $from;
			
			if (!empty ($add_headers))
			{
				$headers.="\n".$add_headers;
			}

            $message = $this->addHostToWysiwygImages($message);
			//--- fetch email-tpl  ---//
			$tpl_email_vars = array(
							"latest_news"	=> $latest_news,
							"body"		    => $message,
						);
			$this->TML->assign("email_vars", $tpl_email_vars);
			$message = $this->TML->fetch("common:custom_templates/mail_dispatch/email.tpl");

			// creating attach params
			$attach_params = array();
			for ($i=1; $i<=3;$i++){
				$key = "file{$i}";
				//$base_path = PATH_FILES."/delivery/";
				$base_path = $this->base_file_upload_path;
				if (!empty($delivery_info[$key])){
					$attach_params[] = array(
											"full_path"	=> $base_path.$delivery_info[$key]	
										);
				}
			} // end for
							
			if ( !empty($attach_params) ){
			//if (true){
				$unique_delimiter  = strtoupper(uniqid(time()));
			    $attach_headers     = "Content-Type:multipart/mixed;"; 
			    $attach_headers     .= "boundary=\"----------".$unique_delimiter."\"\n\n";
	
			    $attach_message = "------------".$unique_delimiter."\nContent-Type:text/html;charset=utf-8\n";
				//$attach_message .= "charset=windows-1251;\n\n$message\n\n"; 
			    $attach_message .= "Content-Transfer-Encoding: 8bit\n\n$message\n\n";
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
			}else{
				$headers .= "Content-Type: text/html; charset=utf-8";
			} 

			// get mail receivers address
			$delivery_receivers = $this->DB->GetDeliveryReceivers($type="real");
			if (!empty($delivery_receivers)){
				$count_messages = 0;
				foreach ($delivery_receivers as $k=>$val){
					$count_messages++;
					$email = $val['email'];
					$message_final = $message;

					//--- replace user_name in message ---//
					$message_final = str_replace("%%USER_NAME%%", $val["name"], $message);
                    
					//--- UBSUBSCRIBE_URL ---//
					$unsubscribe_link = "http://".SITE_HOST."/mail-dispatch/unsubscribe.html?hash={$val["hash"]}";
					$message_final = str_replace("%%UBSUBSCRIBE_URL%%", $unsubscribe_link, $message_final);
					
//					$email = "avdoshyn@gmail.com";
					mail ($email, $subject, $message_final, $headers);
					echo "email: $email <br />";
					@ob_flush();
					flush();
					// show delimiter
					if ( $count_messages % 50 == 0 ){
						echo "<h2>======================| $count_messages |=============================</h2>";
						@ob_flush();
						flush();
					}
    				if($count_messages % 200 == 0){
						//break;
					}
				}
				// set delivery status to sent
				$sql = "
					UPDATE `{$this->DB->table}`
					SET 
						`status` = 'sent'
					WHERE
						`id` = '$id_delivery'
				";
				$this->DB->query($sql);
			}
		}

        function addHostToWysiwygImages($body)
        {
            return str_replace("/images/ckeditor/",
                "http://".SITE_HOST."/images/ckeditor/",
                $body);
        }

	}

?>
