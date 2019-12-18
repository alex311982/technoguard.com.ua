<?php

use Ekv\B\News\News\MNewsCreate;
use Ekv\B\News\News\MNewsTranslationRelated;
use Ekv\Helpers\System\Lang;
use Ekv\News\NewsRoot;
use Ekv\News\Permissions\NewsPermissionContainer;
use Ekv\News\Permissions\UserNewsPermission;
use Ekv\News\Tags\NewsTagsRoot;

class NewsNews extends Entity
{
    public $browseActionTranslation = "news_list";

    public $PK = "id";

    public $newsID;

    function displayEditTpl()
    {
        $this->Display('common:news/news/page.news_edit.tpl');
    }

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('news'));

        $this->newsID = @intval($_REQUEST["id"]);

        $this->_prepareNewsFields();
    }

    private function _prepareNewsFields()
    {
        $this->DB->table = 'news';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "news");
        $this->DB->fields["enabled"]["view"] = true;
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "Новости";
        $breadcrumps []= array('title' => $title,
            'href' => 'news_list');

            return $breadcrumps;
    }
}
