<!--{if $custom_toolbar}-->
	<!--{include file=$custom_toolbar}-->
<!--{else}-->
	<!--{include file="common:toolbar_edit.tpl"}-->
<!--{/if}-->

<!--{if isset($tpl_undertoolbar)}-->
    <!--{include file=$tpl_undertoolbar}-->
    <!--{if isset($tpl_undertoolbar1)}-->
        <!-- tpl_undertoolbar -->
        <!--{include file=$tpl_undertoolbar1}-->
        <!-- /tpl_undertoolbar/ -->
    <!--{/if}-->
<!--{/if}-->