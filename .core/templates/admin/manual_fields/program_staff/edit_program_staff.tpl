<!--{assign var="input_name" value="_input_`$this_field_name`"}-->

<!--{if isset($man_staff_formated)}-->
<!--{foreach from=$man_staff_formated item=msf_val }-->
<div style="float:left; margin-right:5px;">
    <input type="checkbox" style="width:auto;" name="<!--{$this_field_full.input_name}-->" id="<!--{$msf_val.js_unique_id}-->"
        value="<!--{$msf_val.id}-->" <!--{$msf_val.checked_str}--> />
    <label for="<!--{$msf_val.js_unique_id}-->"><!--{$msf_val.title|escape}--></label>
</div>
<!--{/foreach}-->
<!--{/if}-->