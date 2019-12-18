<!--{if !empty($errors)}-->
	<div class="box">
		<div class="errors">
			<p><!--{$smarty.const.TEXTBOX_ERROR}--></p>
			<ul>
			<!--{foreach from=$errors item=error}-->
				<li><!--{$error}--></li>
			<!--{/foreach}-->
			</ul>
		</div>
	</div>
<!--{/if}-->
<!--{if !empty($messages)}-->
	<div class="box">
		<div class="messages">
			<p><!--{$smarty.const.TEXTBOX_MESSAGE}--></p>
			<ul>
			<!--{foreach from=$messages item=message}-->
				<li><!--{$message}--></li>
			<!--{/foreach}-->
			</ul>
		</div>
	</div>
<!--{/if}-->