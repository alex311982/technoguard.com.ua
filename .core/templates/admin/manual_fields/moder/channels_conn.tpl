<style type="text/css">
.channels-conn input[type="checkbox"]{
    width: auto;
}
.channels-conn li{
    float: left;
    padding-right: 7px;
}
</style>

<!-- shipments -->
<ul class="nulled channels-conn">
<!--{foreach from=$this_field_full.data.main_opts item='channels_conn_item'}-->
    <li>
        <input type="checkbox" value="<!--{$channels_conn_item.id}-->" id="<!--{$channels_conn_item.js_unique_id}-->"
           name="<!--{$this_field_full.input_names.main_input}-->"
           <!--{$channels_conn_item.checked_str}-->
        />
        <label for="<!--{$channels_conn_item.js_unique_id}-->"><!--{$channels_conn_item.title|escape}--></label>
    </li>
<!--{/foreach}-->
</ul>