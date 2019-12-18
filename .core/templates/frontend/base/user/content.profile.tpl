<script type="text/javascript" src="/-js/jquery.maskedinput.js"></script>
<script type="text/javascript" src="/-js/_misc/phone.js"></script>

<div class="user-container content-middle2">
<!--{if $show_success_greeting}-->
	<h3>Изменения сохранены.</h3>
    <br/>
    <a href="/user/cabinet.html">Вернуться в личный кабинет</a>
<!--{else}-->
    <h3 class="h3-colored">Изменение данных</h3>

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
<!--{*
        <!-- password -->
        <li>
            <!--{if isset($err.password)}-->
                <div class="err"><!--{$err.password}--></div>
            <!--{/if}-->
            <label>Пароль*</label>
            <input name="_input_password" value="<!--{$vars.password}-->" class="text" type="password"/>
        </li>
 *}-->

        <!-- phone -->
		<script type="text/javascript" src="/-js/jquery.maskedinput.js"></script>
		<script type="text/javascript" src="/-js/_misc/phone.js"></script>
        <li class="phone">
            <!--{if isset($err.phone)}-->
                <div class="err"><!--{$err.phone}--></div>
            <!--{/if}-->
			<label>Моб. телефон*</label>
			<div class="phone-parts">
				<select name="_input_phonecode" class="phone-code">
					<!--{foreach from=$phonecodes key=phonecode item=phoneopts}-->
					<option value="<!--{$phonecode}-->"<!--{if $vars.phonecode == $phonecode}--> selected="true"<!--{/if}-->>
						<!--{$phoneopts.title}-->
					</option>
					<!--{/foreach}-->
					<option value=""<!--{if !$vars.phoneCodeFromList}--> selected="true"<!--{/if}-->>другая страна</option>
				</select>
				<input name="_input_phonecodecustom" value="<!--{$vars.phonecode}-->" class="text" id="idEkvPhoneCodeCustom" type="text"/>
				<input name="_input_phonenum" id="idEkvPhone" value="<!--{$vars.phonenum}-->" class="text" type="text"/>
			</div>
			<div id="idPhoneRes"><img style="display:none;" src="/-images/ekv_phone_ok.png" /></div>
        </li>

		<script type="text/javascript">
			var v = ekvPhoneValidator();
			<!--{if !$vars.phoneCodeFromList}-->
			v.showCustomCode();
			<!--{else}-->
			v.hideCustomCode();
			<!--{/if}-->
		</script>

        <!-- user_country_id -->
        <li>
            <!--{if isset($err.user_country_id)}-->
                <div class="err"><!--{$err.user_country_id}--></div>
            <!--{/if}-->
            <label>Страна</label>
            <select id="idUserCountry" name="_input_user_country_id" class="text">
                <!--{html_options options = $country_options selected=$vars.user_country_id}-->
            </select>
        </li>

        <!-- user_city_id -->
        <li>
            <!--{if isset($err.user_city_id)}-->
                <div class="err"><!--{$err.user_city_id}--></div>
            <!--{/if}-->
            <label>Город</label>
            <select id="idUserCityIdDdl" name="_input_user_city_id" class="text">
                <option>--выбрать--</option>
                <!--{html_options options = $city_options selected=$vars.user_city_id}-->
            </select>
            <input id="idUserCityText" name="_input_user_city" value="<!--{$vars.user_city}-->" class="text" type="text"/>
        </li>
        
        <!--{*
        <script type="text/javascript">
        $(function(){
            var ekvCountryUkr = <!--{$country_ukr}-->;
            $("#idUserCountry").change(function(){
                var countrySelected = $(this).val();
                //--- ukr ---//
                if(countrySelected == ekvCountryUkr){
                    $("#idUserCityIdDdl").show();
                    $("#idUserCityText").hide();
                }else{
                    $("#idUserCityIdDdl").hide();
                    $("#idUserCityText").show();
                }
            }).change();
        });
        </script>
         *}-->
        <!-- city js -->
        <!--{include file="common:base/user/js.city.tpl"}-->
        <!-- / city js / -->

        <li class="centered">
            <input type="hidden" name="_input_is_form_submited">
            <!--
            <button type="submit" class="btn-common">отправить</button>
             -->
            <input type="submit" class="btn-fancy" value="отправить" />
        </li>

    </ul>
	</form>
<!--{/if}-->
</div>
