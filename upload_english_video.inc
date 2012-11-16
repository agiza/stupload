<?php
	function upload_english_video($params) {
		$params_data = parse_url($params);


		$data = explode('&', $params_data['query']);
		$tutorial_content = explode('=', $data[0]);
		$tutorial_content_id = $tutorial_content[1];

		$tutorial_resourse = explode('=', $data[1]);
		$tutorial_resourse_id = $tutorial_resourse[1];

		$tutorial_detail = explode('=', $data[2]);
		$tutorial_detail_id = $tutorial_detail[1];


		$output .= drupal_get_form('upload_english_video_form', $tutorial_content_id, $tutorial_resourse_id, $tutorial_detail_id);
		return $output;


	}


	function upload_english_video_form($form_state, $tutorial_content_id, $tutorial_resourse_id, $tutorial_detail_id) {
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['tutorial_content_id'] = array(
			'#type' => 'hidden',
			'#value' => $tutorial_content_id,
		);

		$form['tutorial_resourse_id'] = array(
			'#type' => 'hidden',
			'#value' => $tutorial_resourse_id,
		);
		$form['tutorial_detail_id'] = array(
			'#type' => 'hidden',
			'#value' => $tutorial_detail_id,
		);

		$form['video_status'] = array(
			'#type' => 'select',
			'#title' => t('Slide Status'),
			'#required' => TRUE,
			'#options' => array('' => 'Video', 0 => 'Required' , 1 => 'Will upload later'),
			'#attributes' => array('class' => 'upeng_script'),
		);
		$form['video'] = array(
			'#type' => 'file',
			'#title' => t('Video'),
			'#attributes' => array('class' => 'upeng_slide'),
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
				$allowed_file_types = array('.mp4', '.ogv');
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('slide', t('Please select proper file. ogv and mp4 files only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_english_video_form_submit($form, &$form_state){



		// select Tutorial categoty
		$query = "SELECT * FROM tutorial_details WHERE id='".$form_state['values']['tutorial_detail_id']."'";
		$result = db_query($query);

		$tutorial_details = db_fetch_object($result);

		$query = "SELECT name FROM foss_categories WHERE id='".$tutorial_details->foss_category_id."'";

		$result = db_query($query);

		$foss_category = db_fetch_object($result);
		$video_path = 'st_videos/'.$foss_category->name.'/'.$tutorial_details->tutorial_level.'/'.$tutorial_details->tutorial_name.'/';
		$path = '/var/www/spoken_tutorial_org/sites/default/files/'.$video_path;
		$file_ext = substr($_FILES['files']['name']['video'], strripos($_FILES['files']['name']['video'], '.'));
		$file_name = $tutorial_details->tutorial_name.'-English'.$file_ext;

		if($_FILES['files']['size']['video'] > 0){
			move_uploaded_file($_FILES['files']['tmp_name']['video'], $path.$file_name);

		}

		global $user;

		$tutorial_slide = $video_path.$file_name;
		if($form_state['values']['video_status'] == 1){
			$tutorial_slide = 'pending';
		}

		// insert into common data table

		$query = "UPDATE tutorial_resources SET tutorial_video='".$tutorial_slide."' WHERE id='".$form_state['values']['tutorial_resourse_id']."'";


		db_query($query);

		// to track uploader 

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['tutorial_resourse_id']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

		db_query($query);


		if(db_query($query)){
			drupal_set_message('Tutorial Video Updated Successfully');
		}else {
			drupal_set_message('Error while Adding Tutorial Video, Please try again.');
		}
		$redirect = 'stupload/upload_english/codefile/?tutorial_content_id='.$form_state['values']['tutorial_content_id'].'&tutorial_resources='.$form_state['values']['tutorial_resourse_id'].'&tutorial_detail_id='.$form_state['values']['tutorial_detail_id'];

		drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);

}

?>
