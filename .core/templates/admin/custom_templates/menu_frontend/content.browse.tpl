
<!--{*<script type="text/javascript" src="/_js/jquery.ui.js"></script>*}-->
<script type="text/javascript" src="/_js/tree_table/jquery.treeTable.min.js"></script>
<link rel="stylesheet" href="/_js/tree_table/jquery.treeTable.css" >
<style>
    #idMenuTreeTable td{
        border-bottom:1px solid #E9E9E9;
        height:20px;
    }
    #idMenuTreeTable td.toolbar{
        width:150px;
    }
    #idMenuTreeTable td.td-tt-onmain span.title{
        background-color:#B9E7FF;
    }
</style>

<script type="text/javascript">
    $(function(){
        $("#idMenuTreeTable").treeTable({
          initialState: "expanded",//"collapsed",
          clickableNodeNames: true
        });

    });
</script>



<form id="main_form" action="" method="post" enctype="multipart/form-data">
<input type="hidden" name="_input_is_form_submited" />
<input type="hidden" name="MAX_FILE_SIZE" value="50000000">


<!-- = = = = = = [ main ] = = = = = = -->
<div class="main"  id="main-scroll">
	<!--{include file="common:block.errors.tpl"}-->
	
    <table id="idMenuTreeTable" style="margin-left:20px; width:500px;" cellpadding="0" cellspacing="0">
    <!--{if !empty($html_tree)}-->
    <!--{foreach from=$html_tree item="tree_val"}-->
        <tr id="<!--{$tree_val.js_info.node}-->" class="<!--{$tree_val.js_info.child_of_node}-->">
            <!--{if $tree_val.display_on_main == 1}-->
                <!--{assign var="td_tree_table_class" value=" td-tt-onmain "}-->
            <!--{else}-->
                <!--{assign var="td_tree_table_class" value=""}-->
            <!--{/if}-->
            <td class="item-name<!--{$td_tree_table_class}-->">
                <span class="title"><!--{$tree_val.title}--></span>
            </td>
            <td class="toolbar">
                <!--{if $buttonsDisplayParams.create == true}-->
                    <a href="<!--{$tree_val.urls.new_sibling}-->" title="Добавить в текущий уровень"
                        ><img alt="" src="/_images/btns/add_sibling.gif"></a>
                    <a href="<!--{$tree_val.urls.new_child}-->" title="Добавить во вложенный уровень"
                        ><img alt="Редактировать" src="/_images/btns/add_child.gif"></a>
                <!--{/if}-->

                <!--{if $buttonsDisplayParams.edit == true}-->
                    <a href="<!--{$tree_val.urls.edit}-->" title="Редактировать"
                        ><img height="16" width="12" alt="Редактировать" src="/_images/btns/edit.gif"></a>
                <!--{/if}-->

                <!--{if $buttonsDisplayParams.delete == true}-->
                    <a onclick="if(!confirm('Подтвердите удаление текущего и всех вложенных пунктов')){return false;}"
                        href="<!--{$tree_val.urls.delete}-->" title="Удалить"
                        ><img height="16" width="14" alt="Удалить" src="/_images/btns/delete.gif"></a>
                <!--{/if}-->

                <!--{if $buttonsDisplayParams.move == true}-->
                    <a href="<!--{$tree_val.urls.move_up}-->"
                        ><img src="/_images/sort-up.png" alt="Вверх" title="Передвинуть Вверх" ></a>
                    <a href="<!--{$tree_val.urls.move_down}-->"
                        ><img src="/_images/sort-down.png" alt="Вниз" title="Передвинуть Вниз" ></a>
                <!--{/if}-->

                <!--{if $tree_val.is_active}-->
                    <img height="16" src="/_images/btns/active-menu-item.png" alt="активен" title="Активен" />
                <!--{else}-->
                    <img height="16" src="/_images/btns/blank.png" alt=""  />
                <!--{/if}-->
<!--{*
                <!--{if $tree_val.display_in_text}-->
                <img height="16" src="/_images/btns/t-icon.gif" alt="текстовое меню" title="Присутствует в текстовом меню" />
                <!--{/if}-->
 *}-->
            </td>
        </tr>
    <!--{/foreach}-->
    <!--{else}-->
        <a href="/admin/base/menuFrontend/new.html?create_for_id=0&add_type=initial&menuType=<!--{$menuType}-->&langID=<!--{$langID}-->">Створити перший пункт</a>
    <!--{/if}-->
    </table>
</div>
</form>