<?php

//-------------------------
// used to replace gallery tag to real html data
//-------------------------
class GalleryMultiUpload
{
	static private $DB = null;
    const TABLE_GALLERY = "base_galleries";
    CONST TABLE_PHOTOS_TABLE = "base_gallery_photos";
	//static public $gallery_photos_table = "base_gallery_photos";

    //--- used to replace ta ---//
	static private $pocketNumbers = 
		array(
			"tpl_name" => 4,
			"gallery_id" => 2,
		);
	//--- template types ---//
	static public $tplTypes = 
		array(
			"standart"	=> 
				array(
					"file" => "common:common/blocks/gallery_item_standart.tpl",
				),
            /*
			"full"	=> 
				array(
					"file" => "common:common/blocks/gallery_item_full.tpl",
				)
            */
		);
	static function getFileByTplName($tpl_name)
	{
		return self::$tplTypes[$tpl_name]["file"];
	}
 	static function initDb(&$db)
 	{
		self::$DB = $db;
	}
		
		
	static function generateGalleryInsertCode($gallery_id)
	{
		$ready_code = array();
		foreach(array_keys(self::$tplTypes) as $tplTypeVal){
			$ready_code[] = "%%gallery id='{$gallery_id}' tpl_{$tplTypeVal}%%";
		}
		$str_ready_code = implode("\n<br />\n",$ready_code);
		$res = array(
				"arr" => $ready_code,
				"str" => $str_ready_code
			);
		return $res;
	}

    static function replaceGalleryTagsExt($body, Entity $entity)
    {
        $gallery_replaced_res  = GalleryMultiUpload::replaceGalleryTag($body);
        //pa($gallery_replaced_res);

        if($gallery_replaced_res !== false){
            $entity->TML->assign("jquery_galleries", true);
            $entity->TML->assign("galleries_structure", $gallery_replaced_res["galleries_structure"]);
            $body = $gallery_replaced_res["text_body"];
        }

        return $body;
    }

    static function assignGalleryOptions(Entity $en, $visible = 3, $scroll = 2)
    {
        $galleryOptions = array(
            'visible' => $visible,
            'scroll' => $scroll
        );
        $en->TML->assign("galleryOptions", $galleryOptions);
    }

	static function replaceGalleryTag($text_body)
	{
		$text_body = html_entity_decode($text_body, ENT_QUOTES, "utf-8");
		$has_gallery_tag = false;
		$pattern = "#%%(gallery id=[\"']{1}([\d]+)[\"']{1})( tpl_([a-zA-Z0-9_]+))?[^%]*%%#im";
		
		//#------------------- get gallries structure -------------------#//
		preg_match_all($pattern, $text_body, $matches_all);
		//pa($matches_all);
		$pocketNumForGalleryId = self::$pocketNumbers["gallery_id"];// see $pattern
		$pocketNumForTplName = self::$pocketNumbers["tpl_name"];// see $pattern
	 	if(!empty($matches_all[$pocketNumForGalleryId])){
		 	$has_gallery_tag = true;	
			//--- this pocket contains galleries ids ---//
			$galleries_structure = array();
			if(!empty($matches_all[$pocketNumForGalleryId])){
				foreach($matches_all[$pocketNumForGalleryId] as $gallery_id_val){
					$gallery_id_val = (int)$gallery_id_val;
					//--- can be placed 2 equal galleries on the same page ---//
					if(!isset($galleries_structure[$gallery_id_val])){
						$sql = "
							SELECT * 
							FROM `".self::TABLE_PHOTOS_TABLE."`
							WHERE
								`id_gallery` = '$gallery_id_val'
							ORDER BY 
								`date` DESC
						";
						$gallery_photos = self::$DB->RunSql($sql, true);
						$galleries_structure[$gallery_id_val] = $gallery_photos;
					}
				}
			}
			//pa($galleries_structure);

			$text_body = preg_replace_callback($pattern, 
				"GalleryMultiUpload::callBackForPregReplace",
				$text_body);
			
			//--- include plugin css and js files in header ---//
			//$this->TML->assign("jquery_galleries", true);
			//$this->TML->assign("galleries_structure", $galleries_structure);
			
			return  array(
					"galleries_structure" => $galleries_structure,
					"text_body" => $text_body						
				);
		}else{
			return false;
		}
	}
	
	
	static function callBackForPregReplace($matches)
	{
		$tplPocketNum = self::$pocketNumbers["tpl_name"];
		$galleryIdPocketNum = self::$pocketNumbers["gallery_id"];
		$tpl_name = "carousel"; // by default
		if(!empty($matches[$tplPocketNum])){
			$tpl_name = $matches[$tplPocketNum];
		}
		$tpl_file = self::getFileByTplName($tpl_name);
		$gallery_id =  $matches[$galleryIdPocketNum];
        $replaced_string = "";
		
        if(self::checkGalleryIdCorrectness($gallery_id)){
            //--- create smarty include operation ---//
		    $replaced_string = '<!--{include file="'.$tpl_file.'" plugin_gallery_id="'.$gallery_id.'" }-->';
        }
		//echo "<h2>================== $replaced_string =========================</h2>";
		return $replaced_string;
	}

    //-------------------------
    // gallery exists and have some photos
    //-------------------------
    static function checkGalleryIdCorrectness($gallery_id)
    {
        $ret = false;
        $sql = "
            SELECT *,
                (
                    SELECT COUNT(*)
                    FROM `".self::TABLE_PHOTOS_TABLE."`
                    WHERE
                        `id_gallery` = '$gallery_id'
                ) as `photos_count`
            FROM `".self::TABLE_GALLERY."`
            WHERE
                `id` = '$gallery_id'
        ";
        $gallery_info = self::$DB->RunSql($sql);

        if(!empty($gallery_info) && $gallery_info["photos_count"] > 0 ){
            $ret = true;
        }
        return $ret;
    }
	
}


?>