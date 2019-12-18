<div>
<!--{assign var="toolbar_action_prefix" value=""}-->
<table class="toolbar" cellspacing="0" cellpadding="0" border="0">
	<tr>
		<td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_NEW}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->new"/></td>
		<td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_EDIT}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->edit"  restriction="single" /></td>
		<td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_DELETE}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->delete" restriction="multi" confirm="delete" /></td>

		<!-- send button -->
		<td>
			<input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="Отправить" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->send"  restriction="single" />
		</td>

		<!--{if !$_disable_filter}-->
			<td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_FILTER}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->browse" restriction="none" /></td>
		<!--{/if}-->


	</tr>
</table>
</div>