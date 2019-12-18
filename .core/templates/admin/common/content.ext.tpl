
<form id="main_form" action="" method="post" enctype="multipart/form-data">
<!--{include file="common:toolbar_ext.tpl"}-->
<div class="main"  id="main-scroll">
<!--{include file="common:block.errors.tpl"}-->
<table width="100%" cellspacing="0" cellpadding="2" border="0" class="grid">
	<!--{foreach from=$result.fields key=name item=field}-->
	<!--{assign var=input_name value="_input_`$name`"}-->

	<!--{if !empty($smarty.post.$input_name)}-->
		<!--{assign var=value value=$smarty.post.$input_name|escape}-->
	<!--{else}-->
		<!--{assign var=value value=$result.record.$name.value|escape}-->
	<!--{/if}-->

	<tr<!--{cycle values=', class="bg"'}-->>
		<!--{if $field.view == true && ($field.type != 'autotext' || $value != '')}-->
			<td class="title"><!--{$field.title}--></td><td class="req"><!--{if $field.required}-->*<!--{else}-->&nbsp;<!--{/if}--></td><td>
			<!--{if $field.type == 'text'}-->
				<input<!--{if $field.readonly}--> readonly<!--{/if}--> type="text" value="<!--{$value}-->" name="_input_<!--{$name}-->"/>
			<!--{elseif $field.type == 'textarea'}-->
				<textarea <!--{if $field.readonly}--> readonly<!--{/if}--> name="_input_<!--{$name}-->" rows="<!--{$field.rows}-->"/><!--{$value}--></textarea>
			<!--{elseif $field.type == 'html'}-->
				<textarea name="_input_<!--{$name}-->" rows="10" cols="80" style="width: 100%; height: 200px"><!--{$value}--></textarea>
				<script type="text/javascript">
				<!--
					var oFCKeditor = new FCKeditor("_input_<!--{$name}-->");
					oFCKeditor.BasePath = "_fck";
					oFCKeditor.ReplaceTextarea();
				//-->
				</script>
			<!--{elseif $field.type == 'html_new'}-->
				<script type="text/javascript" src="-js/tiny_mce/tiny_mce.js"></script>  
				<script language="javascript" type="text/javascript">
					//alert("risking");
					// Notice: The simple theme does not use all options some of them are limited to the advanced theme
					tinyMCE.init({
						mode : "textareas",
						theme : "advanced",
						editor_selector : "mceEditor",
						content_css : "-css/main.css",
						theme_advanced_disable : "separator,cleanup,strikethrough,undo,redo,bullist,numlist,styleselect,formatselect,help,visualaid,removeformat,hr,code,anchor,sub,sup,charmap,indent,outdent,separator,justifyfull,justifyleft,justifycenter,justifyright,link,unlink,image"
					});
				</script>
				<textarea name="_input_<!--{$name}-->" rows="10" cols="80" style="width: 100%; height: 200px" class="mceEditor"><!--{$value}--></textarea>
			<!--{elseif $field.type == 'file'}-->
				<!--{if $value}-->
<!--
picture
					<img src = '-imgs/<!--{$value}-->'><br>
-->
					<!--{if $result.record.typed.value == 'swf'}-->
						<object type="application/x-shockwave-flash" data='-imgs/<!--{$result.record.banner.value}-->' width="<!--{$result.record.widthx.value}-->" 
							height="<!--{$result.record.heighty.value}-->" id="flash" align="middle">
							<param name="allowScriptAccess" value="sameDomain" />
							<param name="movie" value='-imgs/<!--{$result.record.banner.value}-->' />
							<param name="menu" value="false" />
							<param name="quality" value="high" />
							<param name="scale" value="noscale" />
							<param name="bgcolor" value="#ffffff" />
						</object>
						<br>
					<!--{else}-->
						<img src = '-imgs/<!--{$value}-->'><br>
					<!--{/if}-->

					If you want to delete this image, please, mark: <input type = "checkbox" name = "_input_check_image" value = "ok" class="chk">
				<!--{/if}-->
				<input<!--{if $field.readonly}--> readonly<!--{/if}--> type="file" name="_input_<!--{$name}-->"/>
			<!--{elseif $field.type == 'autotext'}-->
				<!--{if $value}-->
					<input<!--{if $field.readonly}--> readonly<!--{/if}--> type="text" value="<!--{$value}-->" name="_input_<!--{$name}-->"/>
				<!--{/if}-->
			<!--{elseif $field.type == 'password'}-->
				<input<!--{if $field.readonly}--> readonly<!--{/if}-->  type="password" value="" name="_input_<!--{$name}-->"/>
			<!--{elseif $field.type == 'date'}-->
				<!--{if empty($result.record.$name.start_year)}-->
					<!--{assign var=start_year value=$result.fields.$name.start_year}-->
				<!--{else}-->
					<!--{assign var=start_year value=$result.record.$name.start_year}-->
				<!--{/if}-->
				<!--{include file="common:ui.date_picker.tpl" date_value=$result.record.$name.value attrs=$field start_year=$start_year date_name=$name}-->
			<!--{elseif $field.type == 'date_readonly'}-->
				<!--{$value|date_format:"%e-%m-%Y"}-->
			<!--{elseif $field.type == 'select'}-->
				<select<!--{if $field.readonly}--> readonly<!--{/if}-->  name="_input_<!--{$name}-->">
					<!--{html_options options=$field.options selected=$value}-->
				</select>
			<!--{elseif $field.type == 'image'}-->
				<!--{if !empty ($value) }-->
					<img src = '-imgs/<!--{$value}-->' ><br>
				<!--{else}-->
					<img src = '-imgs/default/nophoto.jpg' ><br>
				<!--{/if}-->
			<!--{else}-->
				<input<!--{if $field.readonly}--> readonly<!--{/if}-->  type="text" value="<!--{$value}-->" name="_input_<!--{$name}-->"/>
			<!--{/if}-->
			</td>
		<!--{else}-->
			<input type="hidden" value="<!--{$value|escape}-->" name="_input_<!--{$name}-->" />
		<!--{/if}-->
	</tr>
	<!--{/foreach}-->
</table>
<!--{*debug*}-->
<!--{* pa value=$smarty.post *}-->
</div>
</form>