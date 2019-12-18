<h1 class="page-title">Сторінки за тегом «<!--{$tag}-->»</h1>

<!--{if !empty($result.records)}-->

<ul class="news nulled">
<!--{foreach from=$result.records item='sp_val'}-->
    <!--{assign var="sp_title" value=$sp_val.sp_title|escape}-->
    <li>
        <!--{*
        <!--{if !empty($sp_val.url_img)}-->
            <a href="<!--{$sp_val.url_preview}-->"
                ><img src="<!--{$sp_val.url_img}-->" alt="<!--{$sp_title}-->" class="img"/></a>
        <!--{/if}-->
         *}-->
        <a href="<!--{$sp_val.url_preview}-->" class="<!--{$sp_val.css_classes}-->"><!--{$sp_title}--></a>

        <div>
            <!--{$sp_val.text_short}-->
        </div>
        <div class="spacer"></div>
    </li>
<!--{/foreach}-->
</ul>

<!--{include file="common:block.pagination.tpl"}-->

<!--{else}-->
    <h4>Даних за цей день не знайдено</h4>
<!--{/if}-->

<style type="text/css">
    .news li{
        margin-bottom: 10px;
    }
    .news .img{
        float: left;
        margin: 0 10px 10px 0;
    }


</style>

