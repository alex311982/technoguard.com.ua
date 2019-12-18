<?php

class TextIdGenerator
{
    public $DB;
    public $helper;
    public $lengthLimit = 250; //limit because of varchar length
    private $_generateUniqueTextId = true;
    
    function __construct($db = null, $table_helper = "", $pk_helper = "", $text_id_helper = null)
    {
        $this->DB = $db;
        if (!empty($pk_helper) && !empty($table_helper)) {
            $this->setHelper($table_helper, $pk_helper, $text_id_helper);
        }
    }

    function setHelper($table, $pk, $text_id_helper = null)
    {
        $this->helper = new TextIdGenerator_Helper($this->DB, $table, $pk, $text_id_helper);
    }

    // change `text_id` field name
    function setTextIdFieldName($field_name)
    {
        $this->helper->text_id_field_name = $field_name;
    }

    //--------
    // for 1251 only
    //--------
    function getTextId($src_str)
    {
        $src_str = strip_tags($src_str);
        $src_str = trim($src_str);
        $src_str_translited = $this->translit($src_str);
        $src_str_translited = substr($src_str_translited, 0, $this->lengthLimit);
        $text_id = $this->escapeWrongCharacters($src_str_translited);

        $text_id = $this->skipMultipleUnderlinings($text_id); // '___' -> '_'
        //--- check whether just generated text_id is unique ---//
        if(
            $this->_generateUniqueTextId
            && $this->helper->checkTextIdUniqueness($text_id) == false
        ){
            $text_id = $this->helper->getReallyUniqueTextId($text_id);
        }

        return $text_id;
    }

    //--------
    // the class is written using win1251 but to use utf8  values you need to call
    // this method instead of getTextId
    //--------
    function getTextIdInUtf8($src_str_utf8, $delim = null)
    {
        $src_str_win = @iconv('utf-8', "windows-1251", $src_str_utf8);
        $text_id_win = $this->getTextId($src_str_win);
        if(is_null($delim) == false){
            $text_id_win = str_replace('_', $delim, $text_id_win);
        }
        $text_id_utf = iconv("windows-1251", 'utf-8', $text_id_win);
        return $text_id_utf;
    }

    /**
     * gets text id like part1-part2-p3
     *      DOESN'T CHECK UNIQUENESS(makes it in incrorrect way)
     * @param  $src_str_utf8
     * @param  bool $generate_unique_textid
     * @return mixed|string
     */
    function getTextIdInUtf8WithDash($src_str_utf8, $generate_unique_textid = false)
    {
        $this->_generateUniqueTextId = $generate_unique_textid;
        $text_id = $this->getTextIdInUtf8($src_str_utf8, '-');
        $text_id = $this->skipMultipleDashes($text_id);
        return $text_id;
    }


    function escapeWrongCharacters($src_val)
    {
        $pattern = "#[^0-9a-z_\-]#i";
        $res_str = preg_replace($pattern, "_", $src_val);
        return $res_str;
    }


    function skipMultipleUnderlinings($text_id)
    {
        $text_id = preg_replace("#_{2,}#", "_", $text_id);
        $text_id = preg_replace("#_$#", '', $text_id); //risk_ -> risk
        return $text_id;
    }
    function skipMultipleDashes($text_id)
    {
        $text_id = preg_replace("#\-{2,}#", "-", $text_id);
        $text_id = preg_replace("#\-$#", '', $text_id); //risk_ -> risk
        return $text_id;
    }

    function translitUtf8($utf_val)
    {
        $src_str_win = @iconv('utf-8', "windows-1251", $utf_val);
        $text_id_win = $this->translit($src_str_win);
        $text_id_utf = iconv("windows-1251", 'utf-8', $text_id_win);
        return $text_id_utf;
    }

    function translit($st)
    {
        //$st = utf8_win($st);
        // Сначала заменяем "односимвольные" фонемы.
        //echo "<h2>ST = $st</h2>";
        $st = strtr($st, "абвгдеёзийклмнопрстуфхъыэґі",
            "abvgdeeziyklmnoprstufh_iegi");

        $st = strtr($st, "АБВГДЕЁЗИЙКЛМНОПРСТУФХЪЫЭҐІ",
            "ABVGDEEZIYKLMNOPRSTUFH_IEGI");

        // Затем - "многосимвольные".
        $st = strtr($st,
            array(

                "ж" => "zh", "ц" => "ts", "ч" => "ch", "ш" => "sh",

                "щ" => "shch", "ь" => "", "ю" => "yu", "я" => "ya",

                "Ж" => "Zh", "Ц" => "Ts", "Ч" => "Ch", "Ш" => "Sh",

                "Щ" => "Shch", "Ь" => "", "Ю" => "Yu", "Я" => "Ya",

                "ї" => "yi", "Ї" => "Yi", "є" => "ye", "Є" => "Ye"

            )

        );
        //echo "<h2>ST = $st</h2>";
        // Возвращаем результат.
        //$st = win_utf8($st);
        return $st;

    }

    function buildProductName($prod_brand, $prod_model)
    {
        return "{$prod_brand}-{$prod_model}";
    }


}

class TextIdGenerator_Helper
{
    public $text_id_field_name = "text_id";
    public $DB;
    public $table, $tbl_pk;

    function __construct($db, $table, $pk, $text_id_field_name = null)
    {
        $this->DB = $db;
        $this->table = $table;
        $this->tbl_pk = $pk;
        if(is_null($text_id_field_name) == false){
            $this->text_id_field_name = $text_id_field_name;
        }
    }

    function checkTextIdUniqueness($text_id_src)
    {
        //return false;
        $tmp = $this->DB->bFieldsGetInfoById($this->table, $text_id_src, $this->text_id_field_name, "{$this->tbl_pk},  {$this->text_id_field_name}");

        return empty($tmp);
    }

    function getReallyUniqueTextId($text_id)
    {
        $i = 1;
        while ($this->checkTextIdUniqueness($text_id) == false) {
            $len_i = strlen((string) ($i - 1)); // needed when iterations come from 9->10, 99->100
            if ($i > 1) {
                $text_id = substr($text_id, 0, $len_i * (-1));
            }
            $text_id .= $i;
            $i++;
            if ($i > 100) {
                break;
            }
        }
        return $text_id;
    }

    function updateTextId($pk_val, $text_id)
    {
        $sql = "
			UPDATE `{$this->table}`
			SET
				`{$this->text_id_field_name}` = '$text_id'
			WHERE
				`{$this->tbl_pk}` = $pk_val 
		";
        $this->DB->query($sql);
    }

}


?>