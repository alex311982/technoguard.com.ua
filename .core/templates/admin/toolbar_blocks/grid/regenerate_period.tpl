<td>
    <input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}-->
        type="button" value="регенерувати"
        module="<!--{$_module}-->"
        entity="<!--{$_entity}-->"
        action="period_regenerate"
        <!--{*restriction="single"*}-->
        <!--{*confirm="dispatch_perform"*}-->
    />

    <input <!--{if !empty($redirect_vars)}-->_get="<!--{$redirect_vars}-->"<!--{/if}-->
        type="button" value="email-звіт"
        module="<!--{$_module}-->"
        entity="<!--{$_entity}-->"
        action="period_email_report"
        <!--{*restriction="single"*}-->
        <!--{*confirm="dispatch_perform"*}-->
    />
</td>