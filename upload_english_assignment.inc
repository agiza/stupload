<?php
	function upload_english_assignment() {
		if($_REQUEST['trid'] && $_REQUEST['tccid']){
			$output .= drupal_get_form('upload_english_assignment_form', $_REQUEST['trid'], $_REQUEST['tccid']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}


	}


	function upload_english_assignment_form($form_state, $trid, $tccid) {
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

		$form['asgmnt_status'] = array(
			'#type' => 'select',
			'#title' => t('Assignment Status'),
			'#required' => TRUE,
			'#options' => array('' => 'Assignment', 0 => 'Required' , 1 => 'Will upload later', 2 => 'Not Required'),
			'#attributes' => array('class' => 'upeng_asgmnt_status'),
		);
		$form['assignment'] = array(
			'#type' => 'file',
			'#title' => t('Assignment'),
			'#attributes' => array('class' => 'upeng_asgmnt'),
			'#prefix' => '<div class="stupload-form-asgmnt">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_assignment_form_validate($form, &$form_state){
		if ($form_state['values']['asgmnt_status'] == 0){
			if($_FILES['files']['tmp_name']['assignment'] == ''){
				form_set_error('codefile', t('Please select the codefile file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['assignment'], strripos($_FILES['files']['name']['assignment'], '.')); // get file name
				$filesize = $_FILES['files']['size']['assignment'];
				$allowed_file_types = array('.txt', '.pdf');
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('assignment', t('Please select proper file. txt and pdf files only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_english_assignment_form_submit($form, &$form_state){

		// select Tutorial categoty
		$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";

		$result = db_query($query);
		$tutorial_detail = db_fetch_object($result);

		$codefile_path = 'st_videos/'.$tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/Resource/';
		require_once('path.config.inc');
		$path = $file_upload_path.$codefile_path;
		$file_ext = substr($_FILES['files']['name']['assignment'], strripos($_FILES['files']['name']['assignment'], '.'));
		$file_name = $tutorial_detail->tutorial_name.'-Assignment-'.$tutorial_detail->language.$file_ext;
	
		if($_FILES['files']['size']['assignment'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['assignment'], $path.$file_name)){
				global $user;

				$tutorial_codefile = $codefile_path.$file_name;
				// insert into common data table
				$query = "UPDATE tutorial_common_contents SET tutorial_assignment='".$tutorial_codefile."' WHERE id='".$form_state['values']['tccid']."'";
				db_query($query);

				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					drupal_set_message('Tutorial Codefile Adding Successfully');
					$redirect = 'stupload/english_index/?tr='.$form_state['values']['trid'];
					drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
				}else {
					drupal_set_message('Error while Adding Assignment, Please try again.');
				}
			}else{
				drupal_set_message('Error while Adding Assignment, Please try again..');
			}
		}

	}

?>
