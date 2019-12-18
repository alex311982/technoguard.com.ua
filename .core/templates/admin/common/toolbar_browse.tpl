<div>
<table class="toolbar" cellspacing="0" cellpadding="0" border="0">
<tr>
    <!-- create -->
    <!--{if !isset($toolbar_create_disable)}-->
        <td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_NEW}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->new"/></td>
    <!--{/if}-->

    <!-- upload multiple -->
    <!--{if isset($show_multi_upload_btn)}-->
    <td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="Multiple Upload" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->multi_upload_new"/></td>
    <!--{/if}-->

    <!-- details -->
    <!--{if isset($toolbar_details_enable)}-->
    <td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_DETAILS}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->details" restriction="single" /></td>
    <!--{/if}-->

    <!-- edit -->
    <!--{if !isset($toolbar_edit_disable) || $toolbar_edit_disable != 1}-->
        <td>
            <input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_EDIT}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->edit"  restriction="single" />
        </td>
    <!--{/if}-->

    <!-- delete -->
    <!--{if !isset($toolbar_delete_disable) || $toolbar_delete_disable == 0}-->
        <td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_DELETE}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->delete" restriction="multi" confirm="delete" /></td>
    <!--{/if}-->

    <!--{if $show_approve_btn}-->
        <td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_APPROVE}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->approve" restriction="multi"  /></td>
    <!--{/if}-->

    <!--{if isset($manual_toolbar_block)}-->
        <!--{include file="common:toolbar_blocks/`$manual_toolbar_block`"}-->
    <!--{/if}-->

    <!--{if $show_moving_btns}-->
    <td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->&direction=up"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_TOP}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->rubricator_exchange"  restriction="single" /></td>
    <td><input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->&direction=down"<!--{/if}--> type="button" value="<!--{$smarty.const.TOOLBAR_DOWN}-->" module="<!--{$_module}-->" entity="<!--{$_entity}-->" action="<!--{$toolbar_action_prefix}-->rubricator_exchange"  restriction="single" /></td>
    <!--{/if}-->

    <!--{if !$_disable_filter}-->
        <!--{if !isset($filter_action)}-->
            <!--{assign var="filter_action" value="`$toolbar_action_prefix`browse"}-->
        <!--{/if}-->
        <td>
            <input
                <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}-->
                type="button"
                value="<!--{$smarty.const.TOOLBAR_FILTER}-->"
                module="<!--{$_module}-->" entity="<!--{$_entity}-->"
                <!--{*action="<!--{$toolbar_action_prefix}-->browse"*}-->
                action="<!--{$filter_action}-->"
                restriction="none" />
        </td>
    <!--{/if}-->


</tr>
</table>
</div>