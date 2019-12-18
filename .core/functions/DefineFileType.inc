<?php

function DefineFileType($defining_type)
{

	switch ($defining_type)
	{
		case "image/gif":
			$type = "gif";
		break;
		case "image/jpeg":
			$type = "jpg";
		break;
		case "image/jpg":
			$type = "jpg";
		break;
		case "image/png":
			$type = "png";
		break;

		case "image/pjpeg":
			$type = "jpg";
		break;
		case "image/x-png":
			$type = "png";
		break;
		default:
			$type = "unknown";
	}
	return $type;

}

function GetMimeTypeByExtension($path)
{
	$path_info = pathinfo($path);
    $ext = $path_info['extension'];
    
	switch ($ext)
	{
		case "gif":
			$type = "image/gif";
		break;
		case "jpg":
			$type = "image/jpeg";
		break;
		case "jpg":
			$type = "image/jpg";
		break;
		case "png":
			$type = "image/png";
		break;

		case "png":
			$type = "image/x-png";
		break;
		case "txt":
			$type = "text/plain";
		break;
		default:
			$type = "application/octet-stream";
	}
	return $type;    
}

function DefineFileExtension($fname)
{
	$tmp_parts = pathinfo($fname);
	$extension = @$tmp_parts['extension'];
	return $extension;
}

?>