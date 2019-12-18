<?php
/**
 * simple class for work with images
 * @type	php class
 * @author	Serg Parshin
 * @date	10/04/2007
 * ---------------
 * simple exapmple:
 *	<?
 *		include("class.simage.inc");
 *		$simg = new SImage();
 *		$simg->resize('source.jpg', null, 127, 127);
 *	?>
 */

class SImage
{
	var $img; // array of data of image(size, mime, resource  etc.)
	var $do_sharping = false;

	function __construct()
	{
		$this->img = array();
	}

	function get_file_ext($fn)
	{
		if(file_exists($fn))
		{
			$res=false;
			$info=@ getimagesize($fn);
			switch($info['mime'])
			{
				case 'image/jpg':
				case 'image/jpeg':
					$res='.jpg';
					break;
				case 'image/png' :
					$res='.png';
					break;
				case 'image/gif' :
					$res='.gif';
					break;
				case 'image/bmp' :
					$res='.bmp';
					break;
			}
			return $res;
		}
		return false;
	}

	/**
	* load image
	*/
	function load($fn)
	{
		if(file_exists($fn))
		{
			$info=@ getimagesize($fn);
			switch($info['mime'])
			{
				case 'image/jpg':
				case 'image/jpeg':
					$image=imagecreatefromjpeg($fn);
					break;
				case 'image/png' :
					$image=imagecreatefrompng($fn);
					break;
				case 'image/gif' :
					$image=imagecreatefromgif($fn);
					break;
				case 'image/bmp' :
					$image=imagecreatefromwbmp($fn);
					break;
				default:
					return false;
			}
			$this->img=array_merge($info, array('res'=>$image));
			return $this->img;
		}
		return false;
	}

	/**
	 * write image to file or to screen
	 * @param	(string)$fn -> file name where write or null for write to screen
	 * @param	(int)$quality -> image quality (jpeg, png)
	 */
	function write($fn=null,$quality=null)
	{
		$img=$this->img;
		switch($img['mime'])
		{
			case 'image/jpeg':
				@ imagejpeg($img['res'], $fn, $quality);
				break;
			case 'image/png' :
				//@ imagepng ($img['res'], $fn, $quality);
				@ imagepng ($img['res'], $fn, 8);
				break;
			case 'image/gif' :
				//@ imagejpeg($img['res'], $fn);
				@ imagegif($img['res'], $fn);
				break;
			case 'image/bmp' :
				@ imagewbmp($img['res'], $fn);
				break;
			default:
				return false;
		}
		return true;
	}

	/**
	 * write needed image header
	 */
	function image_header()
	{
		header('Content-type: '.$this->img['mime']);
	}

	/**
	 * return proportional sizes
	 * @param	(int)$w -> current width
	 * @param	(int)$h -> current height
	 * @param	(int)$need_w -> need width
	 * @param	(int)$need_h -> need width
	 * @return	(array) -> result sizes array(width, height)
	 */
	function get_proportional($w, $h, $need_w, $need_h)
	{
		if(!$w || !$h || !$need_w || !$need_h){return false;}
		$prop=(($need_w/$w) < ($need_h/$h)) ? ($need_w/$w) : ($need_h/$h);
		if($prop>1)
		{
			return array(
				0=>$w,
				1=>$h,
			);
		}

		return array(
			0=>intval($w*$prop),
			1=>intval($h*$prop),
		);
	}

	/**
	 * resize image
	 * @param	(string)$fn	-> source image file name
	 * @param	(string)$fn_to	-> write to file or null for print to screen
	 * @param	(int)$new_w	-> resized width
	 * @param	(int)$new_h	-> resized height
	 * @param	(bool)$proportional -> proportional resize ?
	 * @return	(bool) -> write is ok?
	 */
	function resize($fn, $fn_to, $new_w, $new_h, $proportional = true, $sharped = false)
	{
		$this->load($fn);
		if(empty($this->img)){return false;}
		if($proportional)
		{
			$res=$this->get_proportional($this->img[0], $this->img[1], $new_w, $new_h);
			$new_w=$res[0];
			$new_h=$res[1];
		}
		$image_p=imagecreatetruecolor($new_w, $new_h);
		imagecopyresampled($image_p,$this->img['res'],0,0,0,0,$new_w,$new_h,$this->img[0],$this->img[1]);

		if ($sharped == true){
			$amount = 40;
			$radius = 0.5;
			$threshold = 3;
			$this->UnsharpMask($image_p, $amount, $radius, $threshold);
		}

		$this->img['res']=$image_p;
		$this->img[0]=$new_w;
		$this->img[1]=$new_h;
		if(!$fn_to){$this->image_header($this->img);}
		return $this->write($fn_to, 100);
	}

