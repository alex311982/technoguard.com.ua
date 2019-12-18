<?php
/**
 * @deprecated - use EntityImageUpload instead
 */
class ManageUpload extends DB{
	var $base_photo_path;
	var $input_name = "_input_photo"; // can be str like "_input_photo" or array like array("_input_photo", "_input_photo1")
	var $max_file_size =  15242880;//5*1024*1024;
	var $file_prefix_original = "fname";
	var $size_settings = array(
							0 => array(
									"size_dir" => "size1",
									"size_single"	=> 0,
									"size_weight"	=> 0,
									"size_height"	=> 0,
									"resize_method" => "resize_biggest_side"
								),
						);
	function __construct($base_photo_path, $input_name = "_input_photo", $file_prefix_original = "fname"  )
	{
		//$this->base_photo_path = PATH_ROOT."/".$base_photo_path;
		$this->base_photo_path = $base_photo_path;
		$this->input_name = $input_name;
		$this->file_prefix_original = $file_prefix_original;
	}
	
	
	//--- logo upload ---//
	function UploadAndResizeSingleImage(&$err = array(), $return_single_photoname = false)
	{
		$FILES_AR = $_FILES;
		$base_path = $this->base_photo_path;
		$MAX_FILE_SIZE = $this->max_file_size;
		$return_names = array();
		//photo uploading settings
		$file_is_uploading = false;
		$input_names = (is_array($this->input_name))? $this->input_name : array("{$this->input_name}");			
		//$input_name = "_input_photo";
		$ret_name_key = str_replace("_input_", "", $this->input_name);
		$return_names = array(
								//"{$ret_name_key}"	=>	"",
						);

		//look over all files in form
		$input_number = 0;  // will be added to file name to save it unique
		foreach ($input_names as $k=>$input_name){
			$input_number++;
			$file_is_uploading = false;
			///************* defining file type
			$type = "unknown";
			if (!empty($FILES_AR [$input_name] ['tmp_name']) ){
				// swfuploader type defining by extension
				if ( isset($FILES_AR[$input_name]['define_type_by_extension']) ){
					$tmp_parts = pathinfo($FILES_AR[$input_name]['name']);
					$type = $tmp_parts['extension'];
				}else{
					$type = DefineFileType($FILES_AR [$input_name] ["type"]);
				}
				$file_is_uploading = true;
			}else{
				//$err['photo'] = "Поле не может быть пустым";
				//return false;
			}
			// do uploading and resizing
			if ($file_is_uploading == true && $FILES_AR [$input_name]['size'] > $MAX_FILE_SIZE ){
				$err["$input_name"] = "Максимальный размер файла".($MAX_FILE_SIZE/1024)."Кб";
			}elseif($file_is_uploading == true && $type == "unknown"){
				$err["$input_name"] = "Некорректный тип файла";
			}
			elseif($file_is_uploading == true ){
				$up = $FILES_AR[$input_name]['tmp_name'];
				//---## original size copy ##---//
				$dir = "original";
				//$file_prefix_original = "{$this->file_prefix_original}"."_".time()."_{$input_number}";
				$file_prefix_original = "{$this->file_prefix_original}"."_".uniqid(time()."_");
  				$filename = "{$file_prefix_original}.$type";
				$path_new = $base_path."{$dir}/$filename";

				$path_new_dirname = pathinfo($path_new, PATHINFO_DIRNAME);
                //pa($path_new_dirname);exit;
				if(!file_exists($path_new_dirname)){ mkdir($path_new_dirname, 0755, true); }

				$move_res = move_uploaded_file($up, $path_new);
				// all ok  - make  preview
				if ($move_res == true){
					$path_original = $path_new;
					$simg_obj = new Simage();
					if(!empty($this->size_settings)){

						foreach($this->size_settings as $ss_k=>$ss_val){
							$dir = $ss_val["size_dir"];
							$creating_file_path = $base_path."{$dir}/$filename";

							$creating_file_path_dirname = pathinfo($creating_file_path, PATHINFO_DIRNAME);
							if(!file_exists($creating_file_path_dirname)){ mkdir($creating_file_path_dirname, 0755, true); }

							switch(@$ss_val["resize_method"]){
								case "resize_proportional_by_height":
									$simg_obj->resize_proportional_by_height($path_original, $creating_file_path, $ss_val["size_single"]);
								break;
								case "resize_proportional_by_width":
									$simg_obj->resize_proportional_by_width($path_original, $creating_file_path,
										 $ss_val["size_single"], $new_h=null, $sharped = false);
								break;
								case "resize_smaller_side":
									$simg_obj->resize_smaller_side($path_original, $creating_file_path,
										 $ss_val["size_single"], $new_h=null, $sharped = false);
								break;
                                
                                case "resize_proportional_and_cut_by_width":
									$simg_obj->resize_proportional_and_cut_by_width($path_original, $creating_file_path,
										 $ss_val["size_width"], $ss_val["size_height"]);
                                    break;
                                case "resize_toch_outside":
									$simg_obj->resize_toch_outside($path_original, $creating_file_path,
										 $ss_val["size_width"], $ss_val["size_height"]);
                                    break;
								case "resize_propotional_exact":
									$simg_obj->ResizeProportionalExact($path_original,
										pathinfo($creating_file_path, PATHINFO_DIRNAME), $creating_file_path,
										 $ss_val["size_width"], $ss_val["size_height"]);
									break;
                                case "resizeByWidthAndCropByHeight":
									$simg_obj->resizeByWidthAndCropByHeight($path_original, $creating_file_path,
										 $ss_val["size_width"], $ss_val["size_height"]);
                                    break;
								default:
									$simg_obj->resize_biggest_side($path_original, $creating_file_path, $ss_val["size_single"]);
							}							
						}
					}
/*											
					// size1
					$dir = "size1";
					$creating_file_path = $base_path."{$dir}/$filename";
					$simg_obj->resize_biggest_side($path_original, $creating_file_path, $this->LOGO_SIZE1);
					// size2
					$dir = "size2";
					$creating_file_path = $base_path."{$dir}/$filename";
					$simg_obj->resize_biggest_side($path_original, $creating_file_path, TECH_STORE_SIZE2);
*/					
					
					$key = str_replace("_input_", "", $input_name);
					// return file name with delimiter
					$return_names[$key] = $filename;
				}
			}
			
		}// end main foreach
		//pa($return_names); exit;
		if($return_single_photoname == true && !empty($return_names)){
			return array_pop($return_names);
		}
		return $return_names;			
	}
	
	function removeAllSizesPhoto($photo_name)
	{
		$sizes = $this->size_settings;
		$sizes[-1] = array("size_dir" => "original");
		foreach ($sizes as $k=>$val){
			$full_path = $this->base_photo_path."/{$val["size_dir"]}/$photo_name";
			@unlink($full_path);
		}
	} 

    function isFileUploading()
    {
        $file_is_uploading = false;
        if (!empty($_FILES [$this->input_name] ['tmp_name']) ){
            $type = DefineFileType($_FILES [$this->input_name] ["type"]);
            $file_is_uploading = true;
        }

        return $file_is_uploading;
    }
	
	
}
	
	
?>
