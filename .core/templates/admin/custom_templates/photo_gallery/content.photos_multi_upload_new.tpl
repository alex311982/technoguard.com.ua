<!-- = = = = = = [ swf uploader scripts ] = = = = = = -->
<script type="text/javascript" src="/-js/swfupload/swfupload.js"></script>
<script type="text/javascript" src="/-js/swfupload/handlers.js?rnd=1"></script>
<script type="text/javascript" src="/-js/swfupload/swfupload.queue.js"></script>
<script type="text/javascript">
	var swfu;
	window.onload = function () {
		swfu = new SWFUpload({
			// Backend Settings
			upload_url: "<!--{$swf_saving_url}-->",
			post_params: {"PHPSESSID": "<!--{$session_id}-->"},

			// File Upload Settings
			file_size_limit : "500 MB",	// 2MB
			file_types : "*.jpg;*.jpeg;*.png;*.gif",
			file_types_description : "Images",
			file_upload_limit : "0",

			// Event Handler Settings - these functions as defined in Handlers.js
			//  The handlers are not part of SWFUpload but are part of my website and control how
			//  my website reacts to the SWFUpload events.
//						queue_complete_handler : queueComplete,
			file_queue_error_handler : fileQueueError,
			file_dialog_complete_handler : fileDialogComplete,
			upload_progress_handler : uploadProgress,
			upload_error_handler : uploadError,
			upload_success_handler : uploadSuccess,
			upload_complete_handler : uploadComplete,
			queue_complete_handler : queueComplete,	// Queue plugin event
			swfupload_loaded_handler : swfUploadLoaded,

			// Button Settings
			button_image_url : "/-js/swfupload/images/SmallSpyGlassWithTransperancy_17x18.png",
			button_placeholder_id : "spanButtonPlaceholder",
			button_width: 180,
			button_height: 18,
			button_text : '<span class="button">Выбрать фотографии</span>',
			button_text_style : '.button { font-family: Helvetica, Arial, sans-serif; font-size: 12pt; } .buttonSmall { font-size: 10pt; }',
			button_text_top_padding: 0,
			button_text_left_padding: 18,
			button_window_mode: SWFUpload.WINDOW_MODE.TRANSPARENT,
			button_cursor: SWFUpload.CURSOR.HAND,
			
			// Flash Settings
			flash_url : "/-js/swfupload/swfupload.swf",

			custom_settings : {
				upload_target : "divFileProgressContainer",
				cancelButtonId : "btnCancel"
			},
			
			// Debug Settings
			debug: false
		});
	};
</script>

<form id="main_form" action="" method="post" enctype="multipart/form-data">
<input type="hidden" name="_input_is_form_submited" />
<input type="hidden" name="MAX_FILE_SIZE" value="50000000">

<!--{*include file="common:toolbars/user_media/edit.tpl"*}-->


<!-- = = = = = = [ main ] = = = = = = -->
<div class="main"  id="main-scroll">
	<!--{include file="common:block.errors.tpl"}-->
	
	<div id="idResultMessage" class="result-message" style="display:none;">
		<strong>Фотографии загружены.</strong> Перейти к <a href="<?=$url_parent_gallery?>">текущей галерее</a> <br>
		<input type="hidden" id="idGallery" value="<!--{$id_gallery}-->" />
		<input type="hidden" id="idQueueCompleteUrl" value="/admin/galleries/category/photos_browse.html?gallery_id=<!--{$id_gallery}-->" />
	</div>
	<br>
	
	<div style="display: inline; border: solid 1px #7FAAFF; background-color: #C5D9FF; padding: 6px; padding-top:10px;">
		<span id="spanButtonPlaceholder"></span>
		&nbsp;
		<input id="btnCancel" type="button" value="Отменить загрузку" disabled="disabled"  />
	</div>
	

	<br><br>
	<div id="divFileProgressContainer" style="padding-left:none; height: 75px;"></div>
	
	<div id="thumbnails" style="width:1000px;"></div>

	
</div>
</form>