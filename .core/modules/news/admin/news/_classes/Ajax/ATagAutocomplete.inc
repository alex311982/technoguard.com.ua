<?php
namespace Ekv\B\News\News\Ajax;

use Ekv\Helpers\System\AjaxCmd;
use EkvGlobalHelper;

class ATagAutocomplete extends AjaxCmd
{
    private $_searchTerm;
    private $_langID;

    protected function _init()
    {
        $this->_searchTerm = isset($this->data["term"]) ? trim($this->data["term"]) : "";
        $this->_langID = @intval($this->data["langID"]);
    }

    function mainPerformCmd()
    {
        $this->_jsonHeader();

        $term = $this->_searchTerm;
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_TCONTENT_TAGS."`
            WHERE
                tag_name LIKE '{$term}%'
                AND tag_enabled = 1
                AND lang_id = '{$this->_langID}'
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $raw = $this->DB->RunSql($sql, true);

        $formatted = array();
        if(!empty($raw)){
            foreach($raw as $rval){
                $formatted[] = array(
                    'id' => $rval["id_tag"],
                    'value' => $rval["tag_name"]
                );
            }
        }

        return $formatted;
    }

}
 