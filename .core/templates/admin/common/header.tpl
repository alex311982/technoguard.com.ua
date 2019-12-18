<!--{add_debug_point value="template_header_start"}-->
<div class="box" >
    <!--{*
	<div class="title">
	    <a href="http://10x15.ua" target="_blank"><img src="/_images/logo.gif" alt="лого компании" /></a>
    </div>
     *}-->
	<!--{if $smarty.session.user->id > 0 && $smarty.session.user->view == $smarty.const.SITE_BACKEND}-->
		<div class="copy">Користувач: <!--{$smarty.session.user->login}-->
		</div>
	<!--{/if}-->
	<div class="clear"></div>
	<div class="header" >
		<!--{include file="common:block.menu.tpl"}-->
		<!--{include file="common:block.breadcrumps.tpl"}-->
	</div>
</div>
<!--{add_debug_point value="template_header_end"}-->