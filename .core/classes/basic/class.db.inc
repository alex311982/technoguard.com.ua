<?php

class DB extends DbBase
{
    var $table = '';
    var $per_page = 20;
    var $per_pagination = 10;

    var $last_insert_id = 0;
    var $is_new = false;
    var $default_order = array();

    var $fields = array();
    var $_fields_parsed = false;
    var $errors_manual = array();
    static private $_isTransactionOpened = false;

    /**
     * when possible inner trans set this to true - then ignore all further inner trans operators
     * like commit or rollback
     * @see transStartSilent()
     * @var bool
     */
    private $_ignoreFurtherInnerTrans = false;

	/**
	 * @var DB
	 */
	private static $instance;

    //============================================================================
    public static function isTransactionOpened()
    {
        return self::$_isTransactionOpened;
    }

    public static function setIsTransactionOpened($isTransactionOpened)
    {
        self::$_isTransactionOpened = $isTransactionOpened;
    }

    function __construct()
    {
        $this->is_new = false;
        parent::__construct(
            DB_HOST, DB_BASE, DB_USER, DB_PASS,
            DB_PORT, DB_TABLE_PREFIX);
        if(defined('SITE_PER_PAGE')) $this->per_page = SITE_PER_PAGE;
        if(defined('SITE_PER_PAGINATION')) $this->per_pagination = SITE_PER_PAGINATION;
    }

	/**
	 * @return DB
	 */
	public static function getInstance()
	{
		if(empty(self::$instance))
		{
			self::$instance = new self();
		}

		return self::$instance;
	}

    //============================================================================

    function AddNew()
    {
        $this->_parse_fields();
        $this->is_new = true;
        $this->_get_options();
        $result = array();
        if(!empty($this->fields))
        {
            $result['fields'] = $this->fields;
        }
        //--- add MANUAL ERRORS support  ---//
        $this->JoinManualErrors();
        return $result;
    }

    //============================================================================

    function Delete($ids, $table = '', $pk_field_name = "id")
    {
        $table = empty($table) ? $this->table : $table;
        if(!is_array($ids)) $ids = array($ids);
        $sql = '
            DELETE FROM
                `'.$table.'`
            WHERE
                `'.$pk_field_name.'` IN ("'.implode('", "', $ids).'")
            ';
        $this->query($sql);
    }

    //============================================================================

    function IsNew()
    {
        if($this->is_new)
            return true;
        else
            return false;
    }

    //============================================================================

    function RetrieveRecordsSql($sql, $pagination = true, $use_fields = true)
    {
        $this->ResetErrors();
        $this->ResetMessages();

        $fields = array_keys($this->fields);

        $page = (isset($_GET['page'])) ? $_GET['page'] : 1;
        if (empty($sql)) return;
        if($pagination) $sql.= ' LIMIT '.($page-1)*$this->per_page.', '.$this->per_page;
        //---------------------------------------------------------------------------------------
        //echo "<h3>sql = $sql</h3>";
        $this->query($sql);
        $result = array();
        $_result = array();
        $total_recs = 0;
        while($row = $this->fetch_row())
        {
            $_result[]= $row;
        }
        // total rows
        $sql = "SELECT FOUND_ROWS() as `total_recs`";
        $total_result = $this->RunSql($sql);
        $total_recs = $total_result['total_recs'];

        //echo "<h1>tot recs  = $total_recs</h1>";

        //use_fields
        if ($use_fields == true){
            foreach($_result as $key => $row){
                 $result['records'][$key] = $this->_expand_record($row);
            }
        }else{
            foreach($_result as $key => $row){
                 $result['records'][$key] = $row;
            }
        }

        if(!empty($this->fields)) $result['fields'] = $this->fields;
        if($pagination) $result['pagination'] = $this->_pagination($total_recs, $this->per_page, $this->per_pagination, $page);

        return $result;
    }

