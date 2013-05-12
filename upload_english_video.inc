<?php
	function upload_english_video() {		
		if($_POST['trid'] && $_POST['td_path'] && $_REQUEST['redirection']){		
			$output = "<h2>Upload English Video for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('upload_english_video_form', $_REQUEST['trid'], $_POST['td_path'], $_REQUEST['redirection']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}
	function upload_english_video_form($form_state, $trid, $td_path, $redirection) {
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
		$form['td_path'] = array(
			'#type' => 'hidden',
			'#value' => $td_path,
		);
		$form['redirection'] = array(
			'#type' => 'hidden',
			'#value' => $redirection,
		);
		$form['video_status'] = array(
			'#type' => 'select',
			'#title' => t('Video Status'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required' , 1 => 'Will upload later'),
			'#attributes' => array('class' => 'upeng_video_status'),
		);
		$form['video'] = array(
			'#type' => 'file',
			'#title' => t('Video <span class="upload-help-test"> (Upload .ogv file only)</span>'),
			'#attributes' => array('class' => 'upeng_video'),
			'#prefix' => '<div class="stupload-form-video">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}	
	
	function upload_english_video_form_validate($form, &$form_state){
		if ($form_state['values']['video_status'] == 0){
			if($_FILES['files']['tmp_name']['video'] == ''){
				form_set_error('video', t('Please select the video file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.')); // get file name
				$filesize = $_FILES['files']['size']['video'];
				$allowed_file_types = explode(',', variable_get('video_extensions', ''));
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('video', t('Please select proper file. ogv files only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_english_video_form_submit($form, &$form_state){
		global $user;
		$td = explode('/', $_POST['td_path']);
		$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.'));
		
		$file_name = $td[2].'-English.'.$file_ext;
		$tutorial_video = 'st_videos/'.$_POST['td_path'].$file_name;
		$path = variable_get('file_upload_path', '').$tutorial_video;
		
		// upload later
		if($form_state['values']['video_status'] == 1){
			$query = "UPDATE tutorial_resources SET tutorial_video_uid=".$user->uid.", tutorial_video_status=0, cvideo_version=1, tutorial_video='pending' WHERE id='".$form_state['values']['trid']."'";
			db_query($query);
			// to track uploader 
			$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

			if(db_query($query)){
				drupal_set_message('Tutorial Video Updated Successfully');
				$query= array('trid' => $form_state['values']['trid']);
				if($form_state['values']['redirection'] == 1){
					drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
				} elseif($form_state['values']['redirection'] == 2){
					drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
				}
			}else {
				drupal_set_message('Error while Adding Tutorial Video, Please try again.');
			}
		}
		if($_FILES['files']['size']['video'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['video'], $path)){
				// insert into common data table
				$query = "UPDATE tutorial_resources SET tutorial_video_uid=".$user->uid.", tutorial_video_status=1, cvideo_version=1, tutorial_video='".$tutorial_video."' WHERE id='".$form_state['values']['trid']."'";
				db_query($query);
				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					//Mail to contributor
					$a = new stdClass();
					$a->tc = 'Video';					
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				    $mailto = $user->mail;
				    $role = variable_get('contributor', '');
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					//Mail to admin_review_user
					$role = variable_get('quality_checker', '');
					$quality_mailid = get_language_role_emails($tutorial_detail->language);
					$role = variable_get('admin_review_user', '');
					$admin_mailid = get_role_emails($role);

					$mailto = $admin_mailid.$quality_mailid;

				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					drupal_set_message('Tutorial Video Updated Successfully');
					$query= array('trid' => $form_state['values']['trid']);
					if($form_state['values']['redirection'] == 1){
						drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					} elseif($form_state['values']['redirection'] == 2){
						drupal_goto('stupload/update_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
					}
				}else {
					drupal_set_message('Error while Adding Tutorial Video, Please try again.');
				}
			}else{
				drupal_set_message('Error while Uploading Video file, Please try again..');
			}
		}
	}
?>
