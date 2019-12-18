<?php
namespace Ekv\TContent\Backend;
use Ekv\Helpers\System\Lang;
use Ekv\TContent\Permissions\TcPermissionContainer;
use EkvGlobalHelper;

abstract class TcCreate
{
    private $DB;
    private $_tcPermContainer;

    protected $_tableRoot;
    protected $_tableTransl;
    protected $_fieldTcID;

    function __construct(TcPermissionContainer $tcPermContainer)
    {
        $this->DB = \DB::getInstance();
        $this->_tcPermContainer = $tcPermContainer;

        $this->_init();
    }

    abstract protected function _init();


    /**
     * Get language options which are still available for creating.
     * When completely new piece of news is created (not translation for existent news item, $news_id = 0)
     * then all langs are available (except denied for user).
     * When the translation is created for exsistent news item then show options for absent lang translations
     * @param int $news_id
     * @return array
     */
    function getLangsStillAvailable($news_id = 0)
    {
        $langObj = Lang::getInstance();
        $langs_options = $langObj->getLangAvailableOptions();

        $langs_allowed = $this->_tcPermContainer->getLangIdsActive();
        $stuffLangUkCurrentModer = $this->_tcPermContainer->checkStuffAccess('stuff_lang_uk');

        if ($stuffLangUkCurrentModer) {
            $langs_allowed[LANG::LANG_ID_UK] = LANG::LANG_ID_UK;
        }

        $langs_options = array_intersect_key($langs_options, $langs_allowed);
        //$langs_options = array_diff_key($langs_options, $langs_allowed);

        if($news_id == 0){
            return $langs_options;
        }

        $langs_existed = $this->_getLangsAlreadyCreated($news_id);
        $langs_options = array_diff_key($langs_options, $langs_existed);

        return $langs_options;
    }

    private function _getLangsAlreadyCreated($tc_id)
    {
        $tc_id = intval($tc_id);

        $lang_ids = array();

        $sql = "
            SELECT lang_id
            FROM `{$this->_tableTransl}`
            WHERE
                `{$this->_fieldTcID}` = '{$tc_id}'
        ";

        $this->DB->query($sql);
        while($row = $this->DB->fetch_row()){
            $lang_ids[$row["lang_id"]] = $row["lang_id"];
        }

        return $lang_ids;
    }

    private function _createTcRootID()
    {
        $sql = "
            INSERT INTO `{$this->_tableRoot}`
            SET
                `fake` = ''
        ";
        $this->DB->query($sql);

        return $this->DB->get_insert_id();
    }

    function updateTcRootID($translation_id)
    {
        $translation_id = intval($translation_id);
        $tc_id = $this->_createTcRootID();

        $sql = "
            UPDATE `{$this->_tableTransl}`
            SET
                `{$this->_fieldTcID}` = '{$tc_id}'
            WHERE
                `id_transl` = '{$translation_id}'
        ";
        $this->DB->query($sql);
    }
}
 