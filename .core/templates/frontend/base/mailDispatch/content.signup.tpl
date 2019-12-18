<div class="mcol">

    <h2>Підписатися на розсилку</h2>
    <hr />
	<form action="/mail-dispatch/signup.html" method="post" id="form-validate" name="frm_signup">
	<input type="hidden" name="_input_is_form_submited" />
	<!--{if !isset($finished)}-->
	    <table cellpadding="0" cellspacing="0" class="tbl-form">
			<!-- email -->
	        <!--{if $err.email}-->
	        <tr>
	            <td colspan="3" class="form-error"><!--{$err.email}--></td>
	        </tr>
			<!--{/if}-->                    
	        <tr>
	            <td class="title">E-mail: </td>
	            <td class="required">*</td>
	            <td class="inputField inputText">
	                <input type="text" value="<!--{$vars.email|escape}-->" name="_input_email"  />
	            </td>
	        </tr>
	        
			<!-- name -->
	        <!--{if $err.name}-->
	        <tr>
	            <td colspan="3" class="form-error"><!--{$err.name}--></td>
	        </tr>
			<!--{/if}-->
	        <tr>
	            <td class="title">Ім'я: </td>
	            <td class="required">*</td>
	            <td class="inputField inputText">
	                <input type="text" value="<!--{$vars.name|escape}-->" name="_input_name"  />
	            </td>
	        </tr>

			<!-- org -->
	        <!--{if $err.organisation}-->
	        <tr>
	            <td colspan="3" class="form-error"><!--{$err.organisation}--></td>
	        </tr>
			<!--{/if}-->
	        <tr>
	            <td class="title">Організація: </td>
	            <td class="required">*</td>
	            <td class="inputField inputText">
	                <input type="text" value="<!--{$vars.organisation|escape}-->" name="_input_organisation"  />
	            </td>
	        </tr>

			<!-- captcha -->
	        <tr>
	            <td class="title"></td>
	            <td class="required"></td>
	            <td class="inputField inputText">
	                <img src="/captcha.html" alt="captcha">
	            </td>
	        </tr>
	        <!--{if $err.captcha}-->
	        <tr>
	            <td colspan="3" class="form-error"><!--{$err.captcha}--></td>
	        </tr>
			<!--{/if}-->
	        <tr>
	            <td class="title">Захисний код: </td>
	            <td class="required">*</td>
	            <td class="inputField inputText">
	                <input type="text" value="<!--{$vars.captcha|escape}-->" name="_input_captcha"  />
	            </td>
	        </tr>

	        <!-- submit -->
	        <tr>
	            <td colspan="3" align="center">
                    <button class="button101" name="_input_is_form_submited" type="submit"><u>підписатися</u></button>
	            </td>
	        </tr>   
	    </table>
    <!--{else}-->
    	<br>
    	<p>
			Ваша email-адреса була занесена до списку розсилки. 
		</p>
    <!--{/if}-->
	</form>

</div>