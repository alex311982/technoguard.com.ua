<!--{assign var="input_name" value="_input_`$this_field_name`"}-->

<!--{if isset($man_departments_formated)}-->
<!--{foreach from=$man_departments_formated item=mdf_val }-->
<div style="float:left; margin-right:5px;">
    <input type="checkbox" style="width:auto;" name="<!--{$this_field_full.input_name}-->" id="<!--{$mdf_val.js_unique_id}-->"
        value="<!--{$mdf_val.id}-->" <!--{$mdf_val.checked_str}--> />
    <label for="<!--{$mdf_val.js_unique_id}-->"><!--{$mdf_val.title|escape}--></label>
</div>
<!--{/foreach}-->
<!--{/if}-->