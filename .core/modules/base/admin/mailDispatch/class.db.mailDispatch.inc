<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2008
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */
  	ini_set("MAX_EXECUTION_TIME", 0);
	class DbbasemailDispatch  extends DB
	{
		public $table_receivers = "base_mail_dispatch_receivers";
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_mail_dispatch';
			$this->fields = $this->IncludeSpecificFields(__FILE__, "dispatch");
		}		
	
		function GetDeliveryReceivers($dispatch_type="real")
		{
			$res = array();
			if ($dispatch_type == "real"){
				$sql = "
					SELECT id, email, name, hash
					FROM `{$this->table_receivers}`
					WHERE 1=1
					ORDER BY
						`date` ASC
				";
				$res = $this->RunSQL($sql, true);
			}
			
			//--- TEST SEND -> addressees are taken from site.conf BACKEND_MAIL_DISPATCH_RECEIVERS ---//
//			elseif ($dispatch_type == "test"){
//				$res = array();
//				if( defined("BACKEND_GENERICS_MAIL_DELIVERY_RECEIVERS") ){
//					$explode_ar = explode(",", BACKEND_GENERICS_MAIL_DELIVERY_RECEIVERS);
//					if(!empty($explode_ar)){
//						$i = 0;
//						foreach($explode_ar as $exval){
//							$i++;
//							$res[] = array(
//										"id" => $i,
//										"login" => "test-login [no login]",
//										"email"	=> $exval,
//										"real_name" => "real_name",
//										"user_hash" => "XXX_test_hash_XXX"
//									);
//						}
//					}
//				}
//				
//			}
			return $res;
		}
		
		function CorrectDeliveryDelete($id_delivery)
		{
			$files_frame = array(
									"file1"	=>	"",
									"file2"	=>	"",
									"file3"	=>	"",
							);
			$delivery_info = $this->BGetInfoById("base_mail_dispatch", $id_delivery);
			if ( !empty($delivery_info) ){
				//delete files first
				foreach ($files_frame as $k=>$val){
					if (!empty($delivery_info[$k])){
						$path = PATH_FILES."/mail_dispatch/".$delivery_info[$k];
						//echo "<h2>path = $path</h2>";
						unlink($path);
					}
				}
				// row delete
				$sql = "DELETE FROM `base_mail_dispatch`
							WHERE `id` = '$id_delivery'
				";
				$this->query($sql);
			}
		}
	}

?>