    function RetrieveRecordsSqlOptimizedManualTotal($sql, $sql_total, $pagination = true, $use_fields = true, $page_param = -1 )
    {
        return $this->RetrieveRecordsSqlOptimized($sql, $pagination, $use_fields, $page_param, $sql_total);
    }
    //--- slow SQL_CALC_FOUND_ROWS is replaced with faster extra query COUNT(*) ---//
    function RetrieveRecordsSqlOptimized($sql, $pagination = true, $use_fields = true, $page_param = -1, $sql_total_direct = null)
    {
        $this->ResetErrors();
        $this->ResetMessages();

        //--- get sql total by cutting select feilds and replacing them with COUNT(*) ---//
//			$sql_total = preg_replace("#select .+(from .+)$#isU",
//					"select COUNT(*) as `total_recs` \\1",
//					$sql
//				);

        //--- new way to place select count(*) automatically ---//
        $position_of_from = stripos($sql, "from");
        if(is_null($sql_total_direct) == false){
            $sql_total = $sql_total_direct;
        }else if($position_of_from !== false){
            $sql_total = substr($sql, $position_of_from);
            $sql_total = "select COUNT(*) as `total_recs` " . $sql_total;
        }

        $fields = array_keys($this->fields);
        if($page_param > 0 == false){ // can be passed direct page
            //$page = (isset($_GET['page'])) ? $_GET['page'] : 1;
            $page = (isset($_GET['page'])) ? (int)$_GET['page'] : 1;
            if($page > 0 == false){
                $page = 1;
            }
        }else{
            $page = $page_param;
        }
        if (empty($sql)) return;
        if($pagination) $sql.= ' LIMIT '.($page-1)*$this->per_page.', '.$this->per_page;
        //---------------------------------------------------------------------------------------
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $this->query($sql);
        $result = array();
        $_result = array();
        $total_recs = 0;
        while($row = $this->fetch_row())
        {
            $_result[]= $row;
        }
        //--- total rows ---//
        $total_result = $this->RunSql($sql_total);
        $total_recs = $total_result['total_recs'];
        //echo "<h1>tot recs  = $total_recs</h1>";

        //use_fields
        if ($use_fields == true){
            foreach($_result as $key => $row){
                 $result['records'][$key] = $this->_expand_record($row);
            }
        }else{
            foreach($_result as $key => $row){
                 $result['records'][$key] = $row;
            }
        }

        if(!empty($this->fields)) $result['fields'] = $this->fields;
        if($pagination) $result['pagination'] = $this->_pagination($total_recs, $this->per_page, $this->per_pagination, $page);

        $result["sql"] = $sql;

        return $result;
    }

    function processWhereArray($where_array)
    {
        $sql_where = "";
        if(!empty($where_array))
        {
            $sql_where .= ' AND ';
            $where_parts = array();
            foreach($where_array as $field => $value){
                $where_parts []= '`'.$field.'` = "'.$value.'"';
            }
            $sql_where .= implode(' AND ', $where_parts);
        }
        return $sql_where;
    }

    //#------------------- RetrieveRecords -------------------#//

    function RetrieveRecords($table = '', $where = array(), $where_sql='')
    {
        $this->ResetErrors();
        $this->ResetMessages();
        $table = empty($table) ? $this->table : $table;
        $fields = array_keys($this->fields);
        //$filter = $this->_get_filter();
        //pa($filter);
        $page = (isset($_GET['page'])) ? $_GET['page'] : 1;

        //---------------------------------------------------------------------------------------
//			$sql_where = '';
//			if(!empty($where))
//			{
//				$sql_where .= ' AND ';
//				$where_parts = array();
//				foreach($where as $field => $value) $where_parts []= '`'.$field.'` = "'.$value.'"';
//				$sql_where .= implode(' AND ', $where_parts);
//			}
        $sql_where = $this->processWhereArray($where);

        $sql_where .= $this->standartFilter();

        if($where_sql!='')
        {
         $sql_where .= $where_sql;
        }
        //---------------------------------------------------------------------------------------
        $sql = '
            SELECT
                ';
        foreach($fields as $field)
        {
            if(!isset($this->fields[$field]['calculable']))
            {
                $sql .= ' `'.$table.'`.`'.$field.'`,';
            }
        }
        $sql = substr($sql, 0, -1);
        $sql .= '
            FROM
                `'.$table.'`';
            $sql .= ' WHERE 1=1 ';
            $sql .= $sql_where;
        if(!empty($this->default_order))
        {
            $order_parts = array();

            foreach($this->default_order as $okey => $ovalue)
            {
                $order_parts []= '`'.$okey.'` '.$ovalue;
            }
            $sql .= ' ORDER BY '.implode(', ', $order_parts);
        }

        $sql.= ' LIMIT '.($page-1)*$this->per_page.', '.$this->per_page;
        //echo "\n <pre> sql = $sql </pre> <br/>\n";

        //---------------------------------------------------------------------------------------
        $sql_query_final = $sql;
        $this->query($sql);
        $result = array();
        $_result = array();
        $total_recs = 0;
        while($row = $this->fetch_row())
        {
            $_result[]= $row;
        }
        if(!empty($_result))
        {
            $sql = 'SELECT COUNT(*) as `_total_` FROM `'.$table.'` WHERE 1=1 ' . $sql_where;
            $this->query($sql);
            $r = $this->fetch_row();
            $total_recs = $r['_total_'];
        }

        foreach($_result as $key => $row)
        {
            $result['records'][$key] = $this->_expand_record($row);
        }

        if(!empty($this->fields))
        {
            $result['fields'] = $this->fields;
        }
        $result['pagination'] = $this->_pagination($total_recs, $this->per_page, $this->per_pagination, $page);
        $result["sql"] = $sql_query_final;
        return $result;
    }

