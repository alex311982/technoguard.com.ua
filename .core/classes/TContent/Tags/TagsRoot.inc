<?php
namespace Ekv\TContent\Tags;
use EkvGlobalHelper;

abstract class TagsRoot
{
    const TAGS_DELIMITER = ',';

    const TYPE_NEWS = 1;
    const TYPE_STAT_PAGE = 2;

    protected $DB;

    protected $_currentType;

    function __construct()
    {
        $this->DB = \DB::getInstance();
        $this->_initOverridable();
    }

    abstract protected function _initOverridable();

    private function _getTranslationTags($translID)
    {
        $translID = intval($translID);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_TCONTENT_TAGS_CONN."` c
                INNER JOIN `".EkvGlobalHelper::TABLE_TCONTENT_TAGS."` t
                    ON c.tag_id = t.id_tag
                        AND c.conn_type = '{$this->_currentType}'
            WHERE
                c.transl_id = '{$translID}'
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    function getTranslationTagsPlain($translID)
    {
        $tags_full = $this->_getTranslationTags($translID);

        $tags_plain = array();

        foreach($tags_full as $val){
            $tags_plain[] = $val["tag_name"];
        }

        return $tags_plain;
    }

    function removeTcTagConnections($translatableContentID)
    {
        $sql = "
            DELETE FROM `".\EkvGlobalHelper::TABLE_TCONTENT_TAGS_CONN."`
            WHERE
                transl_id = '{$translatableContentID}'
                AND conn_type = '{$this->_currentType}'
        ";
        $this->DB->query($sql);
    }
}
 