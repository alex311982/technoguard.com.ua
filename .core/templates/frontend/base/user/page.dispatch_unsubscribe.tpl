<!--{if $no_page_file != 1}-->
    <!--{assign var="_DISPLAY_TYPE" value="3columns"}-->
    <!--{assign_html title = "Отказ от подписки" }-->
    <!--{include file="common:page.main.tpl" content_tpl_direct_html="common:base/user/page.dispatch_unsubscribe.tpl"}-->
<!--{else}-->

<div class="standart-text centered">
    <!--{if $result == "ok"}-->
        <h3>Ваш профиль успешно удален из списка рассылки</h3>
    <!--{elseif $result == "user_failed"}-->
        <h3>Неправильный идентификатор пользователя</h3>
    <!--{/if}-->
</div>

<!--{/if}-->