    /**
     * if you want to use standart backend filter for custom queries in backend
     * you should invoke this method and put return result to your custom query (where section)
     *
     * field["original_db_field"] - allows to assign proper sql prefix to field
     * (for example to avoid ambigous equal field from different tables)
     *
     * @return string - sqk where for custom query
     */
    function standartFilter()
    {
        $sql_where = "";
        $filter = $this->_get_filter();
        if(!empty($filter))
        {
            $filter_parts = array();
            foreach($filter as $field => $value)
            {
                $field_name_prepared = "`{$field}`";
                //--- original_db_field for complicated manual queries ---//
                if(isset($this->fields[$field]["original_db_field"])){
                    $field_name_prepared = "{$this->fields[$field]["original_db_field"]}";
                }
                if(
                    isset($this->fields[$field]["manual_processing"])
                    || !isset($this->fields[$field])
                ){
                    continue;
                }

                $value = trim($value);
                if(
                    (
                        !empty($value)
                        ||
                        //--- exact '0' is passed and it must be used as filter `field` = '0' - don't ignor it ---//
                        (
                            strlen($value) == 1
                            && $value == 0
                        )
                    )
                    && !isset($this->fields[$field]['calculable']))
                {
                    $value = mysqli_real_escape_string($this->connection_id, $value);

                    $current_field_type = @$this->fields[$field]['type'];
                    if( in_array($current_field_type, array("select", "checkbox") ) ){
                        $filter_parts []= $field_name_prepared.' = "'.$value.'"';
                    }
                    else{
                        $filter_parts []= 'LOWER('.$field_name_prepared.') LIKE LOWER("%'.$value.'%")'."\n";
                    }
                }
            }
            if(!empty($filter_parts))
            {
                $sql_where .= ' AND ';
                $sql_where .= implode(' AND ', $filter_parts);
            }
        }
        return $sql_where;
    }

    function RetrieveRecord($id, $field = 'id', $table = '', $sql_where_custom = "")
    {
        $id = intval($id);
        $this->ResetErrors();
        $this->ResetMessages();
        $table = empty($table) ? $this->table : $table;

        // ------------------------------
//        $sql_where = '';
//        if(!empty($where))
//        {
//            $sql_where .= ' AND ';
//            $where_parts = array();
//            foreach($where as $_field => $_value) $where_parts []= '`'.$_field.'` = "'.$_value.'"';
//            $sql_where .= implode(' AND ', $where_parts);
//        }

        // ------------------------------
        $SQL_WHERE_DEFAULT = ' `'.$field.'` = "'.$id.'" ';

        //--- make default condition empty when custom condition is passed ---//
        if(!empty($sql_where_custom)){
            $SQL_WHERE_DEFAULT = "";
        }

        $sql = "
            SELECT
                *
            FROM
                `{$table}`
            WHERE
                {$SQL_WHERE_DEFAULT}
                {$sql_where_custom}
        ";
        $this->query($sql);
        // ------------------------------

        $res = $this->fetch_row();
        $result = array();
        if(!empty($res))
        {
            $result['record'] = $this->_expand_record($res);
            if(!empty($this->fields))
            {
                $result['fields'] = $this->fields;
            }
        }
        return $result;
    }

