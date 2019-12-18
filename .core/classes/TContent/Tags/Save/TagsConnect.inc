<?php
namespace Ekv\TContent\Tags\Save;
use EkvGlobalHelper;

abstract class TagsConnect
{
    protected $DB;
    private $_translID;

    /**
     * Tags being linked
     * @var array
     */
    private $_tagIds;

    protected $_currentType;

    function __construct($tagIds, $translID)
    {
        $this->DB = \DB::getInstance();
        $this->_initOverridable();

        $this->_translID = intval($translID);;
        $this->_tagIds = $tagIds;
    }

    /*
     * Set $_currentType here
     */
    abstract protected function _initOverridable();

    function mainHandleConnections()
    {
//        if(empty($this->_tagIds)){
//            return false;
//        }

        $tag_ids_already_linked = $this->_getTagIdsAlreadyLinked();
        $this->_divideToGroupsAndApplyActions($tag_ids_already_linked);

        return true;
    }

    private function _getTagIdsAlreadyLinked()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_TCONTENT_TAGS_CONN."`
            WHERE
                transl_id = '{$this->_translID}'
                AND conn_type = '{$this->_currentType}'
        ";
        $res = $this->DB->RunSql($sql, true, 'tag_id');

        $tag_ids_found = array();
        if(!empty($res)){
            $tag_ids_found = array_keys($res);
        }

        return $tag_ids_found;
    }

    /**
     * Divides to groups and apply specific actions for each one (delete or insert)
     * @param $tags_already_linked
     */
    private function _divideToGroupsAndApplyActions($tags_already_linked)
    {
        /*
         * Compare already linked tags to the ones being connected now and create 2 groups:
         * - new tags, which must be linked
         * - removed tags, which have to be deleted
         */
        $delete_ids = array_diff($tags_already_linked, $this->_tagIds);

        $insert_ids = array_diff($this->_tagIds, $tags_already_linked);

        //pa($delete_ids, $insert_ids);

        if(!empty($insert_ids)){
            $this->_linkTags($insert_ids);
        }

        if(!empty($delete_ids)){
            $this->_deleteLinkedTagsConn($delete_ids);
        }
    }

    private function _linkTags($tag_ids)
    {
        $sql_tpl = "
            INSERT IGNORE INTO `".EkvGlobalHelper::TABLE_TCONTENT_TAGS_CONN."`
            (tag_id, transl_id, conn_type)
            VALUES
                %1\$s
        ";

        $parts = array();
        foreach($tag_ids as $tag_id){
            $tag_id = intval($tag_id);
            $parts[] = "('{$tag_id}', '{$this->_translID}', '{$this->_currentType}')";
        }

        $sql = sprintf($sql_tpl, \EkvCommonHelper::implodeWithComma($parts));
        $this->DB->query($sql);
    }

    private function _deleteLinkedTagsConn($tag_ids)
    {
        $sql = "
            DELETE FROM `".EkvGlobalHelper::TABLE_TCONTENT_TAGS_CONN."`
            WHERE
                transl_id = '{$this->_translID}'
                AND conn_type = '{$this->_currentType}'
                AND tag_id IN (".\EkvCommonHelper::implodeWithComma($tag_ids).")
        ";
        $this->DB->query($sql);
    }


}
 