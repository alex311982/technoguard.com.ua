<?php
//pa("Smarty_Resource_Common");

class Smarty_Resource_Common extends Smarty_Resource_Custom {
    // PDO instance
    protected $db;
    // prepared fetch() statement
    protected $fetch;
    // prepared fetchTimestamp() statement
    protected $mtime;

    public function __construct() {
//        try {
//            $this->db = new PDO("mysql:dbname=test;host=127.0.0.1", "smarty", "smarty");
//        } catch (PDOException $e) {
//            throw new SmartyException('Mysql Resource failed: ' . $e->getMessage());
//        }
//        $this->fetch = $this->db->prepare('SELECT modified, source FROM templates WHERE name = :name');
//        $this->mtime = $this->db->prepare('SELECT modified FROM templates WHERE name = :name');
    }
    
    /**
     * Fetch a template and its modification time from database
     *
     * @param string $name template name
     * @param string $source template source
     * @param integer $mtime template modification timestamp (epoch)
     * @return void
     */
    protected function fetch($name, &$source, &$mtime)
    {
        $filename = $this->_get_tpl_filename($name);
        if(!empty($filename))
        {
              $source = file_get_contents($filename);
        }

    }
    
    /**
     * Fetch a template's modification time from database
     *
     * @note implementing this method is optional. Only implement it if modification times can be accessed faster than loading the comple template source.
     * @param string $name template name
     * @return integer timestamp (epoch) the template was modified
     */
    protected function fetchTimestamp($name) {
//        $this->mtime->execute(array('name' => $name));
//        $mtime = $this->mtime->fetchColumn();
//        $this->mtime->closeCursor();
//        return strtotime($mtime);

        $filename = $this->_get_tpl_filename($name);
        $tpl_timestamp = filemtime($filename);
        return $tpl_timestamp;
    }


    private function _get_tpl_filename($tpl_name)
    	{
    	    $parts = preg_split('/\//', $tpl_name);
            $site_core_view = SITE_CORE_VIEW;
            $site_core_view_fake = EkvCommonHelper::getSiteCoreViewFake();
            /**
             * sometimes it's required to process frontend tpls from backend environment so we need to make fake core
             * to make proper path for including
             */
            if(is_null($site_core_view_fake) == false){
                $site_core_view = $site_core_view_fake;
            }

            $use_theme = EkvCommonHelper::getUseTplTheme();

            $_templates_path = "";
            //#------------------- no theme or backend view -> use common usual templates -------------------#//
            if($use_theme == false || $site_core_view == SITE_BACKEND){
                $_templates_path = PATH_TEMPLATES . DIRECTORY_SEPARATOR . $site_core_view;
            }
            //#------------------- use_theme = true and view = frontend -> use old theme -------------------#//
            else if($use_theme && $site_core_view == SITE_FRONTEND){
                $_templates_path = PATH_TEMPLATES . DIR_SEP . 'theme_old';
            }
    //        $_templates_path = PATH_TEMPLATES . DIRECTORY_SEPARATOR . $site_core_view;

    	    $_tpl_filename = '';
    	    if(1 == count($parts))
    	    {
    	    	if(file_exists($_templates_path . DIRECTORY_SEPARATOR . 'common' . DIRECTORY_SEPARATOR . $tpl_name))
    	    	{
    	    		$_tpl_filename = $_templates_path . DIRECTORY_SEPARATOR . 'common' . DIRECTORY_SEPARATOR . $tpl_name;
    	    	}
    	    }
    	    elseif(3 == count($parts))
    	    {
    	    	$_module = $parts[0];
    	    	$_entity = $parts[1];
    	    	$_tpl = $parts[2];

    			$_tpl_filename = $_templates_path . DIRECTORY_SEPARATOR . $_module . DIRECTORY_SEPARATOR . $_entity . DIRECTORY_SEPARATOR . $_tpl;
    	    	if(!file_exists($_tpl_filename))
    	    	{
    	    		$_tpl_filename = '';
    	    	}
    	    }
    	    elseif(count($parts) > 3)
    	    {
    	    	$_module = $parts[0];
    	    	$_entity = $parts[1];
    	    	$_tpl = $parts[2].DIRECTORY_SEPARATOR.$parts[3];

    			$_tpl_filename = $_templates_path . DIRECTORY_SEPARATOR . $_module . DIRECTORY_SEPARATOR . $_entity . DIRECTORY_SEPARATOR . $_tpl;
    	    	if(!file_exists($_tpl_filename))
    	    	{
    	    		$_tpl_filename = '';
    	    	}
    	    }
        	return $_tpl_filename;
    	}

}