    //============================================================================

    function _get_options()
    {
        foreach($this->fields as $key => $field)
        {
            if(isset($field['_table']) && isset($field['_values']))
            {
                $_delimiter = empty($field['_delimiter']) ? DEFAULT_FIELD_DELIMITER : $field['_delimiter'];
                $table = $field['_table'];
                $vals = $field['_values'];
                if(!is_array($vals)) $vals = array($vals);
                $keys = isset($field['_keys']) ? $field['_keys'] : DEFAULT_ID_FIELD;

                $sql = '
                    SELECT
                        `'.$keys.'` AS `_key`,';
                foreach($vals as $_key => $val)
                    $sql .= ' `'.$val.'` AS `_value_'.$_key.'`,';
                $sql = substr($sql, 0, -1);
                $sql .= '
                        FROM
                        `'.$table.'`
                        ';
                if(isset($field['_where']))
                {
                    if(is_array($field['_where']))
                    {
                        $where_parts = array();
                        foreach($field['_where'] as $f => $v)
                        {
                            $where_parts []= '`'.$table.'`.`'.$f.'` = "'.$v.'"';
                        }
                        $sql .= ' WHERE '.implode(', ', $where_parts);
                    }
                    else
                    {
                        $sql .= ' WHERE '.$field['_where'];
                    }
                }
                $this->query($sql);
                $this->fields[$key]['options'][''] = '- Select one -';
                while($row = $this->fetch_row())
                {
                    $values = array();
                    foreach($row as $_key => $val)
                        if(preg_match('/_value_\d/i', $_key) == 1)
                            $values []= $val;

                    $this->fields[$key]['options'][$row['_key']] = implode($_delimiter, $values);
                }
            }
        }
    }

    //============================================================================
    function _parse_fields()
    {
        if(!$this->_fields_parsed)
        {
            foreach($this->fields as $field => $attrs)
            {
                if(isset($attrs['type']) && ('date' == $attrs['type'] || 'datetime' == $attrs['type']))
                {
                    $attrs['dateformat'] = isset($attrs['dateformat']) ? $attrs['dateformat'] : DEFAULT_DATE_FORMAT;
                    if(!isset($attrs['display_days'])) $attrs['display_days'] = true;
                    if(!isset($attrs['display_months'])) $attrs['display_months'] = true;
                    if(!isset($attrs['display_years'])) $attrs['display_years'] = true;
                }
                if( isset($attrs['type']) && ('time' == $attrs['type']) )
                {
                    $attrs['timeformat'] = isset($attrs['timeformat']) ? $attrs['timeformat'] : DEFAULT_TIME_FORMAT;
                    if(!isset($attrs['display_hours'])) $attrs['display_hours'] = true;
                    if(!isset($attrs['display_minutes'])) $attrs['display_minutes'] = true;
                    if(!isset($attrs['display_seconds'])) $attrs['display_seconds'] = true;
                }
                $this->fields[$field] = $attrs;
            }
        }
    }

    //============================================================================

    function ExpandRecords($records)
    {
        foreach($records as $key => $record)
        {
            $records[$key] = $this->_expand_record($record);
        }

        return $records;
    }

    function _expand_record($record)
    {
//			$fields = array_keys($this->fields);
        $this->_parse_fields();
        $_record = array();
        $this->_get_options();

        foreach($this->fields as $field => $attrs)
        {
            if(isset($attrs['calculable']))
            {
                $_record[$field] = $this->fields[$field];
                // TODO: Process the calculable field
            }
            //if(isset($record[$field]))
            if(array_key_exists($field, $record))
            {
                if(isset($attrs['options']) && !empty($attrs['options']))
                {
                    if(isset($attrs['options'][$record[$field]]))
                    {
                        $attrs['_lookuped_value'] = $attrs['options'][$record[$field]];
                    }
                    else
                    {
                        $attrs['_lookuped_value'] = '';
                    }
                }
                else if( array_key_exists("type", $attrs) && $attrs["type"] == "checkbox"){
                    $attrs['_lookuped_value'] = ($record[$field] == 1)? YES_CONST : NO_CONST;
                }
                $attrs['value'] = $record[$field];
                $attrs['name'] = $field;
                $_record[$field] = $attrs;
            }
        }
        return $_record;
    }