	/**
	 * cut image
	 * @param	(string)$fn	-> source image file name
	 * @param	(string)$fn_to	-> write to file or null for print to screen
	 * @param	(int)$w		-> cut width
	 * @param	(int)$h		-> cut height
	 * @param	(int)$src_x	-> source x offset
	 * @param	(int)$src_y	-> source y offset
	 * @return	(bool) -> write is ok?
	 */
	function cut($fn, $fn_to, $w, $h, $src_x = 0, $src_y = 0)
	{
		$this->load($fn);

		if($this->img[0]<$w){$w=$this->img[0];}
		if($this->img[1]<$h){$h=$this->img[1];}

		$image_p=imagecreatetruecolor($w, $h);
		imagecopy($image_p, $this->img['res'], 0, 0, $src_x, $src_y, $w, $h);
		$this->img['res']=$image_p;
		$this->img[0]=$w;
		$this->img[1]=$h;
		if(!$fn_to){$this->image_header($this->img);}
		return $this->write($fn_to, 100);
	}



	/**
	 * уменьшение по БОЛЬШЕЙ стороне исходного
	 * БЕЗ ОБРЕЗАНИЯ, сохранение пропорций
	 */
	function resize_biggest_side($fn, $fn_to, $bigger_side_size)
	{
		$this->load($fn);
		if($bigger_side_size > $this->img[0] && $bigger_side_size > $this->img[1]){
			$new_w_ = $this->img[0];
			$new_h_ = $this->img[1];
		}else{
			// calculate proportions
			$prop = (($bigger_side_size / $this->img[0]) < ($bigger_side_size / $this->img[1])) ?
				($bigger_side_size / $this->img[0]) : ($bigger_side_size / $this->img[1]);

			$new_w_ = ceil($this->img[0] * $prop);
			$new_h_ = ceil($this->img[1] * $prop);
		}

		$image_p = imagecreatetruecolor($new_w_, $new_h_);
		imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);
		if ($this->do_sharping == true){
			$amount = 40;
			$radius = 0.5;
			$threshold = 3;
			$this->UnsharpMask($image_p, $amount, $radius, $threshold);
		}

