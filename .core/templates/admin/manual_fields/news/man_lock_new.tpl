<!--{assign var="current_news_transl_id" value=$result.records.$id.id_transl.value}-->
<!--{if $news.$current_news_transl_id.is_locked_in_admin}-->
так
<!--{else}-->
ні
<!--{/if}-->