    //============================================================================

    function CheckVariables(&$set)
    {
        //pa($set, $this->fields);
        $this->ResetErrors();
        foreach($this->fields as $field => $attrs)
        {
            if(isset($attrs['type']) && $attrs['type'] == 'date' && is_array($set[$field]))
            {
                if(!isset($set[$field]['Date_Day'])) $set[$field]['Date_Day'] = date('d');
                if(!isset($set[$field]['Date_Month'])) $set[$field]['Date_Month'] = date('m');
                if(!isset($set[$field]['Date_Year'])) $set[$field]['Date_Year'] = date('Y');
                $set[$field] =	$set[$field]['Date_Year'].'-'.
                        $set[$field]['Date_Month'].'-'.
                        $set[$field]['Date_Day'];
            }
        //pa($field, $attrs, $set[$field]);
            if( isset($attrs['required']) && ($attrs['required']) && (@$set[$field] == null) )
            {
                //pa($field, $attrs, $set[$field]);
                $this->AddError(ERR_FIELD_REQUIRED, array('field' => $attrs['title']));
            }
        }
    }

    //============================================================================

    function InsertRecord($set, $table = '')
    {
        // Error
        $this->ResetErrors();
        if(empty($set)) return;
        if($this->IsError()) return 0;
        $this->CheckVariables($set);
        //# used to support manuals error checking and outputting #//
        $this->JoinManualErrors();
        if($this->IsError()) return;

        $table = empty($table) ? $this->table : $table;

        $insert = array();
        foreach($this->fields as $field => $attrs)
        {
            if(isset($set[$field]) && !isset($attrs['calculable']))
            {
                $insert[$field] = addslashes($set[$field]);
            }
        }

        $sql = 'INSERT INTO `'.$table.'`(`'.implode('`, `', array_keys($insert)).'`)
                VALUES("'.implode('", "', $insert).'")';

        $this->query($sql);

        $this->last_insert_id = $this->get_insert_id();

        // Error
        if($this->last_insert_id == 0) return;

        return $this->last_insert_id;
    }

    //============================================================================

    function UpdateRecord($value, $set, $table = '', $_field = 'id')
    {
        //echo "<h1>CALLING UPDATE FUNC</h1>";
        if(empty($set)) return;
        $this->CheckVariables($set);
        $table = empty($table) ? $this->table : $table;
        //# used to support manuals error checking and outputting #//
        $this->JoinManualErrors();
        if($this->IsError()) return;
        $sql = '
            UPDATE
                `'.$table.'`
            SET
            ';

        foreach($this->fields as $field => $attrs)
        {
            //pa($field);
            if(isset($set[$field]) && !isset($attrs['calculable']) && $field != 'id')
            {
                $sql .= ' `'.$field.'` = "'.addslashes($set[$field]).'",';
            }
        }
        $sql = substr($sql, 0, -1);
        $sql .= ' WHERE `'.$_field.'` = "'.$value.'"';

        //exit;
        //echo "\n <pre>$sql </pre> <br/>\n";exit;

        $this->query($sql);
        return $value;
    }

    //# adds manuals errors to general errors cotext #//
    function JoinManualErrors()
    {
        if(!empty($this->errors_manual)){
            foreach($this->errors_manual as $val){
                $this->AddTextError($val);
            }
        }
    }
    //# oop way to add manual error #//
    function AddManualError($text)
    {
        if (!empty($text)){
            $this->errors_manual[] = $text;
        }
    }

    //============================================================================

    function _get_filter()
    {
        $filter = array();
        foreach($_POST as $key => $value)
        {
            $matches = array();
            if(preg_match('/^_filter_([\w_\s]+)$/i', $key, $matches) > 0)
            {
                $filter[$matches[1]] = $value;
            }
        }

        return $filter;
    }

    //============================================================================
    /*
     * Work with Trees
     * */
    //============================================================================

    function Linear2Tree($records = array())
    {
        return $this->_parse_tree($records);
    }

