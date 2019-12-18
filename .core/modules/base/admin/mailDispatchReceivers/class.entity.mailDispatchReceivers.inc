<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */

 	class basemailDispatchReceivers extends Entity
	{
		function __construct()
		{
			parent::__construct();
		}

        function ManualAfterDbInit()
        {
            EkvAdminLog::initLogParams($this, "dispatch_receivers");
        }

        function manageAccessDependencies()
        {
            if(EkvBackendAdmins::isSuperAdmin($this) == false){

                if($this->action == "browse"){
                    if(EkvBackendAdmins::getPageAccessItem("can_edit") == 0){
                        unset($this->DB->fields["email"]["href"]);
                    }
                }
                else if($this->action == "save"){
                    EkvBackendAdmins::checkStandartSaveActionRestrictions();
                }
            }
        }

		function SetBreadcrump()
		{
			$breadcrumps = array();
			$title = "Рассылка. Получатели";
			$breadcrumps []= array('title' => $title,
									'href' => 'browse');

			return $breadcrumps;
		}
		
		
		
	}

?>
