<?php
/**
 * ============================================================
 * File name: action.logout.inc
 * ============================================================
 */


	EkvUser::doLogout();
	//$this->force_referer();
	force_location("/");
	exit();

?>