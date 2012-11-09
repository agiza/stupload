<?php

	function upload_english_form($form_state) {
		
		$upload_option = array('Reqired' => 'Reqired', 'Not Reqired' => 'Not Reqired', 'Upload Later' => 'Upload Later');
		$outline = array('' => 'Outline');
		$video = array('' => 'Video');
		$script = array('' => 'Script');
		$code_file = array('' => 'Code File');
		$slide = array('' => 'Slide');
		$assignment = array('' => 'Assignment');
		$outline = array_merge($outline, $upload_option);
		$video = array_merge($video, $upload_option);
		$script = array_merge($script, $upload_option);
		$code_file = array_merge($code_file, $upload_option);
		$slide = array_merge($slide, $upload_option);
		$assignment = array_merge($assignment, $upload_option);
		// get foss category
		$query = "select * from foss_categories";
		$result = db_query($query);
		$foss_category = array('' => 'FOSS Category');
		while($row = db_fetch_object($result)){
			$foss_category[$row->id] =  $row->name;
		}
		$form['#attributes'] = array('enctype' => "multipart/form-data");
	
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'upeng_foss_category_name')
		);
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Leval'),
			'#required' => TRUE,
			'#options' => '',
			'#attributes' => array('class' => 'upeng_tutorial_level')
		);
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#options' => '',
			'#attributes' => array('class' => 'upeng_tutorial_name')
		);
		$form['outline'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $outline,
			'#attributes' => array('class' => 'upeng_outline')
		);
		$form['video'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $video,
			'#attributes' => array('class' => 'upeng_video')
		);
		$form['script'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $script,
			'#attributes' => array('class' => 'upeng_script')
		);
		$form['code_file'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $code_file,
			'#attributes' => array('class' => 'upeng_code_file')
		);
		$form['slide'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $slide,
			'#attributes' => array('class' => 'upeng_slide')
		);
		$form['assignment'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $assignment,
			'#attributes' => array('class' => 'upeng_assignment')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_form_validate($form, &$form_state){
	
		$query = "select name from foss_categories where name='".$form_state['values']['name']."'";
		$result = db_query($query);
		$row = db_fetch_object($result);
		if (!preg_match('/^[a-zA-Z-]+$/', $form_state['values']['name'])){
			form_set_error('name', t('Invalid Name, Please use a-z, A-Z, - Only'));
		}else{
			if($row){
				form_set_error('name', t('Name already exists.'));
			}
		}

	  	return;
	}
	
	function upload_english_form_submit($form, &$form_state){
		$query = "INSERT INTO `foss_categories` (`id`, `name`) VALUES (NULL, '".$form_state['values']['name']."')";
		if(db_query($query)){
			drupal_set_message('New FOSS Category Added Successfully');
		}else {
			drupal_set_message('Error while Adding Foss Category, Please try again.');
		}
		drupal_goto($path = 'stupload/addfoss', $query = NULL, $fragment = NULL, $http_response_code = 302);

}


?>
