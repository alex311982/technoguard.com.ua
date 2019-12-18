<!--{*
<td>
    <input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}-->
        type="button" value="Отправить" 
        module="<!--{$_module}-->"
        entity="<!--{$_entity}-->"
        action="<!--{$toolbar_action_prefix}-->send"
        restriction="single"
        confirm="dispatch_perform"
    />
</td>
 *}-->
<td>
    <input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}-->
        type="button" value="В очередь" 
        module="<!--{$_module}-->"
        entity="<!--{$_entity}-->"
        action="<!--{$toolbar_action_prefix}-->put2queue"
        restriction="single"
        confirm="dispatch_perform"
    />
</td>