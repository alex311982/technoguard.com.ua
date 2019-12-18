<div>
<!--{assign var="toolbar_action_prefix" value=""}-->
<table class="toolbar" cellspacing="0" cellpadding="0" border="0">
	<tr>
		<td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_SAVE}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->save"/></td>
<!--{*
        <!--{if isset($edit_CAN_BE_SEND_BUTTON_DISPLAYED) && $edit_CAN_BE_SEND_BUTTON_DISPLAYED == true }-->
		    <td><input _get="do_action=send" type="button" value="Отправить" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->save"/></td>
		<!--{/if}-->
*}-->
        <td><input _get="do_action=put2queue" type="button" value="В очередь" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->save"/></td>
	</tr>
</table>
</div>