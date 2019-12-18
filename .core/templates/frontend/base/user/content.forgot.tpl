<div class="content-middle2 user-container standart-text" >
    <h3 class="h3-colored">Восстановление пароля</h3>

	<form action="/user/forgot.html" method="post">
	<input type="hidden" name="_input_is_form_submited" />
	<!--{if !isset($finished)}-->
		<p style="width: 370px; padding-bottom:5px; font-style:italic;">
			Введите Ваш электронный адрес, для того чтобы мы могли выслать Вам новый пароль
		</p>

        <ul class="frm frm-user">
            <!-- email -->
            <li>
                <!--{if isset($err.email)}-->
                    <div class="err"><!--{$err.email}--></div>
                <!--{/if}-->
                <label>Email*</label>
                <input name="_input_email" value="<!--{$vars.email|escape}-->" class="text" type="text"/>
            </li>

            <li class="centered">
                <input type="submit" class="btn-fancy" value="отправить" />
            </li>
        </ul>
    <!--{else}-->
    	<p>Новый пароль был выслан на ваш email адрес</p>
    <!--{/if}-->
	</form>
							
</div>