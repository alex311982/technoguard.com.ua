<!--{*<!--{pa param1=$this_field_full}-->*}-->

<!--{* connect prog presenters for bank to tpl item (multi presenters) *}-->

<!--{*
<style type="text/css">
.presenter-conn input[type="checkbox"]{
    width: auto;
}
.presenter-conn li{
    float: left;
    padding-right: 7px;
}
</style>
<!-- shipments -->
<ul class="nulled presenter-conn">
<!--{foreach from=$this_field_full.data.presenter_opts item='presenter_item'}-->
    <li>
        <input type="checkbox" value="<!--{$presenter_item.id}-->" id="<!--{$presenter_item.js_unique_id}-->"
           name="<!--{$this_field_full.input_names.presenter}-->"
           <!--{$presenter_item.checked_str}-->
        />
        <label for="<!--{$presenter_item.js_unique_id}-->"><!--{$presenter_item.title|escape}--></label>
    </li>
<!--{/foreach}-->
</ul>

<div class="spacer"></div>
<br/><br/>
 *}-->

<select name="<!--{$this_field_full.input_names.presenter}-->" id="idPresenterConn"
        multiple="multiple" data-placeholder=<!--{$smarty.const.DEFAULT_SELECT_NAME}-->>
<!--{foreach from=$this_field_full.data.presenter_opts item='presenter_item'}-->
    <option
            value="<!--{$presenter_item.id|escape}-->"
            <!--{$presenter_item.selected_str}-->
        ><!--{$presenter_item.title|escape}--></option>
<!--{/foreach}-->
</select>

