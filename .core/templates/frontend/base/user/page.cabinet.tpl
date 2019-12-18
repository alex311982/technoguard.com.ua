<!--{if $no_page_file != 1}-->
    <!--{assign var="_DISPLAY_TYPE" value="3columns"}-->
    <!--{assign_html title = "Личный кабинет" }-->
    <!--{include file="common:page.main.tpl" content_tpl_direct_html="common:base/user/page.cabinet.tpl"}-->
<!--{else}-->

<div class="standart-text centered content-middle3">
    <h3 class="h3-colored">Личный кабинет</h3>

    <ul class="nulled cabinet">
    <li>
        <a href="/user/orders.html">заказы</a>
    </li>
    <li>
        <a href="/user/profile.html">редактирование данных профиля</a>
    </li>
    <li>
        <a href="/user/cabinet.html?cmd=subscriptions">управление подписками</a>
    </li>
    <li>
        <a href="/user/profile.html?pwd_change=1">смена пароля</a>
    </li>
    </ul>

</div>
<style>
ul.cabinet{
    text-align:left;
}
ul.cabinet li{
    padding:5px 0;
}
ul.cabinet a{
    font-size:16px;
}
</style>

<!--{/if}-->
