<?php
	function upload_english_slide() {
		if($_REQUEST['trid'] && $_REQUEST['tccid']){		
			$output = "<h2>Upload English Slide for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('upload_english_slide_form', $_REQUEST['trid'], $_REQUEST['tccid']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}

	}

	function upload_english_slide_form($form_state, $trid, $tccid) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);
		$form['tccid'] = array(
			'#type' => 'hidden',
			'#value' => $tccid,
		);

		$form['slide_status'] = array(
			'#type' => 'select',
			'#title' => t('Slide Status'),
			'#required' => TRUE,
			'#options' => array('' => 'Slide', 0 => 'Required' , 1 => 'Will upload later'),
			'#attributes' => array('class' => 'upeng_slide_status'),
		);
		$form['slide'] = array(
			'#type' => 'file',
			'#title' => t('Slide <span class="upload-help-test"> (Upload .zip file only)</span>'),
			'#attributes' => array('class' => 'upeng_slide'),
			'#prefix' => '<div class="stupload-form-slide">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_slide_form_validate($form, &$form_state){
		if ($form_state['values']['slide_status'] == 0){
			if($_FILES['files']['tmp_name']['slide'] == ''){
				form_set_error('slide', t('Please select the slide file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['slide'], strripos($_FILES['files']['name']['slide'], '.')); // get file name
				$filesize = $_FILES['files']['size']['slide'];
				$allowed_file_types = array('.zip');
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('slide', t('Please select proper file. zip file only allowed.'));
				}
			}
		}
	  	return;
	}
	
	function upload_english_slide_form_submit($form, &$form_state){

		// select Tutorial categoty
		$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
		$result = db_query($query);

		$tutorial_detail = db_fetch_object($result);

		$slide_path = 'st_videos/'.$tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/Resource/';
		require_once('path.config.inc');
		$path = $file_upload_path.$slide_path;
		$file_ext = substr($_FILES['files']['name']['slide'], strripos($_FILES['files']['name']['slide'], '.'));
		$file_name = $tutorial_detail->tutorial_name.'-Slide-'.$tutorial_detail->language.$file_ext;
		
		// for upload later
		if ($form_state['values']['slide_status'] == 1){
			$tutorial_slide = 'pending';
			// insert into common data table
			$query = "UPDATE tutorial_common_contents SET tutorial_slide='".$tutorial_slide."' WHERE id='".$form_state['values']['tccid']."'";
			db_query($query);

			// to track uploader 
			$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

			if(db_query($query)){
				drupal_set_message('Tutorial Slide Added Successfully');
				$redirect = 'stupload/english_index/?tr='.$form_state['values']['trid'];
				drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
			}else {
				drupal_set_message('Error while Adding Slide, Please try again.');
			}
			
		}
		// for upload file
		if($_FILES['files']['size']['slide'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['slide'], $path.$file_name)){
				global $user;

				$tutorial_slide = $slide_path.$file_name;
				// insert into common data table
				$query = "UPDATE tutorial_common_contents SET tutorial_slide='".$tutorial_slide."' WHERE id='".$form_state['values']['tccid']."'";
				db_query($query);

				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					drupal_set_message('Tutorial Slide Added Successfully');
					$redirect = 'stupload/english_index/?tr='.$form_state['values']['trid'];
					drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
				}else {
					drupal_set_message('Error while Adding Slide, Please try again.');
				}
			}else{
				drupal_set_message('Error while Slide, Please try again..');
			}
		}

}

?>
