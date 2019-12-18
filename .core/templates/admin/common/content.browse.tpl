<!--{*debug*}-->
<form id="main_form" action="<!--{compose_url_backend url="/`$_view`/`$_module`/`$_entity`/`$_action`"}-->" method="post">
<!--{if $custom_toolbar}-->
	<!--{include file=$custom_toolbar}-->
<!--{elseif isset($__DISABLE_TOOLBAR)}-->

<!--{else}-->
	<!--{include file="common:toolbar_browse.tpl"}-->
<!--{/if}-->

<!--{if isset($tpl_undertoolbar)}-->
    <!--{include file=$tpl_undertoolbar}-->
    <!--{if isset($tpl_undertoolbar1)}-->
        <!--{include file=$tpl_undertoolbar1}-->
    <!--{/if}-->
<!--{/if}-->

<!--{include file="common:block.errors.tpl"}-->
<div class="main" id="main-scroll">
<table width="100%" cellspacing="0" cellpadding="0" border="0" class="grid">
    <!-- = = = = = = [ field names ] = = = = = = -->
	<tr>
    <th class="check">&nbsp;</th>
	<!--{foreach from=$result.fields item=field}-->
		<!--{if $field.browse == true}-->
			<th<!--{if $field.width}--> width="<!--{$field.width}-->"<!--{/if}-->><!--{$field.title}--></th>
		<!--{/if}-->
	<!--{/foreach}-->	
	</tr>

	<!-- = = = = = = [ filter inputs ] = = = = = = -->
	<!--{if !$_disable_filter}-->
    <tr class="filter">
        <td class="check">
            <input type="checkbox" id="idSelectAllCheckBoxes" onclick="allCheckBoxesItemClick(this);" />
        </td>
        <!--{foreach from=$result.fields key=field item=attrs}-->
        <!--{if isset($attrs.filter_disable)}-->
            <td>&nbsp;</td>
        <!--{elseif $attrs.browse == true}-->
            <td<!--{if $attrs.width}--> width="<!--{$attrs.width}-->"<!--{/if}-->>
                <!--{assign var=filter_name value="_filter_`$field`"}-->
                <!--{assign var="filter_value" value=$smarty.post.$filter_name|escape}-->


                <!--{if isset($attrs.filter_manual)}-->
                    <!--{include file=$attrs.filter_manual}-->
                <!--{elseif !isset($attrs.calculable)
                        || isset($attrs.calc_has_filter)
                    }-->
                    <!--{if 'select' == $attrs.type
                        && isset($attrs.options)
                        }-->
                        <select name="_filter_<!--{$field}-->"
                            <!--{if isset($attrs.element_width)}-->style="width:<!--{$attrs.element_width}-->"<!--{/if}--> />
                            <!--{html_options options=$attrs.options selected=$filter_value}-->
                        </select>
                    <!--{elseif $attrs.type == "checkbox"}-->
                        <!--{assign var="filter_local_name" value="_filter_`$field`"}-->
                        <select name="<!--{$filter_local_name}-->" />
                        <!--{if isset($attrs.options)}-->
                            <!--{html_options options=$attrs.options selected=$filter_value}-->
                        <!--{else}-->
                            <option value=""><!--{$smarty.const.DEFAULT_SELECT_NAME}--></option>
                            <option value="1" <!--{if $smarty.post.$filter_local_name == 1}-->selected<!--{/if}-->>так</option>
                            <option value="0" <!--{if $smarty.post.$filter_local_name === '0'}-->selected<!--{/if}-->>ні</option>
                        <!--{/if}-->
                        </select>
                    <!--{elseif 'fio' == $field}-->
		        <select name="_filter_<!--{$field}-->" id="fio-filter" />
                            <!--{html_options options=$fio_all.options selected=$filter_value}-->
                        </select>
                        <!--<input name="_filter_<!--{$field}-->" value="<!--{$filter_value}-->" />-->
                    <!--{elseif 'prog_name' == $field}-->
		        <select name="_filter_<!--{$field}-->" id="prog-filter" />
                            <!--{html_options options=$fio_all.options selected=$filter_value}-->
                        </select>
                        <!--<input name="_filter_<!--{$field}-->" value="<!--{$filter_value}-->" />-->
                    <!--{elseif 'text' == $attrs.type}-->
                        <input name="_filter_<!--{$field}-->" value="<!--{$filter_value}-->" />
                    <!--{elseif 'date' == $attrs.type}-->
                        &nbsp;
                    <!--{elseif 'datime' == $attrs.type}-->
                        &nbsp;
                    <!--{elseif 'datetime' == $attrs.type}-->
                        &nbsp;
                    <!--{elseif 'password' == $attrs.type}-->
                        &nbsp;
                    <!--{elseif 'textarea' == $attrs.type}-->
                        <input name="_filter_<!--{$field}-->" value="" />
                    <!--{elseif 'file' == $attrs.type || 'image' == $attrs.type}-->
                        &nbsp;
                    <!--{else}-->
                        <input name="_filter_<!--{$field}-->" value="<!--{$filter_value}-->" />
                    <!--{/if}-->
