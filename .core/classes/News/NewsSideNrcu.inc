<?php
namespace Ekv\News;

use Ekv\Helpers\System\Lang;

class NewsSideNrcu
{
    private $DB;
    private $langID;
    private $now;

    private $tagName;

    function __construct($langID)
    {
        $this->DB = \DB::getInstance();
        $this->langID = intval($langID);
        $this->now = \EkvDateHelper::mysqlNow();

        $tagName = "НРКУ";
        if(!Lang::getInstance()->isLangIDMain($this->langID)){
            $tagName = "NRCU";
        }
        $this->tagName = $tagName;
    }

    function mainQuotes($limit = 3)
    {
 
$sql = "
            SELECT *
            FROM `ekv_schedule_period_item`
	    WHERE quote_header != ''
            ORDER BY
                id_item DESC
            LIMIT ".$limit."
        ";

$result = $this->DB->RetrieveRecordsSqlOptimized($sql, false, false);

foreach ($result['records'] as $key => $item) {
                       
	$item['url_preview'] = '/quotes.html?id='.$item['id_item'];
	
	$item['dt_full'] = date('d.m.Y H:i', strtotime($item['dt_full']));

	$result['records'][$key] = $item;
}

	return $result;
	

    }

    function mainNews($limit = 10, $daysLimit = 14)
    {

        $tagID = $this->getNrcuTagID();

        if($tagID > 0 == false){
            return array();
        }

        $fields = NewsRoot::getNewsFields();

        $sql = "
            SELECT {$fields}
            FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."` n
                INNER JOIN `".\EkvGlobalHelper::TABLE_TCONTENT_TAGS_CONN."` tc
                    ON tc.transl_id = n.id_transl
                        AND tc.tag_id = '{$tagID}'
            WHERE
                ".NewsRoot::sqlMainCond()."
                AND n.lang_id = '{$this->langID}'
                AND n.date_pub <= '{$this->now}'
                -- AND n.date_pub > DATE_SUB('{$this->now}', INTERVAL {$daysLimit} DAY)
            ORDER BY
                n.date_pub DESC
            LIMIT {$limit}
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";

        $newsRaw = $this->DB->RunSql($sql, true);
        if(!empty($newsRaw)){
            $formatterObj = new NewsFormatter();
            $newsFormatted = $formatterObj->prepareMultipleItems($newsRaw);

            $grouppedNews = $this->groupNews($newsFormatted);
            return $grouppedNews;
        }

        return array();
    }

    private function groupNews($newsFormatted)
    {
        $groupped = array();

        foreach($newsFormatted as $nfVal){
            $date = $nfVal["date_ext"]["date"];

            if(!isset($groupped[$date])){
                $groupped[$date] = array(
                    'date_sym' => $nfVal["date_sym"]["date_var"],
                    'items' => array()
                );
            }

            $groupped[$date]["items"][] = $nfVal;
        }

        return $groupped;
    }

    private function getNrcuTagID()
    {
        $sql = "
            SELECT id_tag
            FROM `".\EkvGlobalHelper::TABLE_TCONTENT_TAGS."`
            WHERE
                lang_id = '{$this->langID}'
                AND tag_enabled = 1
                AND tag_name = ".$this->DB->escape($this->tagName)."
            LIMIT 1
        ";
        $res = $this->DB->RunSql($sql);

        $tagID = 0;
        if(!empty($res)){
            $tagID = $res["id_tag"];
        }

        return $tagID;
    }

    function getAllNewsUrl()
    {
        $tag = urlencode($this->tagName);
        $url = "/news/list.html?tag={$tag}";
        \EkvCommonHelper::appendUrlWithLangPrefixRef($url);
        return $url;
    }
}
 