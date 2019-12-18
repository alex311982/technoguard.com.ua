<form id="main_form" action="<!--{compose_url_backend url="/`$_view`/`$_module`/`$_entity`/`$_action`"}-->" method="post">
	<!--{if $custom_toolbar}-->
		<!--{include file=$custom_toolbar}-->
	<!--{else}-->
		<!--{include file="common:toolbar_details.tpl"}-->
	<!--{/if}-->

	<div class="content" id="main-scroll">
		<table cellspacing="0" cellpadding="2" border="0" class="grid">
			<!--{foreach from=$result.fields key=name item=field}-->
			<tr<!--{cycle values=', class="bg"'}-->>
				<!--{if $field.view == true || $field.details == true}-->
					<td class="title"><!--{$field.title}--></td><td class="req">&nbsp;</td><td><!--{strip}-->
					<!--{assign var=value value=$result.record.$name.value}-->
						<!--{if $field.type == "select"}--><!--{$field.options.$value}-->
						<!--{elseif $field.type == "password"}-->***********
						<!--{elseif $field.type == "textarea"}--><!--{$value|nl2br}-->
						<!--{elseif $field.type == "date"}--><!--{$value|date_format:"`$result.record.$name.dateformat`"}-->
						<!--{elseif $field.type == 'datetime'}-->
							<!--{$value|date_format:$smarty.const.DEFAULT_DATE_FORMAT}--> | <!--{$value|date_format:$smarty.const.DEFAULT_TIME_FORMAT}-->
						<!--{elseif $field.type == "image"}--><img src="-imgs/<!--{$value}-->">
						<!--{elseif $field.image_instead_path }--><img src="-imgs/<!--{$value}-->">
						<!--{else}--><!--{$value|escape}--><!--{/if}-->
					<!--{/strip}--></td>
				<!--{/if}-->		
			</tr>
			<!--{/foreach}-->	
		</table>
	</div>
</form>
