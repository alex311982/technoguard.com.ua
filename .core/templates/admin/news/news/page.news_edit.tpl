<!--{if !isset($include_self)}-->
<!--{assign_html}-->
<!--{include file="common:page.main_custom.tpl"
    content_tpl = $ekv_resource_name
    include_self = 1
    }-->
    <!--{inner_use do_action='exit'}-->
<!--{/if}-->

<script type="text/javascript" src="/_js/transl-related.js"></script>

<form id="main_form" action="" method="post" enctype="multipart/form-data">

<!--{include file="common:common/blocks/b.form_top.tpl"}-->

<!-- duplicate toolbar buttons -->
<div class="tbl-form">
    <!--{include file="common:common/blocks/b.edit_table.tpl"}-->
</div>

<!-- = = = = = = [ related translations ] = = = = = = -->
<div class="transl-related<!--{if isset($transl_type)}--> <!--{$transl_type}--><!--{/if}-->">

    <br/><br/><br/><br/><br/>
    <!--{if !empty($transl_related)}-->
    <!--{foreach from=$transl_related item='transl_val'}-->
        <!--{assign var="default_lang_class" value=""}-->
        <!--{if $transl_val.lang_default}-->
            <!--{assign var="default_lang_class" value=" transl-show-hide-default"}-->
        <!--{/if}-->
        <a class="transl-show-hide<!--{$default_lang_class}-->"><!--{$transl_val.lang_name}--></a>

        <div class="slidingDiv">
            <ul style="display:none;">
                <li class="tRelSrcName"><!--{$transl_val.tc_title}--></li>
                <li class="tRelSrcShort"><!--{$transl_val.text_short}--></li>
                <li class="tRelSrcFull"><!--{$transl_val.text_long}--></li>
            </ul>
        </div>
    <!--{/foreach}-->
    <!--{/if}-->

    <div id="tRelName"></div>

    <div id="tRelShort"></div>

    <div id="tRelFull"></div>

</div>

<div class="spacer"></div>
<!--{include file="common:common/blocks/b.form_top.tpl"}-->
</form>


<style type="text/css">
    .tbl-form{
        width: 60%;
        float: left;
    }

    table.grid tr td.title{
        width: 70px;
    }
</style>

<script type="text/javascript">
    $(function (){
        // +/- click for translations list
        $('.transl-show-hide').click(function (e){
            e.preventDefault();
            translRelated.tClick(this);
        });

        $(".transl-show-hide-default").click();
    });
</script>