    function _parse_tree(&$recs, $pid = 0)
    {
        $tree = array();
        foreach($recs as $key => $rec)
        {
            if($rec['parent_id'] == $pid)
            {
                $tree[$key] = $rec;
                unset($recs[$key]);
            }
        }
        foreach($tree as $key => $rec)
        {
            $subitems  = $this->_parse_tree($recs, $rec['id']);
            if(!empty($subitems)) $tree[$key]['subitems'] = $subitems;
        }
        return $tree;
    }

    //============================================================================

    function GetPath($recs, $id = 0)
    {
        if($id == 0) return;
        $path = array();
        while($recs[$id]['parent_id'] != 0)
        {
            $p = $this->_get_parent($recs, $id);
            $id = $p['id'];
            $path []= $p;
        }

        return array_reverse($path);
    }

    //============================================================================

    function _get_parent($recs, $id)
    {
        return $recs[$recs[$id]['parent_id']];
    }

    //============================================================================

    /**
     * push VARS to fields array(set 'value' subfield)
     * @param	(array)$VARS -> vars to push
     * @param	(array)$fields -> fields where need push VARS
     */
    function push_vals_to_fields($VARS, &$fields)
    {
        foreach($fields as $k=>$v)
        {
            if(isset($VARS[$k]))
            {
                $fields[$k]['value'] = $VARS[$k];
            }
        }
    }

    //============================================================================
    function bFieldsGetInfoById($table, $id, $key="id", $fields_ar = array("id"),$multiple = false, $default_order=array() )
    {
        $fields_str = "";
        if ( is_array($fields_ar) ){
            $fields_str = implode(",", $fields_ar);
        }else{
            $fields_str = $fields_ar;
        }

        $sql = "SELECT $fields_str FROM $table WHERE `$key` = '$id'";
        if(!empty($default_order))
        {
            $order_parts = array();
            foreach($default_order as $okey => $ovalue)
            {
                $order_parts []= '`'.$okey.'` '.$ovalue;
            }
            $sql .= ' ORDER BY '.implode(', ', $order_parts);
        }
        //echo "<h2>SQL = $sql</h2>";

        $this->query($sql);

        if ($multiple == false){
            $row = $this->fetch_row();
            return $row;
        }else{
            $res = array();
            while ( $row = $this->fetch_row() ){
                $res[] = $row;
            }
            return $res;
        }
    }

    //============================================================================
    function BGetInfoById($table, $id, $key="id", $multiple = false, $default_order=array())
    {
        $table = !empty($table) ? $table : $this->table;
        $sql = "SELECT * FROM $table WHERE `$key` = '$id'";

        if(!empty($default_order))
        {
            $order_parts = array();

            foreach($default_order as $okey => $ovalue)
            {
                $order_parts []= '`'.$okey.'` '.$ovalue;
            }
            $sql .= ' ORDER BY '.implode(', ', $order_parts);
        }
        //echo "<h2>SQL = $sql</h2>";

        $this->query($sql);

        if ($multiple == false){
            $row = $this->fetch_row();
            return $row;
        }else{
            $res = array();
            while ( $row = $this->fetch_row() ){
                $res[] = $row;
            }
            return $res;
        }
    }

    //============================================================================



    function RunQueryWithPagination($sql_main, $sql_total)
    {
        $this->ResetErrors();
        $this->ResetMessages();
        $page = (isset($_GET['page'])) ? $_GET['page'] : 1;
        $sql_main .= ' LIMIT '.($page-1)*$this->per_page.', '.$this->per_page;
        //---------------------------------------------------------------------------------------
        $this->query($sql_main);
        $result = array();
        $_result = array();
        $total_recs = 0;
        while($row = $this->fetch_row())
        {
            $_result[]= $row;
        }
        if(!empty($_result))
        {
            $this->query($sql_total);
            $r = $this->fetch_row_numeric();
            $total_recs = $r[0];
        }

        foreach($_result as $key => $row)
        {
            $result['records'][$key] = $this->_expand_record($row);
        }

        if(!empty($this->fields))
        {
            $result['fields'] = $this->fields;
        }
        $result['pagination'] = $this->_pagination($total_recs, $this->per_page, $this->per_pagination, $page);

        return $result;
    }

