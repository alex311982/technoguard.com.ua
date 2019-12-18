<!--edit-->
<form id="main_form" action="" method="post" enctype="multipart/form-data">

<input type="hidden" name="MAX_FILE_SIZE" value="50000000">

<!--{*
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
*}-->

<!--{include file="common:common/blocks/b.form_top.tpl"}-->

<div class="main"  id="main-scroll">
<!--{include file="common:block.errors.tpl"}-->

<!--{include file="common:common/blocks/b.edit_table.tpl"}-->

<!--{* save browse url here and pass it through edit-save actions to provide redirect to specific browse page after saving *}-->
<!--{if isset($smarty.request.ekv_http_referer)}-->
    <!--{assign var="ekv_http_referer" value=$smarty.request.ekv_http_referer}-->
<!--{/if}-->
<input type="hidden" name="ekv_http_referer" value="<!--{$ekv_http_referer|escape}-->" />
</div>
</form>
