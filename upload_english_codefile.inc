<?php
	function upload_english_codefile($params) {
		$params_data = parse_url($params);


		$data = explode('&', $params_data['query']);
		$tutorial_content = explode('=', $data[0]);
		$tutorial_content_id = $tutorial_content[1];

		$tutorial_resourse = explode('=', $data[1]);
		$tutorial_resourse_id = $tutorial_resourse[1];

		$tutorial_detail = explode('=', $data[2]);
		$tutorial_detail_id = $tutorial_detail[1];


		$output .= drupal_get_form('upload_english_codefile_form', $tutorial_content_id, $tutorial_resourse_id, $tutorial_detail_id);
		return $output;


	}


	function upload_english_codefile_form($form_state, $tutorial_content_id, $tutorial_resourse_id, $tutorial_detail_id) {
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

		$form['codefile_status'] = array(
			'#type' => 'select',
			'#title' => t('Codefile Status'),
			'#required' => TRUE,
			'#options' => array('' => 'Codefile', 0 => 'Required' , 1 => 'Will upload later', 2 => 'Not Required'),
			'#attributes' => array('class' => 'upeng_script'),
		);
		$form['codefile'] = array(
			'#type' => 'file',
			'#title' => t('Codefile'),
			'#attributes' => array('class' => 'upeng_slide'),
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_codefile_form_validate($form, &$form_state){
		if ($form_state['values']['codefile_status'] == 0){
			if($_FILES['files']['tmp_name']['codefile'] == ''){
				form_set_error('codefile', t('Please select the codefile file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.')); // get file name
				$filesize = $_FILES['files']['size']['codefile'];
				$allowed_file_types = array('.zip');
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('codefile', t('Please select proper file. .Zip file only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_english_codefile_form_submit($form, &$form_state){

		echo "<pre>";

		// select Tutorial categoty
		$query = "SELECT * FROM tutorial_details WHERE id='".$form_state['values']['tutorial_detail_id']."'";
		$result = db_query($query);

		$tutorial_details = db_fetch_object($result);

		$query = "SELECT name FROM foss_categories WHERE id='".$tutorial_details->foss_category_id."'";

		$result = db_query($query);

		$foss_category = db_fetch_object($result);
		$codefile_path = 'st_videos/'.$foss_category->name.'/'.$tutorial_details->tutorial_level.'/'.$tutorial_details->tutorial_name.'/Resource/';
		$path = '/var/www/spoken_tutorial_org/sites/default/files/'.$codefile_path;
		$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.'));
		$file_name = $tutorial_details->tutorial_name.'-Codefile-English'.$file_ext;
		if($_FILES['files']['size']['codefile'] > 0){

			move_uploaded_file($_FILES['files']['tmp_name']['codefile'], $path.$file_name);

		}

		global $user;

		$tutorial_code = $codefile_path.$file_name;
		if($form_state['values']['codefile_status'] == 1){
			$tutorial_code = 'pending';
		}
		if($form_state['values']['codefile_status'] == 2){
			$tutorial_code = 'notrequired';
		}

		// insert into common data table
		$query = "UPDATE tutorial_common_contents SET tutorial_code='".$tutorial_code."' where id='".$form_state['values']['tutorial_content_id']."'";

		db_query($query);

		// to track uploader 
		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['tutorial_resourse_id']."', CURRENT_TIMESTAMP, '".$user->name."', 'slide')";

		db_query($query);

		if(db_query($query)){
			drupal_set_message('Tutorial Slide Updated Successfully');
		}else {
			drupal_set_message('Error while Adding Tutorial Slide, Please try again.');
		}
		$redirect = 'stupload/upload_english/assignment/?tutorial_content_id='.$form_state['values']['tutorial_content_id'].'&tutorial_resources='.$form_state['values']['tutorial_resourse_id'].'&tutorial_detail_id='.$form_state['values']['tutorial_detail_id'];

		drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);

}

?>