    function ImplodeWhere($where)
    {
        $str = implode(" AND ", $where);
        return $str;
    }


    function RunSql($sql, $multiple=false, $array_key_field = null)
    {
        $this->query($sql);
        if ($multiple == true){
            $res = array();

            if(is_null($array_key_field)){
                while ( $row = $this->fetch_row() ){
                    $res[] = $row;
                }
            }
            //--- for ecample $array_key_field = event_id -> result array keys will have event_id as key  ---//
            else{
                while ( $row = $this->fetch_row() ){
                    $res[$row[$array_key_field]] = $row;
                }
            }
            return $res;
        }else{
            $row = $this->fetch_row();
            return $row;
        }
    }

//		// login(realname);
//		function BCreateUsername($uid,$first_login=true)
//		{
//			$uid = (int)$uid;
//			$sql = "SELECT b.login, t.real_name
//						FROM `base_user` b
//							LEFT JOIN `type_user1` t ON b.id = t.user_id
//						WHERE b.id =$uid ";
//			$res = $this->RunSql($sql);
//			if (!empty($res)){
//				if ($first_login == true){
//					$username = $res ['login']." (".$res['real_name'].")";
//				}else{
//					$username = $res ['real_name']." (".$res['login'].")";
//				}
//				return $username;
//			}else{
//				return "LOGIN_ERR";
//			}
//
//		}

    function BGetFullUserInfo($uid)
    {
        $sql = "SELECT *, b.id as `id`
                    FROM `base_user` b
                        LEFT JOIN `type_user1` t ON b.id = t.user_id
                    WHERE b.id =$uid ";
        $res = $this->RunSql($sql);
        return $res;
    }

    function CheckVariablesFrontend(&$set, &$err)
    {
        //pa($set, $this->fields);
        foreach($this->fields as $field => $attrs)
        {
            //--- check required vars ---//
            if( isset($attrs['required']) && ($attrs['required'] == true) )
            {
                $current_var = @$set[$field];
                $current_var = trim($current_var);
                if(empty($current_var)){
                    $err["$field"] = FIELD_EMPTY;
                }
            }
        }
    }
    //--- $path -> __FILE__, $fields_name -> "fields_$fields_name.inc"  ---//
    function IncludeSpecificFields($path, $fields_name)
    {
        //--- fix when path is applied only like a dir ---//
        if(is_dir($path)){
            $dir = $path;
        }else{
            $dir = dirname($path);
        }
        //$dir = dirname($path);

        $res = false;
        if(!empty($dir)){
            $dir = preg_replace("#(.+)[\\\\|/]+actions$#","\\1", $dir);
            $fields_name = "fields_{$fields_name}.inc";
            //--- try to include fields ---//
            $full_path = $dir."/$fields_name";
            if(file_exists($full_path)){
                $res = include $full_path;
            }
        }
        return $res;
    }

    //#------------------- exchange order functions -------------------#//

    //--------
    // $direction  - up | down
    // $sort_order - ASC | DESC(invert $direction in case of DESC)
    //--------
    function GetExchangedIds ($record_id, $table, $direction, $sort_order = "ASC", $pk_name = "id", $SQL_EXTRA = "")
    {
        $str_fields = " `$pk_name`, `sort_order` ";
        //--- get current item info ---//
        $sql = "
            select $str_fields
            from `$table`
            WHERE
                `$pk_name` = '$record_id'
        ";
        $current_item = $this->RunSql($sql);
        //pa($current_item);

        $ret_val = array("from" =>"", "to"=>"");
        //--- if sort_order is DESC - invert $direction AND SIGN to do correct searching adjacent item ---//
        $COMPARE_SIGN = "";
        if($sort_order == "DESC"){
            if($direction == "up"){
                $direction = "down";
                $COMPARE_SIGN = " > ";
            }elseif($direction == "down"){
                $direction = "up";
                $COMPARE_SIGN = " < ";
            }
        }

        switch ($direction){
            case "up":
                $SQL_SORT_ORDER_SIGN = (!empty($COMPARE_SIGN))? $COMPARE_SIGN : " < " ;
                $SQL_SORT_ORDER_TYPE = "DESC";
            break;
            case "down":
                $SQL_SORT_ORDER_SIGN = (!empty($COMPARE_SIGN))? $COMPARE_SIGN : " > ";
                $SQL_SORT_ORDER_TYPE = "ASC";
            break;
        }

        //--- get record with bigger sort_order ---//
        $sql = "
            SELECT $str_fields
            FROM `$table`
            WHERE
                1=1 AND
                `sort_order` $SQL_SORT_ORDER_SIGN '{$current_item["sort_order"]}'
                $SQL_EXTRA
            ORDER BY
                `sort_order` $SQL_SORT_ORDER_TYPE
            LIMIT 0, 1
        ";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $changing_item = $this->RunSql($sql);
        //pa($changing_item);
        if(!empty($changing_item)){
            $ret_val["from"] = $current_item[$pk_name];
            $ret_val["to"] = $changing_item[$pk_name];
            return $ret_val;
        }else{
            return "none";
        }
    }

