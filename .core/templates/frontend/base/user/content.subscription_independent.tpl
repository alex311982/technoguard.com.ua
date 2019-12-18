<h3 class="block-name">подписка на рассылку</h3>
<div class="content-middle3" style="padding-left:120px;">
<!--{if $show_success_greeting}-->
	<h3>Для завершения процесса подписки необходимо перейти по ссылке, высланной на указанный Вами e-mail!</h3>
<!--{else}-->

	<form action="" method="POST" enctype="multipart/form-data">

    <ul class="frm frm-user">
        <!-- fio -->
        <li>
            <!--{if isset($err.fio)}-->
                <div class="err"><!--{$err.fio}--></div>
            <!--{/if}-->
            <label>ФИО*</label>
            <input name="_input_fio" value="<!--{$vars.fio|escape}-->" class="text" type="text"/>
        </li>

        <!-- email -->
        <li>
            <!--{if isset($err.email)}-->
                <div class="err"><!--{$err.email}--></div>
            <!--{/if}-->
            <label>E-mail*</label>
            <input name="_input_email" value="<!--{$vars.email|escape}-->" class="text" type="text"/>
        </li>

        <!-- user_country_id -->
        <li>
            <!--{if isset($err.user_country_id)}-->
                <div class="err"><!--{$err.user_country_id}--></div>
            <!--{/if}-->
            <label>Страна*</label>
            <select id="idUserCountry" name="_input_user_country_id" class="text">
                <!--{html_options options = $country_options selected=$vars.user_country_id}-->
            </select>
        </li>

        <!-- user_city_id -->
        <li>
            <!--{if isset($err.user_city_id)}-->
                <div class="err"><!--{$err.user_city_id}--></div>
            <!--{/if}-->
            <label>Город*</label>
            <select id="idUserCityIdDdl" name="_input_user_city_id" class="text">
                <option>--выбрать--</option>
                <!--{html_options options = $city_options selected=$vars.user_city_id}-->
            </select>
            <input id="idUserCityText" name="_input_user_city" value="<!--{$vars.user_city}-->" class="text" type="text"/>
        </li>

        <!-- city js -->
        <!--{include file="common:base/user/js.city.tpl"}-->
        <!-- / city js / -->

        <!-- captcha -->
        <li class="captcha">
            <div class="fields">
                <!--{if isset($err.captcha)}-->
                    <div class="err"><!--{$err.captcha}--></div>
                <!--{/if}-->
                <label>Защитный код*</label>
                <input name="_input_captcha" value="<!--{$vars.captcha|escape}-->" class="text" type="text"/>
            </div>
            <div class="img">
                <img src="/captcha.html">
            </div>
            <div class="spacer"></div>
        </li>

        <li class="centered">
            <input type="hidden" name="_input_is_form_submited">
            <!--
            <button type="submit" class="btn-common">отправить</button>
             -->
            <input type="submit" value="отправить" class="btn-fancy" />
        </li>
    </ul>
	</form>
<!--{/if}-->
</div>