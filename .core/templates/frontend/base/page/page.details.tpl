<!--{assign var="__DISPLAY_VARIANT" value="middle_big"}-->
<!--{strip}-->
<!--{assign_html 
		title="`$page.title` `$smarty.const.SITE_TITLE_DELIMITER`  `$smarty.const.SITE_TITLE_FRONTEND` "|escape 
		direct_title=true
}-->
<!--{if $page.record.url.value == 'index'}-->
	<!--{include file="common:page.home.tpl"  content_tpl="common:base/page/content.details.tpl"}-->
<!--{else}-->
	<!--{include file="common:page.main.tpl" content_tpl="common:base/page/content.details.tpl"}-->
<!--{/if}-->
<!--{/strip}-->