		$this->img['res'] = $image_p;
		$this->img[0] = $new_w_;
		$this->img[1] = $new_h_;
		if(!$fn_to) $this->image_header($this->img);
		return $this->write($fn_to, 100);
	}

	/**
	 * уменьшение по МЕНЬШЕЙ стороне исходного
	 * БЕЗ ОБРЕЗАНИЯ, сохранение пропорций
	 */
	function resize_smaller_side($fn, $fn_to, $smaller_side_size)
	{
		$this->load($fn);
		if($smaller_side_size > $this->img[0] && $smaller_side_size > $this->img[1]){
			$new_w_ = $this->img[0];
			$new_h_ = $this->img[1];
		}else{
			// calculate proportions
			$prop = (($smaller_side_size / $this->img[0]) > ($smaller_side_size / $this->img[1])) ?
				($smaller_side_size / $this->img[0]) : ($smaller_side_size / $this->img[1]);

			$new_w_ = ceil($this->img[0] * $prop);
			$new_h_ = ceil($this->img[1] * $prop);
		}

		$image_p = imagecreatetruecolor($new_w_, $new_h_);
		imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);
		if ($this->do_sharping == true){
			$amount = 40;
			$radius = 0.5;
			$threshold = 3;
			$this->UnsharpMask($image_p, $amount, $radius, $threshold);
		}

		$this->img['res'] = $image_p;
		$this->img[0] = $new_w_;
		$this->img[1] = $new_h_;
		if(!$fn_to) $this->image_header($this->img);
		return $this->write($fn_to, 100);
	}

	/**
	 * уменьшение по меньшей стороне исходного
	 * рисунка и обрезание до нужных размеров
	 */
	function resize2($fn, $fn_to, $new_w, $new_h)
	{
		$this->load($fn);

		$prop = (($new_w / $this->img[0]) > ($new_h / $this->img[1])) ? ($new_w / $this->img[0]) : ($new_h / $this->img[1]);
		$new_w_ = intval($this->img[0] * $prop);
		$new_h_ = intval($this->img[1] * $prop);

		$image_p = imagecreatetruecolor($new_w_, $new_h_);
		imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);
		$this->img['res'] = $image_p;
		$this->img[0] = $new_w_;
		$this->img[1] = $new_h_;

		$image_p = imagecreatetruecolor($new_w, $new_h);
		imagecopy($image_p, $this->img['res'], 0, 0, 0, 0, $new_w, $new_h);
		$this->img['res'] = $image_p;
		$this->img[0] = $new_w;
		$this->img[1] = $new_h;
		if(!$fn_to) $this->image_header($this->img);
		return $this->write($fn_to, 100);
	}

	/**
	 * уменьшение по ширине рисунка
	 */
	function resize3($fn, $fn_to, $new_w, $sharped = false)
	{
		$this->load($fn);
		if(empty($this->img)){return false;}
		$width_source = $this->img[0];
		$prop = $new_w / $this->img[0];
		$new_w_ = intval($this->img[0] * $prop);
		$new_h_ = intval($this->img[1] * $prop);

		if ($width_source > $new_w){ // source width is bigger than allowed one - resize

			if($new_w_>$this->img[0])
			{
				$new_w_=$this->img[0];
				$new_h_=$this->img[1];
			}

			$image_p = imagecreatetruecolor($new_w_, $new_h_);
			imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);
			// sharping
			//echo "<h1>sharping new_w_ = $new_w_   width_source = ".$width_source."</h1>";
			if ($sharped == true && $new_w_ < $width_source){
				//echo "<h1>sharping</h1>";exit;
				$amount = 40;
				$radius = 0.5;
				$threshold = 3;
				$this->UnsharpMask($image_p, $amount, $radius, $threshold);
			}
			$this->img['res'] = $image_p;
			$this->img[0] = $new_w_;
			$this->img[1] = $new_h_;

			if(!$fn_to) $this->image_header($this->img);
			return $this->write($fn_to, 100);
		}else{ // source width is smaller than allowed one - simple copy
			$copy_res = copy($fn, $fn_to);
			return $copy_res;
		}
	}

	/**
	 * уменьшение по ширине рисунка и обрезает по высоте
	 */
	function resize4($fn, $fn_to, $new_w, $new_h)
	{
		$this->load($fn);

		$prop = $new_w / $this->img[0];
		$new_w_ = intval($this->img[0] * $prop);
		$new_h_ = intval($this->img[1] * $prop);

		$image_p = imagecreatetruecolor($new_w_, $new_h_);
		imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);
		$this->img['res'] = $image_p;
		$this->img[0] = $new_w_;
		$this->img[1] = $new_h_;

		if($new_h > $new_h_)
		{
			$new_h = $new_h_;
		}
		$image_p = imagecreatetruecolor($new_w, $new_h);
		imagecopy($image_p, $this->img['res'], 0, 0, 0, 0, $new_w, $new_h);
		$this->img['res'] = $image_p;
		$this->img[0] = $new_w;
		$this->img[1] = $new_h;
		if(!$fn_to) $this->image_header($this->img);
		return $this->write($fn_to, 100);
	}

	/**
	 * (~toch outside) вписывает по меньшей стороне и обрезает лишнее
	 */
	function resize5($fn, $fn_to, $new_w, $new_h)
	{
		$this->load($fn);

		$prop = (($new_w / $this->img[0]) > ($new_h / $this->img[1])) ? ($new_w / $this->img[0]) : ($new_h / $this->img[1]);
		$new_w_ = intval($this->img[0] * $prop);
		$new_h_ = intval($this->img[1] * $prop);
		if(($new_w / $this->img[0]) > ($new_h / $this->img[1]))
		{
			$src_x = 0;
			$src_y = ($new_h_ / 2)-($new_h / 2);
		}
		else
		{
			$src_x = ($new_w_ / 2)-($new_w / 2);
			$src_y = 0;
		}

		$image_p = imagecreatetruecolor($new_w_, $new_h_);
		imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);
		$this->img['res'] = $image_p;
		$this->img[0] = $new_w_;
		$this->img[1] = $new_h_;

		$image_p = imagecreatetruecolor($new_w, $new_h);
		imagecopy($image_p, $this->img['res'], 0, 0, $src_x, $src_y, $new_w, $new_h);
		$this->img['res'] = $image_p;
		$this->img[0] = $new_w;
		$this->img[1] = $new_h;

		if(!$fn_to) $this->image_header($this->img);
		return $this->write($fn_to, 100);
	}


	/** ЧП функции **/

	/**
	 * простое НЕ пропорциональное изменение размера рисунка
	 */
	function resize_no_proportional($fn, $fn_to, $new_w, $new_h)
	{
		return $this->resize($fn, $fn_to, $new_w, $new_h, false);
	}

	/**
	 * простое пропорциональное изменение размера рисунка
	 */
	function resize_proportional($fn, $fn_to, $new_w, $new_h)
	{
		return $this->resize($fn, $fn_to, $new_w, $new_h);
	}

	/**
	 * уменьшение по меньшей стороне исходного
	 * рисунка и обрезание до нужных размеров
	 */
	function resize_proportional_by_max_side_and_cut_excessive($fn, $fn_to, $new_w, $new_h)
	{
		return $this->resize2($fn, $fn_to, $new_w, $new_h);
	}

	/**
	 * уменьшение по ширине рисунка
	 */
	function resize_proportional_by_width($fn, $fn_to, $new_w, $new_h=null, $sharped = false)
	{
		return $this->resize3($fn, $fn_to, $new_w, $sharped);
	}

	function resize_topgal($fn, $fn_to)
	{
		$img_info = getimagesize($fn);
		$prop = $this->get_proportional ($img_info[0], $img_info[1], RUB_TOPGAL_W, RUB_TOPGAL_H);
		//print_r($prop);
		$this->resize($fn, $fn_to, $prop[0], $prop[1], $proportional = true);
	}
	/**
	 * уменьшение по высоте рисунка
	 */
	function resize_proportional_by_height($fn, $fn_to, $new_h)
	{
		$this->load($fn);
		if(empty($this->img)){return false;}

		$prop = $new_h / $this->img[1];
		$img_info = getimagesize($fn);
		//pa($prop, $img_info);exit;

		//if ratio > 1.5 resize by width
		if ( ($img_info[0]/$img_info[1]) > 1.5 ){
			$this->resize_proportional_by_width($fn, $fn_to, RUB_PHOTO2_W*10);
		}
		/*
			$resized_h = $img_info[1];
			$resized_w = $img_info[0];
			$infelicity = 10;  /// has been chosen empiricaly
			if ($resized_h / $resized_w > 2.2 || $resized_w / $resized_h > 2.2 ){
				$infelicity = 50;
			}elseif ($resized_h / $resized_w > 1.8 || $resized_w / $resized_h > 1.8 ){
				$infelicity = 30;
			}elseif($resized_h / $resized_w > 1.45 || $resized_w / $resized_h > 1.45 ) {
				$infelicity = 20;
			}
		*/

		$new_w_ = intval($this->img[0] * $prop);
		$new_h_ = intval($this->img[1] * $prop);
		if($new_w_>$this->img[0])
		{
			$new_w_=$this->img[0];
			$new_h_=$this->img[1];
		}

		$image_p = imagecreatetruecolor($new_w_, $new_h_);
		imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);
		$this->img['res'] = $image_p;
		$this->img[0] = $new_w_;
		$this->img[1] = $new_h_;

		if(!$fn_to) $this->image_header($this->img);
		return $this->write($fn_to, 100);
	}

	/**
	 * уменьшение по ширине рисунка и обрезает по высоте
	 */
	function resize_proportional_and_cut_by_width($fn, $fn_to, $new_w, $new_h)
	{
		return $this->resize4($fn, $fn_to, $new_w, $new_h);
	}

	/**
	 * (~toch outside) вписывает по меньшей стороне и обрезает лишнее
	 */
	function resize_toch_outside($fn, $fn_to, $new_w, $new_h)
	{
		return $this->resize5($fn, $fn_to, $new_w, $new_h);
	}

	/**
	 * простое копирование рисунка
	 */
	function simply_copy($fn, $fn_to, $new_w=null, $new_h=null)
	{
		return @ copy($fn,$fn_to);
	}

	///=========================================================================
	/**			PARAMS
		$source_file
		$creating_file_path 		ex.	/var/www/proj/images
		$creating_file_name		ex.	risking.jpg
		....
		$result = array(
					'error'	=>	'',
					'ret_val'	=>	''
					);
	*/
	function ResizeIntoSmallSquare ($source_file, $creating_file_path, $creating_file_name, $need_w, $need_h)
	{
		$result = array(
					'error'	=>	'',
					'ret_val'	=>	''
					);
		$up = $source_file;
		$info = getimagesize($up);
		//$simg_obj = new SImage();
		if ($info[0] >= $need_w && $info[1] >= $need_h){
			//$new_name = $newsimgs_path.LANG."_".time().".".$type;
			$new_path = $creating_file_path.$creating_file_name;//PATH_IMAGE.DIRECTORY_SEPARATOR.$new_name;
			$sizex = $need_w;
			$sizey = $need_h;
			$prop = $this->get_proportional ($info[0], $info[1], $sizex, $sizey);

			$resized_w = $prop[0];
			$resized_h = $prop[1];
			$infelicity = 10;  /// has been chosen empiricaly
			if ($resized_h / $resized_w > 2.2 || $resized_w / $resized_h > 2.2 ){
				$infelicity = 50;
			}elseif ($resized_h / $resized_w > 1.8 || $resized_w / $resized_h > 1.8 ){
				$infelicity = 30;
			}elseif($resized_h / $resized_w > 1.45 || $resized_w / $resized_h > 1.45 ) {
				$infelicity = 20;
			}

			if ($info[0] > $sizex && $info[1] > $sizey){
				/// shirina norma, height menshe
				if ( $resized_h < $sizey ){
					$differance = $resized_w - $resized_h;
					$square_size = $resized_w+$differance+$infelicity;
				}elseif ($resized_w < $sizex){
					$differance = $resized_h - $resized_w;
					$square_size = $resized_h+$differance+$infelicity;
				}
				//echo "<h3>SQUARE = $square_size   |   resized_w=$resized_w   |   resized_h=$resized_h</h3>";
				//$simg_obj->cut($up, $new_path, $sizex, $sizey, $src_x = 0, $src_y = 0);
				//echo "<h3>newpath = $new_path   |   resized_w=$resized_w   |   resized_h=$resized_h</h3>";

				$this->resize ($up, $new_path, $square_size, $square_size, true);
				$this->cut($new_path, $new_path, $sizex, $sizey, $src_x = 0, $src_y = 0);
				//$VARS['img'] = $creating_file_name;
				$result ['ret_val'] = $creating_file_name;
			/// size is exactly needed, do direct copying
			}elseif($info[0] == $sizex && $info[1] == $sizey){
				copy ($up, $new_path);
				$result ['ret_val'] = $creating_file_name;
			}
		}else{ /// image is smaller
			$result ['error'] = "TOO_SMALL";
		}

		return $result;
	}

	function ResizeProportionalExact ($source_file, $creating_file_path, $creating_file_name, $need_w, $need_h, $fixed = "w")
	{
		//echo "<h4> SF = $source_file FPATH= $creating_file_path, FNAME = $creating_file_name, NEED_W= $need_w, NEED_H = $need_h, FIXED = $fixed </h4>";
		$info = getimagesize($source_file);
		$sizex = $need_w;
		$sizey = $need_h;

		if ($info[0] > $info[1]){
			$fixed = "h";
		}else{
			$fixed = "w";
		}

		if ( $fixed == "w" ){
			$need_h *=10;
		}else{
			$need_w *=10;
		}

		if ($info[0] >= $sizex && $info[1] >= $sizey){
			$prop = $this->get_proportional ($info[0], $info[1], $need_w, $need_h);
			$new_path = $creating_file_path.$creating_file_name;
			$this->resize ($source_file, $new_path, $prop[0], $prop[1], true);
			//pa($prop);
			///******** calculating start positions for cutting
			$src_x = floor( ( $prop[0] - $sizex ) / 2 );
			$src_y = floor( ( $prop[1] - $sizey) / 2 );
			//pa($src_x, $src_y);
			if ($src_x < 0){
				$src_x = 0;
			}
			if ($src_y < 0){
				$src_y = 0;
			}

			$this->cut ($new_path, $new_path, $sizex, $sizey, $src_x , $src_y );
			///$this->cut ($new_path, $new_path, $sizex, $sizey, $src_x = 0, $src_y = 0);
			$result ['ret_val'] = $creating_file_name;
		}else{ /// image is smaller
			$result ['error'] = "TOO_SMALL";
		}
		return $result;

	}


	// sharp - резкость картинки http://vikjavev.no/computing/ump.php
  function UnsharpMask($img, $amount, $radius, $threshold)    {

    if ($amount > 500)    $amount = 500;
    $amount = $amount * 0.016;
    if ($radius > 50)    $radius = 50;
    $radius = $radius * 2;
    if ($threshold > 255)    $threshold = 255;

    $radius = abs(round($radius));     // Only integers make sense.
    if ($radius == 0) return $img;
    $w = imagesx($img); $h = imagesy($img);
    $imgCanvas = imagecreatetruecolor($w, $h);
    $imgCanvas2 = imagecreatetruecolor($w, $h);
    $imgBlur = imagecreatetruecolor($w, $h);
    $imgBlur2 = imagecreatetruecolor($w, $h);
    imagecopy ($imgCanvas, $img, 0, 0, 0, 0, $w, $h);
    imagecopy ($imgCanvas2, $img, 0, 0, 0, 0, $w, $h);


    // Gaussian blur matrix:
    //
    //    1    2    1
    //    2    4    2
    //    1    2    1
    //
    //////////////////////////////////////////////////

    imagecopy      ($imgBlur, $imgCanvas, 0, 0, 0, 0, $w, $h); // background

    for ($i = 0; $i < $radius; $i++)    {

        if (function_exists('imageconvolution')) { // PHP >= 5.1
            $matrix = array(
                array( 1, 2, 1 ),
                array( 2, 4, 2 ),
                array( 1, 2, 1 )
                );
            imageconvolution($imgCanvas, $matrix, 16, 0);

        } else {

            // Move copies of the image around one pixel at the time and merge them with weight
            // according to the matrix. The same matrix is simply repeated for higher radii.

            imagecopy      ($imgBlur, $imgCanvas, 0, 0, 1, 1, $w - 1, $h - 1); // up left
            imagecopymerge ($imgBlur, $imgCanvas, 1, 1, 0, 0, $w, $h, 50); // down right
            imagecopymerge ($imgBlur, $imgCanvas, 0, 1, 1, 0, $w - 1, $h, 33.33333); // down left
            imagecopymerge ($imgBlur, $imgCanvas, 1, 0, 0, 1, $w, $h - 1, 25); // up right

            imagecopymerge ($imgBlur, $imgCanvas, 0, 0, 1, 0, $w - 1, $h, 33.33333); // left
            imagecopymerge ($imgBlur, $imgCanvas, 1, 0, 0, 0, $w, $h, 25); // right
            imagecopymerge ($imgBlur, $imgCanvas, 0, 0, 0, 1, $w, $h - 1, 20 ); // up
            imagecopymerge ($imgBlur, $imgCanvas, 0, 1, 0, 0, $w, $h, 16.666667); // down

            imagecopymerge ($imgBlur, $imgCanvas, 0, 0, 0, 0, $w, $h, 50); // center
            imagecopy ($imgCanvas, $imgBlur, 0, 0, 0, 0, $w, $h);

            // During the loop above the blurred copy darkens, possibly due to a roundoff
            // error. Therefore the sharp picture has to go through the same loop to
            // produce a similar image for comparison. This is not a good thing, as processing
            // time increases heavily.
            imagecopy ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h);
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 50);
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 33.33333);
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 25);
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 33.33333);
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 25);
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 20 );
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 16.666667);
            imagecopymerge ($imgBlur2, $imgCanvas2, 0, 0, 0, 0, $w, $h, 50);
            imagecopy ($imgCanvas2, $imgBlur2, 0, 0, 0, 0, $w, $h);

        }
    }
    //return $imgBlur;

    // Calculate the difference between the blurred pixels and the original
    // and set the pixels
    for ($x = 0; $x < $w; $x++)    { // each row
        for ($y = 0; $y < $h; $y++)    { // each pixel

            $rgbOrig = ImageColorAt($imgCanvas2, $x, $y);
            $rOrig = (($rgbOrig >> 16) & 0xFF);
            $gOrig = (($rgbOrig >> 8) & 0xFF);
            $bOrig = ($rgbOrig & 0xFF);

            $rgbBlur = ImageColorAt($imgCanvas, $x, $y);

            $rBlur = (($rgbBlur >> 16) & 0xFF);
            $gBlur = (($rgbBlur >> 8) & 0xFF);
            $bBlur = ($rgbBlur & 0xFF);

            // When the masked pixels differ less from the original
            // than the threshold specifies, they are set to their original value.
            $rNew = (abs($rOrig - $rBlur) >= $threshold)
                ? max(0, min(255, ($amount * ($rOrig - $rBlur)) + $rOrig))
                : $rOrig;
            $gNew = (abs($gOrig - $gBlur) >= $threshold)
                ? max(0, min(255, ($amount * ($gOrig - $gBlur)) + $gOrig))
                : $gOrig;
            $bNew = (abs($bOrig - $bBlur) >= $threshold)
                ? max(0, min(255, ($amount * ($bOrig - $bBlur)) + $bOrig))
                : $bOrig;



            if (($rOrig != $rNew) || ($gOrig != $gNew) || ($bOrig != $bNew)) {
                $pixCol = ImageColorAllocate($img, $rNew, $gNew, $bNew);
                ImageSetPixel($img, $x, $y, $pixCol);
            }
        }
    }

    return $img;
  }

    //-------------------------
    // resizes image to fixed width and height
    // if image x and y  are greater than required params than first resize image by width
    // and then make crop (if required)
    // in the other cases make only one dimesion(width or height crop)
    // or simply copy image if both dimensions are less than required params
    //-------------------------
    function resizeByWidthAndCropByHeight($fn, $fn_to, $new_w_param, $crop_h)
    {
        $this->load($fn);
        if(empty($this->img)){return false;}
        $width_source = $this->img[0];
        $height_source = $this->img[1];
        $final_img_res = null;

        //--- src_x > dest_x && src_y > dest_y ---//
        if($width_source > $new_w_param && $height_source > $crop_h ){
            $prop = $new_w_param / $width_source;
            $new_w_ = intval($width_source * $prop);
            $new_h_ = intval($height_source * $prop);
            $image_p = imagecreatetruecolor($new_w_, $new_h_);
            imagecopyresampled($image_p, $this->img['res'], 0, 0, 0, 0, $new_w_, $new_h_, $this->img[0], $this->img[1]);

            //--- do crop [ src_x = dst_x && src_y after resize is still > crop_h ] ---//
            $image_res_crop = null;
            if($new_h_ > $crop_h){
                $crop_Y = ceil(($new_h_ - $crop_h)/2);
                $image_res_crop = imagecreatetruecolor($new_w_param, $crop_h);
                imagecopy($image_res_crop, $image_p, 0, 0, $x_o=0, $crop_Y, $new_w_param, $crop_h);
                $final_img_res = $image_res_crop;
                $this->img[0] = $new_w_param;
                $this->img[1] = $crop_h;
            }else{
                $final_img_res = $image_p;
                $this->img[0] = $new_w_param;
                $this->img[1] = $new_h_;
            }
        }

        //--- simple copy ---//
        elseif(
                ( $width_source == $new_w_param && $height_source == $crop_h )
                ||
                ( $width_source < $new_w_param && $height_source < $crop_h )
            )
        {
            $copy_res = copy($fn, $fn_to);
            return $copy_res;
        }

        //--- only crop by heigt ---//
        elseif( $width_source <= $new_w_param && $height_source >= $crop_h )
        {
            $crop_Y = ceil(($height_source - $crop_h)/2);
            $image_dest_crop = imagecreatetruecolor($width_source, $crop_h);
            imagecopy($image_dest_crop, $this->img['res'], 0, 0, $x_o=0, $crop_Y, $width_source, $crop_h);
            $final_img_res = $image_dest_crop;

            $this->img[0] = $width_source;
            $this->img[1] = $crop_h;
        }

        //--- only crop by width ---//
        elseif( $width_source >= $new_w_param && $height_source <= $crop_h )
        {
            $crop_X = ceil(($width_source - $new_w_param)/2);
            $image_dest_crop = imagecreatetruecolor($new_w_param, $height_source);
            imagecopy($image_dest_crop, $this->img['res'], 0, 0, $crop_X, 0, $new_w_param, $height_source);
            $final_img_res = $image_dest_crop;
            $this->img[0] = $new_w_param;
            $this->img[1] = $height_source;
        }

        $this->img['res'] = $final_img_res;

        if(!$fn_to) $this->image_header($this->img);
        return $this->write($fn_to, 100);
    }


}

/**
 * end of SImage class
 */

?>