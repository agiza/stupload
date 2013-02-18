<?php 
	$ip = '10.209.4.';
	if (substr($_SERVER['REMOTE_ADDR'], 0, strlen($ip)) === $ip) {
		$wiki_path = "http://www.spoken-tutorial.org/script/index.php/";
		$tutorial_path = "http://www.spoken-tutorial.org/sites/default/files/";
		$file_upload_path = '/var/www/html/spoken_tutorial_org/sites/default/files/';
	}
	else
	{
		$wiki_path = "http://www.spoken-tutorial.iitb.ac.in/script/index.php/";
		$tutorial_path = "http://www.spoken-tutorial.iitb.ac.in/sites/default/files/";
		$file_upload_path = '/var/www/html/spoken_tutorial_org/sites/default/files/';
	}
?>

