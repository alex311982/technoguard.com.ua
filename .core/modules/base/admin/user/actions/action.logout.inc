<?php

	$this->user->Logout();
	unset($_SESSION['user']);
	$url = compose_url_backend('/admin/');
	force_location($url);

?>
