<!--{if
    $smarty.session.user->id > 0
    && $smarty.session.user->view == $smarty.const.SITE_BACKEND
}-->
<!--{* pa a=$menu*}-->
<div class="menu" id="menu">

<!--{if $ADMIN_ROLE == "ROLE_SUPER"}-->
    <!--{retrieve_backend_menu}-->
	<!--{foreach from=$menu item=mitem}-->
		<div >
            <a onMouseOver="ShowSubmenu(this);" onMouseOut="HideMenu()"
                <!--{if !empty($mitem.module) && !empty($mitem.entity) && !empty($mitem.action)}-->
                    href="<!--{compose_url_backend url="/`$smarty.const.SITE_BACKEND`/`$mitem.module`/`$mitem.entity`/`$mitem.action`"}-->"
                <!--{else}-->
                    href="javascript:void(0)"
                <!--{/if}-->
                <!--{if !empty($mitem.subitems)}--> class="sub"<!--{/if}-->
            >
                <!--{$mitem.title|truncate:"40"}-->
            </a>
            <!--{if !empty($mitem.subitems)}-->
            <div class="subitems" onMouseOut="Hide(this)" onMouseOver="Hold(this)">
                <!--{foreach from=$mitem.subitems item=sitem}-->
                    <a href="<!--{compose_url_backend url="/`$smarty.const.SITE_BACKEND`/`$sitem.module`/`$sitem.entity`/`$sitem.action`"}-->">
                        <!--{$sitem.title}-->
                    </a>
                <!--{/foreach}-->
            </div>
            <!--{/if}-->
		</div>
	<!--{/foreach}-->

<!--{elseif $ADMIN_ROLE == "ROLE_MODER"}-->
    <!-- = = = = = = [ ROLE_MODER ] = = = = = = -->
    <div>
        <a href="<!--{compose_url_backend url="/admin/scheduleMod/tpl/channel_list"}-->">Шаблони</a>
    </div>
    <div>
        <a href="<!--{compose_url_backend url="/admin/scheduleMod/grid/channel_list"}-->">Сітка</a>
    </div>

    <div>
        <a href="<!--{compose_url_backend url="/admin/bank/prog/base_browse"}-->">Банк. Програми</a>
    </div>
    <div>
        <a href="<!--{compose_url_backend url="/admin/bank/presenter/base_browse"}-->">Банк. Ведучі</a>
    </div>

    <div>
        <a href="<!--{compose_url_backend url="/admin/news/news/news_list"}-->">Новини</a>
    </div>

    <div>
        <a href="<!--{compose_url_backend url="/admin/tcontent/statPage/sp_list"}-->">Сторінки</a>
    </div>

    <div>
        <a href="<!--{compose_url_backend url="/admin/base/menuFrontend/index"}-->">Меню</a>
    </div>

<!--{*
    <!--{if !empty($newsPermissions) && $newsPermissions->isSuperEditor}-->
        <div>
            <a href="<!--{compose_url_backend url="/admin/news/news/transl_browse"}-->">Новини. Непідтв. переклади</a>
        </div>
    <!--{/if}-->
 *}-->
<!--{/if}-->


<div class="last" >
	<a onMouseOver = "ShowSubmenu(this);" onMouseOut="HideSubmenu(this)" href="<!--{compose_url_backend url="/admin/base/user/logout"}-->">
		<!--{$smarty.const.MENU_LOGOUT}-->
	</a>
</div>
</div> <!-- id="menu" end -->
<div class="clear"></div>
<!--{/if}-->