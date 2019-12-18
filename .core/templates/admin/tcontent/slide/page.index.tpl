<!--{if !isset($include_self)}-->
<!--{assign_html}-->
<!--{include file="common:page.main_custom.tpl"
    content_tpl = $ekv_resource_name
    include_self = 1
    }-->
    <!--{inner_use do_action='exit'}-->
<!--{/if}-->

<h2><!--{$pageTitle}--></h2>

<ul class="nulled">
<!--{foreach from=$lang_list item='lval'}-->
    <li>
        <a href="<!--{$lval.href}-->"><!--{$lval.name}--></a>
    </li>
<!--{/foreach}-->
</ul>