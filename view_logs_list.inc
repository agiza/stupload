<?php

function view_logs_list(){
		
	
	$query = "select * from {tutorial_update_log} order by id DESC";
	$sql_count = "SELECT count(*) FROM node";
	$header = array('Sno', 'Update Time', 'User', 'Content Type');
	$tablesort = tablesort_sql($header);
	$result = pager_query($query.$tablesort, 2, 0, NULL);
	$row = array();
	$destination = drupal_get_destination();
	while ($item = db_fetch_object($result)) {
		$row[] = array($item -> id, $item -> update_time, $item -> updated_by, $item -> updated_content);
	}
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">Log file is empty.</div>';
	} else {
		drupal_set_message($message);
		$output .= theme('table', $header, $row);
		$output .= theme('pager');
	}
	return $output;
}
