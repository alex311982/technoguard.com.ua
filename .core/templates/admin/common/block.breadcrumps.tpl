<!--{if !isset($DISABLE_BREADCRUMPS)}-->

<div>
<!--{if $smarty.session.user->id > 0 && $smarty.session.user->view == $smarty.const.SITE_BACKEND && !empty($breadcrumps)}-->
	<div class="breadcrump">
		&raquo;
		<!--{foreach name=breadcrump from=$breadcrumps item=item}-->
			<a href="<!--{compose_url_backend url=$item.href}-->"><!--{$item.title}--></a>
			<!--{if !$smarty.foreach.breadcrump.last}-->&raquo;<!--{/if}-->
		<!--{/foreach}-->
	</div>
<!--{/if}-->
<!--{if count($result.pagination.pages) > 1}-->
	<div class="pagination" >
		<!--{foreach from=$result.pagination.pages item=page}-->
			<!--{if $page.current == true}-->
				<span class="current"><!--{$page.number}--></span>
			<!--{else}-->
				<a href="<!--{compose_url_backend url="/`$smarty.const.SITE_BACKEND`/`$_module`/`$_entity`/`$_action`?page=`$page.number`&`$redirect_vars`"}-->">
					<!--{$page.number}-->
				</a>
			<!--{/if}-->
		<!--{/foreach}-->
	</div>
<!--{/if}-->
</div>
<div class="clear"></div>

<!--{/if}-->