<?php
namespace Ekv\TContent\Tags\Save;
use Ekv\News\NewsRoot;
use Ekv\TContent\Tags\TagException;
use Ekv\TContent\Tags\Save\TagsConnect;
use EkvGlobalHelper;

abstract class TagsSave
{
    private $_tagsStr;
    private $DB;

    private $_langID;
    private $_translID;

    function __construct($rawTagsStr, $translID)
    {
        $this->DB = \DB::getInstance();
        $this->_tagsStr = trim($rawTagsStr);
        $this->_translID = intval($translID);

        //--- define langID from translation info ---//
        $this->_langID = $this->_getLangIdByTranslID($translID);
//        $newsObj = new NewsRoot(); // todo pass as direct langID
//        $transl_info = $newsObj->getTranslationInfoByID($translID);
//        TagException::ensure(!empty($transl_info), "Incorrect translationID for saving tags");
//        $this->_langID = $transl_info["lang_id"];
    }

    /**
     * @param $tag_ids_final
     * @param $translID
     * @return TagsConnect
     */
    abstract protected function _createTagsConnectObj($tag_ids_final, $translID);

    /**
     * @param $translID
     * @return int - langID
     */
    abstract protected function _getLangIdByTranslID($translID);

    function mainHandleTags()
    {
        $tags_ar = $this->_retrieveTagsArray();
        $tag_ids_final = array();

        if(!empty($tags_ar)){
            $checked_res = $this->_checkTagsInDb($tags_ar);

            $tag_ids_final = $this->_getTagsIdsBeingConnected($checked_res["existed"], $checked_res["not_existed"]);
        }

        //$tagConnectObj = new MTagsConnect($tag_ids_final, $this->_translID);
        $tagConnectObj = $this->_createTagsConnectObj($tag_ids_final, $this->_translID);
        $tagConnectObj->mainHandleConnections();
    }

    /**
     * Convert tags cvs string to array
     * @return array
     */
    private function _retrieveTagsArray()
    {
        $tags = array();

        if(!empty($this->_tagsStr)){
            $tags_raw = explode(',', $this->_tagsStr);

            foreach($tags_raw as $tag_val){
                $tags[] = trim($tag_val);
            }
        }

        return $tags;
    }

    /**
     * Checks whether tags already exist in db and returns 2 groups (already existent and new)
     * @param $tags
     * @return array
     */
    private function _checkTagsInDb($tags)
    {
        $tags_safe = array();
        foreach($tags as $tag_val1){
            $tags_safe[] = $this->DB->escape($tag_val1);
        }

        //--- get related tags from db ---//
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_TCONTENT_TAGS."`
            WHERE
                tag_name IN (".implode(',', $tags_safe).")
                AND lang_id = '{$this->_langID}'
        ";
        $db_res = $this->DB->RunSql($sql, true, 'tag_name');

        $existed = array();
        $not_existed = array();

        foreach($tags as $tag_val){
            if(array_key_exists($tag_val, $db_res)){
                $tag_id = $db_res[$tag_val]["id_tag"];
                $existed[$tag_id] = $db_res[$tag_val];
            }else{
                $not_existed[] = $tag_val;
            }
        }

        return array(
            'existed' => $existed,
            'not_existed' => $not_existed
        );
    }

    /**
     * Returns array both of previously created and just created (new) tags
     * @param $existed
     * @param $not_existed
     * @return array
     */
    private function _getTagsIdsBeingConnected($existed, $not_existed)
    {
        $final_tag_ids = array_keys($existed);

        if(!empty($not_existed)){
            $just_created_tag_ids = $this->_createTags($not_existed);
            $final_tag_ids = array_merge($final_tag_ids, $just_created_tag_ids);

            //pa($final_tag_ids);
        }

        return $final_tag_ids;
    }

    private function _createTags($tags)
    {
        if(empty($tags)){
            if(empty($tags)){}
        }

        $sql_tpl = "
            INSERT IGNORE INTO `".EkvGlobalHelper::TABLE_TCONTENT_TAGS."`
            (`lang_id`, `tag_name`)
            VALUES
                %1\$s
        ";

        $tag_ids_created = array();

        foreach($tags as $tag_val){
            $item = "('{$this->_langID}', ".$this->DB->escape($tag_val).")";

            $sql = sprintf($sql_tpl, $item);
            $this->DB->query($sql);
            $tag_ids_created[] = $this->DB->get_insert_id();
        }

        return $tag_ids_created;
    }
}
 