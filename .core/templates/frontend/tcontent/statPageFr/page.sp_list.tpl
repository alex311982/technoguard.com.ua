<!--{if !isset($include_self)}-->
    <!--{assign_html title = "Статичні сторінки"}-->
    <!--{include file="common:page.main.tpl"
        content_tpl=$ekv_resource_name
        include_self = 1
        }-->
        <!--{inner_use do_action='exit'}-->
<!--{/if}-->

<!--{if !empty($err_front)}-->
    <h3 class="centered"><!--{$err_front}--></h3>
<!--{else}-->

    <!--{if $tag_mode}-->
        <!--{include file="common:tcontent/statPageFr/content.sp_list.tpl"}-->
    <!--{else}-->
        <!--{*<!--{include file="common:news/newsFr/content.news_list_groupped.tpl"}-->*}-->
    <!--{/if}-->

<!--{/if}-->