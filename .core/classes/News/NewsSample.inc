<?php
namespace Ekv\News;

class NewsSample
{
    private $DB;

    private $_now;

    function __construct()
    {
        $this->DB = \DB::getInstance();
        $this->_now = \EkvDateHelper::mysqlNow();
    }

    function getTopNewsItem($lang_id)
    {
        $lang_id = intval($lang_id);
        $fields = NewsRoot::getNewsFields(). ", n.text_short";

        $sql = "
            SELECT {$fields}
            FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."` n
            WHERE
                ".NewsRoot::sqlMainCond()."
                AND n.is_top = 1
                AND n.lang_id = '{$lang_id}'
                AND n.date_pub <= '{$this->_now}'
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql);

        if(!empty($res)){
            $formatter = new NewsFormatter();
            $formatter->setImageSize('size2');
            $res = $formatter->prepareSingleItem($res);
        }

        return $res;
    }

    function getMainNewsOfTheDay($lang_id, $limit = 3)
    {
        $lang_id = intval($lang_id);
        $fields = NewsRoot::getNewsFields();

        $sql = "
            SELECT {$fields}
            FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."` n
            WHERE
                ".NewsRoot::sqlMainCond()."
                AND n.is_top = 0
                AND n.is_main = 1
                AND n.lang_id = '{$lang_id}'
                AND n.date_pub <= '{$this->_now}'
            ORDER BY
                n.date_pub DESC
            LIMIT {$limit}
        ";
        $res = $this->DB->RunSql($sql, true);

        if(!empty($res)){
            $formatter = new NewsFormatter();
            $formatter->setImageSize('size1');
                $res = $formatter->prepareMultipleItems($res);
        }

        return $res;
    }

    function getRestOfDailyNews($lang_id, $limit = 4)
    {
        $lang_id = intval($lang_id);
        $fields = NewsRoot::getNewsFields();

        $sql = "
            SELECT {$fields}
            FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."` n
            WHERE
                ".NewsRoot::sqlMainCond()."
                AND n.is_top = 0
                AND n.is_main = 0
                AND n.lang_id = '{$lang_id}'
                AND n.date_pub <= '{$this->_now}'
            ORDER BY
                n.date_pub DESC
            LIMIT {$limit}
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true);

        if(!empty($res)){
            $formatter = new NewsFormatter();
            $formatter->setImageSize('size1');
            $res = $formatter->prepareMultipleItems($res);
        }

        return $res;
    }

    function getLatestNews($lang_id, $limit = 10)
    {
        $lang_id = intval($lang_id);
        $fields = NewsRoot::getNewsFields();

        $sql = "  
            SELECT {$fields}
            FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."` n
            WHERE
                ".NewsRoot::sqlMainCond()."
                AND n.lang_id = '{$lang_id}'
                AND n.date_pub <= '{$this->_now}'
            ORDER BY
                n.date_pub DESC
            LIMIT {$limit}
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true);

        if(!empty($res)){
            $formatter = new NewsFormatter();
            $formatter->setImageSize('size1');
            $res = $formatter->prepareMultipleItems($res);
        }

        return $res;
    }

    function getNewsPageMoreNews($langID, $exceptNewsID, $limit = 10)
    {
        $langID = intval($langID);
        $fields = NewsRoot::getNewsFields();

        $sql = "
            SELECT {$fields}
            FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."` n
            WHERE
                ".NewsRoot::sqlMainCond()."
                AND n.lang_id = '{$langID}'
                AND n.news_id != '{$exceptNewsID}'
                AND n.date_pub <= '{$this->_now}'
            ORDER BY
                n.date_pub DESC
            LIMIT {$limit}
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true);

        if(!empty($res)){
            $formatter = new NewsFormatter();
            $formatter->setImageSize('size1');
            $res = $formatter->prepareMultipleItems($res);
        }

        return $res;
    }
}
 