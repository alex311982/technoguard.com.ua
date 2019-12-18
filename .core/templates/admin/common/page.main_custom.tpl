<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
	"DTD/xhtml1-transitional.dtd">
<html>
<!--{include file="head.tpl"}-->
<body>
	<!--{* toolbar_action_prefix *}-->
	<!--{if !isset($toolbar_action_prefix) && isset($smarty.request.toolbar_action_prefix)}-->
		<!--{assign var="toolbar_action_prefix" value=$smarty.request.toolbar_action_prefix}-->
	<!--{/if}-->
	
	<!--{*<div id="loading"><div id="txt-loading"><img align="left" src="/_images/loading.gif"><div>Please Wait. Loading...</div></div></div>*}-->
	<script type="text/javascript">
        Loading();Loaded();
        $(function(){
            $(window).resize(function(){
                initLayout();
            })
        })
    </script>
	<!--{include file="common:header.tpl"}-->

    <!-- content -->
    <div class="main"  id="main-scroll">
        <!--{include file="common:block.errors.tpl"}-->
	    <!--{include file="common:content.main.tpl"}-->
    </div>
    <!-- /content/ -->

	<!--{include file="common:footer.tpl"}-->
	<script type="text/javascript">
		initLayout();
		initForm();
		updateToolbar(); 
	</script>

	<script type="text/javascript">
		Loaded();
	</script>
</body>
</html>