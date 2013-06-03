<?php
	
	// Public review form
	function public_review_form($form_state, $tdid, $trid, $video_time){
		if(isset($tdid) && $tdid > 0 && isset($trid) && $trid > 0 && $video_time != '') {
			$path = drupal_get_path('module', 'stupload');
			drupal_add_js($path . '/problem_report.js', 'module');
			
			$time = pr_set_video_time($video_time);
			$form['#attributes'] = array('name' => 'problem_report','enctype' => "multipart/form-data", 'class' => 'problem_report', 'onSubmit' => 'return validate_report();');
		
			$form['tdid'] = array(
				'#type' => 'hidden',
				'#value' => $tdid
			);
			$form['trid'] = array(
				'#type' => 'hidden',
				'#value' => $trid
			);
			$form['resource'] = array(
				'#type' => 'select',
				'#title' => t('Resource'),
				'#options' => array('outline' => 'Outline', 'script' => 'Script', 'video' => 'Video'),
				'#attributes' => array('class' => 'rptutorial-resource'),
				'#prefix' => '<div class="presource-div">',
				'#suffix' => '</div>'
			);
			$form['prvideo_min'] = array(
				'#type' => 'select',
				'#options' => $time['min'],
				'#attributes' => array('class' => 'prvideo_min'),
				'#prefix' => "<div class='date-and-time prvideo-time'><label for='edit-pref-wkshop-time'>Select Time: <span class='form-required' title='This field is required.'>*</span></label>",
			);
			$form['prvideo_sec'] = array(
				'#type' => 'select',
				'#options' => $time['sec'],
				'#attributes' => array('class' => 'prvideo_sec'),
				'#suffix' => "</div>"
			);
			$form['missing_comment'] = array(
				'#type' => 'textarea',
				'#title' => t('Enter your comment here'),
				'#prefix' => '<div class="rpmissing_comment-div">',
				'#suffix' => '</div>'
			);
			$form['save'] = array(
				'#type' => 'submit',
				'#value' => 'Post'
			);
	
			return $form;
		}else {
			drupal_goto($_SERVER['HTTP_REFERER']);
		}
	}

	function public_review_form_submit($form, &$form_state){
		global $user;
		$video_time = '';
		$content = $form_state['values']['missing_comment'];
		if($form_state['values']['resource'] == 'video'){
			$video_time = '00:'.$form_state['values']['prvideo_min'].":".$form_state['values']['prvideo_sec'];
		}
		$query = "insert into tutorial_public_review (uid, trid, date_time, component, video_time, comment) values (".$user->uid.", ".$form_state['values']['trid'].", NOW(), '".$form_state['values']['resource']."', '".$video_time."', '".$content."')";
		if(db_query($query)){
			drupal_set_message('Your comment added successfully! Thank you for your valuable feedback.');
			drupal_goto('show_video', array('tr' => $form_state['values']['trid'], 'sval' => '1'));
			return;
		}
	}
	
	function pr_set_video_time($video_time){
		$vta = explode(':', $video_time);
		$min = '';
		$sec = '';
		$i=0;
		while($i <= intval($vta[1])){
			if($i < 10){
				$min['0'.$i] = '0'.$i;
			}else{
				$min[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$sec['0'.$i] = '0'.$i;
			}else{
				$sec[$i] = $i;
			}
			$i++;
		}
		$time['min'] = $min;
		$time['sec'] = $sec;
		return $time;
	}
// public review list
function public_review_list(){
	$query = '';
	$sql_count = '';
	$sfoss = isset($_REQUEST['foss']) ? $_REQUEST['foss'] : '';
	$slang = isset($_REQUEST['lang']) ? $_REQUEST['lang'] : '';
	if($sfoss && $slang){
		$query= "select tr.id as trid, td.tutorial_level, td.tutorial_name from tutorial_resources tr, tutorial_details td where tr.tutorial_status='public_review' and td.id=tr.tutorial_detail_id and td.foss_category='".$_REQUEST['foss']."' and tr.language='".$_REQUEST['lang']."'";
		$sql_count= "select count(*) from tutorial_resources tr, tutorial_details td where tr.tutorial_status='public_review' and td.id=tr.tutorial_detail_id and td.foss_category='".$_REQUEST['foss']."' and tr.language='".$_REQUEST['lang']."'";
		$result = pager_query($query, 30, 0, $sql_count);
		$row = array();
		$header = array('Level', 'Tutorial Name', 'Action');
		while ($item = db_fetch_object($result)) {
			$link_str = l(t('view'), 'stupload/public_review_view/'. $item->trid , array('attributes' => array('class' => 'view-record', 'title' => 'View')));
			$row[] = array($item -> tutorial_level, $item -> tutorial_name, $link_str);
		
		}

		$output = drupal_get_form('public_review_list_form', $sfoss, $slang);		
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		}else{
			$output .= theme('table', $header, $row);
			$output .= theme('pager');
		}
	}else{
		$output = drupal_get_form('public_review_list_form', $sfoss, $slang);
	}
	
	return $output;
}	
function public_review_list_form($form_state, $sfoss, $slang){
	global $user;
	$path = drupal_get_path('module', 'stupload');
	drupal_add_css($path.'/stupload-view.css');
	
	//fetching list of foss category details
	$query = "select distinct foss_category from tutorial_details ORDER BY foss_category ASC";
	$result = db_query($query);
	$foss_list = array();
	$foss_list[''] = "Select FOSS Category";
	while($row = db_fetch_object($result)){
		$foss_list[$row->foss_category] = $row->foss_category;
	}
	
	$query ="select distinct tl.name from  tutorial_languages tl, tutorial_resources tr where tr.language=tl.name";
	$result = db_query($query);
	$lang[''] = 'Select Language';
	while($row = db_fetch_object($result)){
		$lang[$row->name] = $row->name;
	}
	unset($lang['English']);
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['foss'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $foss_list,
		'#default_value' => $sfoss,
		'#attributes' => array('class' => 'foss_category'),
		'#prefix' => '<br /><center><b>Select FOSS Category, Select Language and Search for a list of Tutorials</b></center><div style="width: 70%; margin: auto;"><table class="foss_search" align="Center" style="border-collapse: inherit; width: 70%;" cellpadding="7"><tr><td>',
		'#suffix' => '</td>'
	);
	
	$form['lang'] = array(
		'#type' => 'select',
		'#options' => $lang,
		'#default_value' => $slang,
		'#attributes' => array('class' => 'language'),
		'#prefix' => '<td>',
		'#suffix' => '</td>'
	);
	$form['save'] = array(
		'#type' => 'submit',
		'#value' => 'Search',
		'#attributes' => array('class' => 'form-select locate_tutorial'),
		'#prefix' => '<td>',
		'#suffix' => '</td></tr></table></div>'
	);
	
	return $form;
}