    //--------
    // pk_name - primary key
    //--------
    function DoExchange($exchange_ids, $table, $pk_name = "id")
    {
        //$pk_name = "id_channel";
        $tmp = $this->BGetInfoById($table,  $exchange_ids["from"], $pk_name);
        $from_order = $tmp["sort_order"];
        $tmp1 = $this->BGetInfoById($table,  $exchange_ids["to"], $pk_name);
        $to_order = $tmp1["sort_order"];

        ///from
        $sql = "
            UPDATE `$table`
            SET `sort_order` = '$from_order'
            WHERE
                `$pk_name` = '".$exchange_ids["to"]."' ";
        $this->query ($sql);

        ///to
        $sql = "
            UPDATE `$table`
            SET `sort_order` = '$to_order'
            WHERE
                `$pk_name` = '".$exchange_ids["from"]."' ";
        $this->query ($sql);
    }

    //--------
    // max order for specific table
    //--------
    function GetMaxOrder($table, $SQL_EXTRA = "", $add_step = null)
    {
        $sql = "
            SELECT MAX(`sort_order`) as `max_order`
            FROM `$table`
            WHERE
                1=1
                $SQL_EXTRA
        ";
        $row = $this->RunSql($sql);
        if(!is_null($add_step)){
            $row['max_order'] += intval($add_step);
        }
        return $row['max_order'];
    }

    function transactionStart()
    {
        if(self::isTransactionOpened() == false){
            self::setIsTransactionOpened(true);
        }else{
            EkvDebug::debugTransaction();
            throw new Exception("Nested transaction is being started!!! ");
        }
        //pa(debug_backtrace());
        $this->query("BEGIN");
    }
    function transactionCommit()
    {
        $this->query("COMMIT");
        self::setIsTransactionOpened(false);
    }
    function transactionRollback()
    {
        $this->query("ROLLBACK");
        self::setIsTransactionOpened(false);
    }
    function TS(){$this->transactionStart();}
    function TC(){$this->transactionCommit();}
    function TR(){$this->transactionRollback();}

    //#------------------- trans silent -------------------#//

    /**
     * silent means if other trans is already opened - don't throw exception
     * but silently ignore inner transactions
     * may be usefull for quite deep inner methods
     * @return bool
     */
    function transStartSilent()
    {
        if(self::isTransactionOpened()){
            $this->_ignoreFurtherInnerTrans = true;
            return false;
        }else{
            $this->transactionStart();
            return true;
        }
    }

    function transCommitSilent()
    {
        $res = false;
        if($this->_ignoreFurtherInnerTrans == false){
            $this->transactionCommit();
            $this->_ignoreFurtherInnerTrans = false;
            $res = true;
        }
        return $res;
    }

    function transRollbackSilent()
    {
        $res = false;
        if($this->_ignoreFurtherInnerTrans == false){
            $this->transactionRollback();
            $this->_ignoreFurtherInnerTrans = false;
            $res = true;
        }
        return $res;
    }


    function simmulateNativeResultSet($plain_results, $fields = null)
    {
        $result = array();
        if(is_null($fields) == false){
            $this->fields = $fields;
        }

        $result["records"] = $this->ExpandRecords($plain_results);
        $result["fields"] = $this->fields;

        return $result;
    }

}
