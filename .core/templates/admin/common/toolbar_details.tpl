<div>
<table class="toolbar" cellspacing="0" cellpadding="0" border="0">
	<tr>
		<td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_NEW}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->new"/></td>
		<td><input <!--{if !empty($redirect_vars) || !empty($smarty.get.id)}-->_get="<!--{if !empty($smarty.get.id)}-->id=<!--{$smarty.get.id}-->&<!--{/if}--><!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_EDIT}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->edit" /></td>
		<td><input <!--{if !empty($redirect_vars) || !empty($smarty.get.id)}-->_get="<!--{if !empty($smarty.get.id)}-->id=<!--{$smarty.get.id}-->&<!--{/if}--><!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_DELETE}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->delete" confirm="delete" /></td>
	</tr>
</table>
</div>