function public_review_list_form_validate($form, &$form_state){

	if($form_state['values']['foss'] == '')
		form_set_error('foss', t('Please select FOSS category'));
	if($form_state['values']['lang'] == '')
		form_set_error('lang', t('Please select language'));
}

function public_review_list_form_submit($form, &$form_state){
	$redirect = 'stupload/public_review_list';
	$query = array('foss' => $form_state['values']['foss'], 'lang'=> $form_state['values']['lang'] );
	drupal_goto($redirect, $query, $fragment = NULL, $http_response_code = 302);
}

function public_review_view($id=NULL){
	if($id){
		$query = "select * from tutorial_public_review where trid=".$id;
		$sql_count="select count(*) from tutorial_public_review where trid=".$id;
		$row = array();
		$header =array(
			array('data'=>'Component', 'field'=>'component', 'sort'=>'asc'),
			array('data'=>'Date/time', 'field'=>'date_time'),
			array('data'=>'Comment', 'field'=>'comment'),
		);
		$query .= tablesort_sql($header);
		$result = pager_query($query, 30, 0, $count_query);
		while ($item = db_fetch_object($result)) {
			if($item -> component == 'video'){
				$row[] = array($item -> component, $item -> date_time, "<p>Video time : ".$item->video_time."</p>".$item -> comment);
			}else{
				$row[] = array($item -> component, $item -> date_time, $item -> comment);
			}		
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">There is no comment.</div>';
		}else{
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}
	}else{
		$output = "<p>Somthing went worng, Please try again</p>";
	}
	return $output;
}
