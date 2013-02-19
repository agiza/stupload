<?php
	function update_video() {
		if($_POST){
			$output = "<h2>Update Video for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('update_video_form', $_POST['trid']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}

	function update_video_form($form_state, $trid) {
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

		$form['video'] = array(
			'#type' => 'file',
			'#title' => t('Video <span class="upload-help-test"> (Upload .ogv file only)</span>'),
			'#attributes' => array('class' => 'upeng_video'),
			'#prefix' => '<div class="stupload-form-videou">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}	
	
	function update_video_form_validate($form, &$form_state){
		if($_FILES['files']['tmp_name']['video'] == ''){
			form_set_error('video', t('Please select the video file.'));
		}else{
			$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.')); // get file name
			$filesize = $_FILES['files']['size']['video'];
			$allowed_file_types = array('.ogv');
			if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
				form_set_error('video', t('Please select proper file. ogv files only allowed.'));
			}
		}
		
	  	return;
	}
	
	function update_video_form_submit($form, &$form_state){

		// select Tutorial categoty
		$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";

		$tdresult = db_query($query);
		$tutorial_detail = db_fetch_object($tdresult);

		$video_path = 'st_videos/'.$tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/';
		require_once('path.config.inc');
		$path = $file_upload_path.$video_path;
		$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.'));
		$file_name = $tutorial_detail->tutorial_name.'-'.$tutorial_detail->language.$file_ext;
	
		if($_FILES['files']['size']['video'] > 0){
			$bac_file_name = 'backup_'.$tutorial_detail->tutorial_name.'-'.$tutorial_detail->language.$file_ext;
			if(file_exists($path.$file_name) && !rename($path.$file_name, $path.$bac_file_name)){
				drupal_set_message('Unable to get backup file');
			}
			if(move_uploaded_file($_FILES['files']['tmp_name']['video'], $path.$file_name)){

				// delete backup file
				if(file_exists($path.$bac_file_name) && !unlink($path.$bac_file_name)){
					drupal_set_message('Unable to delete backup file');
				}
				global $user;

				$tutorial_video = $video_path.$file_name;
				// insert into common data table
				$query = "UPDATE tutorial_resources SET tutorial_status='pending', tutorial_video='".$tutorial_video."' WHERE id='".$form_state['values']['trid']."'";
				db_query($query);

				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					//Mail to contributor
					$a = new stdClass();
					$a->tc = 'Video';					
					$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);
				    $mailto = 'k.sanmugam2@gmail.com';
				    $role = 'contributor';
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);

					//Mail to admin_review_user 
					$role = 'quality_checker';
					$quality_mailid = get_role_emails($role);

					$role = 'admin_review_user';
					$admin_mailid = get_role_emails($role);

					$mailto = $admin_mailid.$quality_mailid;
				    $message = "";
					stupload_notify($mailto, $role, $message, $tds, $user->name);
					
					drupal_set_message('Tutorial Video Updated Successfully');
					$redirect = 'stupload/update_index/?tr='.$form_state['values']['trid'];
					drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
				}else {
					if(file_exists($path.$bac_file_name)){
						rename($path.$bac_file_name, $path.$file_name);
					}
					drupal_set_message('Error while Adding Updating Video, Please try again.');
				}
			}else{
				drupal_set_message('Error while Updating Video, Please try again..');
			}
		}

}

?>
