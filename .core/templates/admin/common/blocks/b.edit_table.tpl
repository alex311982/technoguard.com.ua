<table cellspacing="0" cellpadding="2" border="0" class="grid">

<!--{assign var="next_tr_class" value=""}-->
<!--{foreach from=$result.fields key=name item=field}-->
<!--{assign var=input_name value="_input_`$name`"}-->

<!--{if !empty($smarty.post.$input_name)}-->
    <!--{assign var=value value=$smarty.post.$input_name|escape}-->
<!--{else}-->
    <!--{assign var=value value=$result.record.$name.value|escape}-->
<!--{/if}-->

<tr<!--{$next_tr_class}-->>
<!--{if $field.view == true && ($field.type != 'autotext' || $value != '')}-->
    <!--{cycle values=' class="bg",' assign="next_tr_class"}-->

    <td class="title"><!--{$field.title}--></td><td class="req"><!--{if $field.required}-->*<!--{else}-->&nbsp;<!--{/if}--></td><td>
    <!--{if $field.type == 'text'}-->
        <input<!--{if $field.readonly}--> readonly<!--{/if}--> type="text" value="<!--{$value}-->" name="_input_<!--{$name}-->"/>
    <!--{elseif $field.type == 'textarea'}-->
        <textarea <!--{if $field.readonly}--> readonly<!--{/if}--> name="_input_<!--{$name}-->" rows="<!--{$field.rows}-->"/><!--{$value}--></textarea>

    <!--{* html using ck editor *}-->
    <!--{elseif $field.type == 'html'}-->
        <textarea name="_input_<!--{$name}-->" rows="10" cols="80" style="width: 100%; height: 200px"><!--{$value}--></textarea>
        <script type="text/javascript">
        <!--
            //var editor = ;
            CKFinder.setupCKEditor(CKEDITOR.replace( '_input_<!--{$name}-->'), '/ckeditor/_ckfinder/');
        //-->
        </script>

    <!--{elseif $field.type == 'file'}-->
        <!--{if $value}-->
            <!--{if $result.record.typed.value == 'swf'}-->
                <object type="application/x-shockwave-flash" data='-imgs/<!--{$result.record.banner.value}-->' width="<!--{$result.record.widthx.value}-->"
                    height="<!--{$result.record.heighty.value}-->" id="flash" align="middle">
                    <param name="allowScriptAccess" value="sameDomain" />
                    <param name="movie" value='-imgs/<!--{$result.record.banner.value}-->' />
                    <param name="menu" value="false" />
                    <param name="quality" value="high" />
                    <param name="scale" value="noscale" />
                    <param name="bgcolor" value="#ffffff" />
                </object>
                <br>
            <!--{else}-->
                <!--{if isset($field.size_name_edit)}-->
                    <!--{if isset($field.rewrite_img_base) }-->
                        <img src = '<!--{$field.rewrite_img_base}-->/<!--{$field.size_name_edit}-->/<!--{$value}-->'><br>
                    <!--{else}-->
                        <img src = '-photos/<!--{$field.size_name_edit}-->/<!--{$value}-->'><br>
                    <!--{/if}-->
                <!--{elseif isset($field.size_name)}-->
                    <!--{if isset($field.rewrite_img_base) }-->
                        <img src = '<!--{$field.rewrite_img_base}-->/<!--{$field.size_name}-->/<!--{$value}-->'><br>
                    <!--{else}-->
                        <img src = '-photos/<!--{$field.size_name}-->/<!--{$value}-->'><br>
                    <!--{/if}-->
                <!--{else}-->
                    <img src = '-imgs/<!--{$value}-->' alt="picture"><br>
                <!--{/if}-->
            <!--{/if}-->

            <!--{if $field.show_delete_checkbox }-->
                <label>
                    Удалить изображение:
                    <input type = "checkbox" name = "_input_delete_<!--{$name}-->" value = "1" class="chk">
                </label>
            <!--{else}-->
                <!--{if !isset ($field.notshow_check_delete) }-->
                    If you want to delete this image, please, mark: <input type = "checkbox" name = "_input_check_image" value = "ok" class="chk">
                <!--{/if}-->
            <!--{/if}-->
        <!--{/if}-->
        <input<!--{if $field.readonly}--> readonly<!--{/if}--> type="file" name="_input_<!--{$name}-->"/>
    <!--{elseif $field.type == 'image' }-->
        <!--{if isset($field.path)}-->
                <img src = '<!--{$field.path}-->/<!--{$field.size_name}-->/<!--{$value}-->'><br>
        <!--{elseif isset($field.size_name)}-->
            <img src = '-photos/<!--{$field.size_name}-->/<!--{$value}-->'><br>
        <!--{else}-->
            <img src = '-imgs/<!--{$value}-->' alt="picture"><br>
        <!--{/if}-->
    <!--{elseif $field.type == 'file_other'}-->
        <!--{$value}-->
        <input<!--{if $field.readonly}--> readonly<!--{/if}--> type="file" name="_input_<!--{$name}-->"/>

    <!--{elseif $field.type == 'autotext'}-->
        <!--{if $value}-->
            <input<!--{if $field.readonly}--> readonly<!--{/if}--> type="text" value="<!--{$value}-->" name="_input_<!--{$name}-->"/>
        <!--{/if}-->
    <!--{elseif $field.type == 'password'}-->
        <input<!--{if $field.readonly}--> readonly<!--{/if}-->  type="password" value="" name="_input_<!--{$name}-->"/>
    <!--{elseif $field.type == 'date'}-->
        <!--{if empty($result.record.$name.start_year)}-->
            <!--{assign var=start_year value=$result.fields.$name.start_year}-->
        <!--{else}-->
            <!--{assign var=start_year value=$result.record.$name.start_year}-->
        <!--{/if}-->
        <!--{include file="common:ui.date_picker.tpl" date_value=$result.record.$name.value attrs=$field start_year=$start_year date_name=$name}-->

    <!--{elseif $field.type == 'datetime'}-->
        <!--{if isset($field.readonly) && $field.readonly != false}-->
            <!--{if $result.record.$name.value == '0000-00-00 00:00:00'}-->
                ---
            <!--{else}-->
                <!--{$result.record.$name.value|date_format:$smarty.const.BACKEND_DATE_FORMAT}-->
            <!--{/if}-->
        <!--{else}-->
            <!--{if empty($result.record.$name.start_year)}-->
                <!--{assign var=start_year value=$result.fields.$name.start_year}-->
            <!--{else}-->
                <!--{assign var=start_year value=$result.record.$name.start_year}-->
            <!--{/if}-->
            <!--{include file="common:ui.datetime_picker.tpl" date_value=$result.record.$name.value attrs=$field start_year=$start_year date_name=$name}-->
        <!--{/if}-->

    <!--{elseif $field.type == 'select'}-->
        <select<!--{if $field.readonly}--> readonly<!--{/if}-->  name="_input_<!--{$name}-->">
            <!--{html_options options=$field.options selected=$value}-->
        </select>
    <!--{elseif $field.type == 'checkbox'}-->
        <input type="hidden" name="_input_<!--{$name}-->" value="0" />

        <!--{if $value > 0}-->
            <!--{assign var="current_chb_checked" value="checked"}-->
        <!--{elseif isset($field.init_status) && $is_backend_edit_new_action == true}-->
            <!--{assign var="current_chb_checked" value="checked"}-->
        <!--{else}-->
            <!--{assign var="current_chb_checked" value=""}-->
        <!--{/if}-->


        <input type="checkbox" name="_input_<!--{$name}-->" value="1" <!--{$current_chb_checked}-->
         style="width:auto;" id="idChbx_<!--{$name}-->"
         /><label for="idChbx_<!--{$name}-->"><!--{$field.title}--></label>
    <!--{else}-->
        <!--{if isset($field.manual_field)}-->
            <!--{include file=$field.manual_field
                this_field_value = $value
                this_field_name = $name
                this_field_readonly = $field.readonly
                this_field_full = $field
                }-->
        <!--{else}-->
            <input<!--{if $field.readonly}--> readonly<!--{/if}--> type="text" value="<!--{$value}-->" name="_input_<!--{$name}-->"/>
        <!--{/if}-->
    <!--{/if}-->
    </td>
<!--{else}-->
    <input type="hidden" value="<!--{$value|escape}-->" name="_input_<!--{$name}-->" />
<!--{/if}-->
</tr>

<!--{/foreach}-->

</table>