<!--{* 
                <!--{elseif isset($attrs.calc_has_filter)}-->
                    <select name="_filter_<!--{$field}-->" />
                        <!--{html_options options=$attrs.options selected=$filter_value}-->
                    </select>
                     *}-->
                <!--{else}-->
                    <!--{if !isset($attrs.filter_type)}-->
                        &nbsp;
                    <!--{else}-->
                        <input name="_filter_<!--{$field}-->" value="<!--{$filter_value}-->" />
                    <!--{/if}-->
                <!--{/if}-->
            </td>
        <!--{/if}-->
        <!--{/foreach}-->
    </tr>
	<!--{/if}-->

	<!-- = = = = = = [ content ] = = = = = = -->
	<!--{foreach from=$result.records key=id item=row}-->
	<!--{if $PAGE_TYPE != "orders_list"}-->
	 <tr<!--{cycle values=', class="bg"'}-->>
	<!--{elseif $PAGE_TYPE == "orders_list"}-->
	 <tr class="<!--{$row.calc_css_class.value}-->">
	<!--{/if}-->

    <!--{if $PAGE_TYPE == "orders_list" || $PAGE_TYPE == "booking_list"}-->
     <tr class="<!--{$row.calc_css_class.value}-->">
    <!--{elseif $PAGE_TYPE == "orders_list"}-->
     <tr<!--{cycle values=', class="bg"'}-->>
    <!--{/if}-->

		<!--{foreach from=$row item=col}-->
		<!--{if $col.name == 'id' || $col.PK == true }-->
		<td class="check">
		    <input type="checkbox" name="__service_checkboxes[]"
		        value="<!--{$col.value}-->" <!--{if $col.checked == true }-->checked<!--{/if}--> />
        </td>
		<!--{else}-->
			<!--{if $col.browse == true}-->
				<td
                    <!--{if $col.align}--> align="<!--{$col.align}-->"<!--{/if}-->
                    <!--{if isset($col.css_class)}-->class="<!--{$col.css_class}-->"<!--{/if}-->
                ><!--{strip}-->
                    <!--{* <!-- = = = = = = [ <a> ] = = = = = = --> *}-->
					<!--{if $col.href && !isset($col.manual_field)}-->
						<!--{eval var=$col.href assign=anchor}-->
						<a <!--{if $col.target}-->target="_blank"<!--{/if}--> href="<!--{compose_url_backend url=$anchor}-->">
					<!--{/if}-->

					<!--{if $col.direct_href}-->
						<!--{*eval var=$col.href assign=anchor*}-->
						<a <!--{if $col.target}-->target="_blank"<!--{/if}--> href="http://<!--{$col.direct_href}-->">
					<!--{/if}-->
					<!--{if $col.color}-->
						<span style="color:<!--{$col.color}-->">
					<!--{/if}-->
					<!--{if $col.calculable}-->
                        <!--{if isset($col.manual_field)}-->
                            <!--{include file=$col.manual_field
                                cell_info = $col
                                }-->
                        <!--{else}-->
						    <!--{eval var=$col.calculable}-->
                        <!--{/if}-->
					<!--{elseif $col.sort_order}-->
						<!--{eval var=$col.moving_item_id assign=moving_item_id}-->
						<a href="exchange_order.html?item_id=<!--{$moving_item_id}-->&direction=up&<!--{$col.extra_params_str}-->"
							><img src="/_images/sort-up.png" alt="Вверх" title="Передвинуть Вверх" /></a>
						&nbsp;
						<a href="exchange_order.html?item_id=<!--{$moving_item_id}-->&direction=down&<!--{$col.extra_params_str}-->"
							><img src="/_images/sort-down.png" alt="Вниз" title="Передвинуть Вниз" /></a>
					<!--{else}-->
						<!--{if $col._lookuped_value}-->
							<!--{$col._lookuped_value}-->
						<!--{elseif $col.type == 'date'}-->
							<!--{$col.value|date_format:"`$col.dateformat`"}-->
						<!--{elseif $col.type == 'datime'}-->
							<!--{$col.value|date_format:"`$col.dateformat`"}-->
						<!--{elseif $col.type == 'datetime'}-->
                            <!--{if $col.value == '0000-00-00 00:00:00'}-->
                                ---
                            <!--{else}-->
                                <!--{$col.value|date_format:$smarty.const.BACKEND_DATE_FORMAT}-->
                            <!--{/if}-->
						<!--{elseif $col.direct_image }-->
						
							<!--{if isset($col.size_name) && !isset($col.rewrite_img_base) }--> 
								<img src= "-photos/<!--{$col.size_name}-->/<!--{$col.value}-->" >
							<!--{elseif isset($col.rewrite_img_base) && isset($col.size_name) }-->
								<img src= "<!--{$col.rewrite_img_base}-->/<!--{$col.size_name}-->/<!--{$col.value}-->" >
							<!--{else}-->
								<img src= "-imgs/<!--{$col.value}-->" >
							<!--{/if}-->
						<!--{else}-->
							<!--{if $browse_truncate }-->
								<!--{$col.value|strip_tags|truncate:$browse_truncate}-->
							<!--{else}-->
							    <!--{if isset($col.truncate_length)}-->
							        <!--{assign var=field_truncate_length value=$col.truncate_length}-->
							    <!--{else}-->
							        <!--{assign var=field_truncate_length value=110}-->
							    <!--{/if}-->
								<!--{if isset($col.span_title)}-->
									<span title="<!--{$col.span_title}-->"><!--{$col.value|strip_tags|truncate:$field_truncate_length}--></span>
								<!--{else}-->
									<!--{$col.value|strip_tags|truncate:$field_truncate_length}-->
								<!--{/if}-->
							<!--{/if}-->
						
						<!--{/if}-->
					<!--{/if}-->
					<!--{if $col.color}-->
						</span>
					<!--{/if}-->

					<!--{* <!-- = = = = = = [ </a> ] = = = = = = --> *}-->
					<!--{if $col.href && !isset($col.manual_field)}-->
						</a>
					<!--{/if}-->
                    <!--{*
                    &nbsp;
                     *}-->
				<!--{/strip}--></td>
			<!--{/if}-->
		<!--{/if}-->
		<!--{/foreach}-->
	</tr>
	<!--{/foreach}-->

</table>

</div>
</form>
