<?php
namespace Ekv\B\News\News\Tags;
use Ekv\News\NewsRoot;
use Ekv\TContent\Tags\Save\TagsConnect;
use Ekv\TContent\Tags\Save\TagsSave;
use Ekv\TContent\Tags\TagException;
use EkvGlobalHelper;

class MTagsSave extends TagsSave
{
    /**
     * @param $tag_ids_final
     * @param $translID
     * @return TagsConnect
     */
    protected function _createTagsConnectObj($tag_ids_final, $translID)
    {
        $tagConnectObj = new MTagsConnect($tag_ids_final, $translID);
        return $tagConnectObj;
    }

    protected function _getLangIdByTranslID($translID)
    {
        $newsObj = new NewsRoot();
        $transl_info = $newsObj->getTranslationInfoByID($translID);
        TagException::ensure(!empty($transl_info), "Incorrect translationID for saving tags");
        $lang_id = intval($transl_info["lang_id"]);
        return $lang_id;
    }


}
 