<?php
	function upload_english_slide($params) {
		if($params){
			$params_data = parse_url($params);

			$data = explode('&', $params_data['query']);

			$tutorial_resourse = explode('=', $data[0]);
			$tutorial_resourse_id = $tutorial_resourse[1];

			$tutorial_detail = explode('=', $data[1]);
			$tutorial_detail_id = $tutorial_detail[1];


			$output .= drupal_get_form('upload_english_slide_form', $tutorial_resourse_id, $tutorial_detail_id);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}


	}


	function upload_english_slide_form($form_state, $tutorial_resourse_id, $tutorial_detail_id) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['tutorial_resourse_id'] = array(
			'#type' => 'hidden',
			'#value' => $tutorial_resourse_id,
		);
		$form['tutorial_detail_id'] = array(
			'#type' => 'hidden',
			'#value' => $tutorial_detail_id,
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
			'#title' => t('Slide'),
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
		$query = "SELECT * FROM tutorial_details WHERE id='".$form_state['values']['tutorial_detail_id']."'";
		$result = db_query($query);

		$tutorial_details = db_fetch_object($result);

		$video_path = 'st_videos/'.$tutorial_details->foss_category.'/'.$tutorial_details->tutorial_level.'/'.$tutorial_details->tutorial_name.'/Resource/';
		require_once('path.config.inc');
		$path = $file_upload_path.$video_path;
		$file_ext = substr($_FILES['files']['name']['slide'], strripos($_FILES['files']['name']['slide'], '.'));
		$file_name = $tutorial_details->tutorial_name.'-Slides-English'.$file_ext;

		if($_FILES['files']['size']['slide'] > 0){
			move_uploaded_file($_FILES['files']['tmp_name']['slide'], $path.$file_name);

		}


		global $user;

		$tutorial_slide = $video_path.$file_name;
		if($form_state['values']['script_status'] == 1){
			$tutorial_slide = 'pending';
		}

		// insert into common data table
		$query = "INSERT INTO `tutorial_common_contents` (`id`, `tutorial_detail_id`, `tutorial_slide`, `tutorial_code`, `tutorial_assignment`) VALUES (NULL, '".$form_state['values']['tutorial_detail_id']."', '".$tutorial_slide."' , 'pending', 'pending')";


		db_query($query);
		
		$query = "SELECT id FROM tutorial_common_contents WHERE tutorial_detail_id='".$form_state['values']['tutorial_detail_id']."'";

		$result = db_query($query);
		$common_content = db_fetch_object($result);

		// update common content id to tutorial resource table 

		$query = "UPDATE tutorial_resources SET tutorial_content_id='".$common_content->id."' WHERE id='".$form_state['values']['tutorial_resourse_id']."'";

		db_query($query);
		// to track uploader 
		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['tutorial_resourse_id']."', CURRENT_TIMESTAMP, '".$user->name."', 'slide')";


		if(db_query($query)){
			drupal_set_message('Tutorial Slide Updated Successfully');
		}else {
			drupal_set_message('Error while Adding Tutorial Slide, Please try again.');
		}
		$redirect = 'stupload/upload_english/video/?tutorial_content_id='.$common_content->id.'&tutorial_resources='.$form_state['values']['tutorial_resourse_id'].'&tutorial_detail_id='.$form_state['values']['tutorial_detail_id'];

		drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);

}

?>
