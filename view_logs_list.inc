<?php

function view_logs_list(){
		
	
	$query = "select tl.id, tl.update_time, tl.updated_by, tl.updated_content, tr.language, td.foss_category, td.tutorial_level, td.tutorial_name from tutorial_update_log tl, tutorial_resources tr, tutorial_details td where tl.tutorial_resources_id=tr.id and tr.tutorial_detail_id=td.id order by tl.update_time ASC";
	$sql_count = "SELECT count(*) FROM tutorial_update_log tl, tutorial_resources tr, tutorial_details td where tl.tutorial_resources_id=tr.id and tr.tutorial_detail_id=td.id";
	$header = array('Sno', 'FOSS', 'Level', 'Tutorial Name', 'Language', 'Time', 'User', 'Action');
	$result = pager_query($query, 30, 0, $sql_count);
	$row = array();
	$destination = drupal_get_destination();
	while ($item = db_fetch_object($result)) {
		$row[] = array($item -> id, $item->foss_category, $item->tutorial_level, $item->tutorial_name, $item->language, $item -> update_time, $item -> updated_by, $item -> updated_content);
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
