<?php
	function upload_olang_video($params) {
		if($params){
			$params = parse_url($params);
			$tr_data = explode('=', $params['query']);

			$trid = $tr_data[1];

			$output .= drupal_get_form('upload_olang_video_form', $trid);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}

	}


	function upload_olang_video_form($form_state, $trid) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		
		$form['tutorial_resource_id'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);

		$form['video_status'] = array(
			'#type' => 'select',
			'#title' => t('Slide Status'),
			'#required' => TRUE,
			'#options' => array('' => 'Video', 0 => 'Required' , 1 => 'Will upload later'),
			'#attributes' => array('class' => 'upeng_video_status'),
		);
		$form['video'] = array(
			'#type' => 'file',
			'#title' => t('Video'),
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
	
	
	function upload_olang_video_form_validate($form, &$form_state){
		if ($form_state['values']['video_status'] == 0){
			if($_FILES['files']['tmp_name']['video'] == ''){
				form_set_error('video', t('Please select the video file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.')); // get file name
				$filesize = $_FILES['files']['size']['video'];
				$allowed_file_types = array('.mp4', '.ogv');
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('slide', t('Please select proper file. ogv and mp4 files only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_olang_video_form_submit($form, &$form_state){



		// select Tutorial categoty
		$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['tutorial_resource_id']."'";

		$result = db_query($query);
		$tutorial_detail = db_fetch_object($result);



		$video_path = 'st_videos/'.$tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/';
		require_once('path.config.inc');
		$path = $file_upload_path.$video_path;
		$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.'));
		$file_name = $tutorial_detail->tutorial_name.'-'.$tutorial_detail->language.$file_ext;

	
		if($_FILES['files']['size']['video'] > 0){
			move_uploaded_file($_FILES['files']['tmp_name']['video'], $path.$file_name);

		}

		global $user;

		$tutorial_video = $video_path.$file_name;
		if($form_state['values']['video_status'] == 1){
			$tutorial_video = 'pending';
		}

		// insert into common data table

		$query = "UPDATE tutorial_resources SET tutorial_video='".$tutorial_video."' WHERE id='".$form_state['values']['tutorial_resource_id']."'";


		db_query($query);

		// to track uploader 

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['tutorial_resourse_id']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

		if(db_query($query)){
			drupal_set_message('Tutorial Video Updated Successfully');
		}else {
			drupal_set_message('Error while Adding Tutorial Video, Please try again.');
		}
		$redirect = 'stupload/list';

		drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);

}

?>
