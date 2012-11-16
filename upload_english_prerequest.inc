<?php

	function upload_english_prerequest_form($form_state) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$upload_option = array('Reqired' => 'Reqired', 'Not Reqired' => 'Not Reqired', 'Upload Later' => 'Upload Later');
		$tutorial_name = array('' => 'Tutorial Name');
		// $outline = array('' => 'Outline');
		// $video = array('' => 'Video');
		// $script = array('' => 'Script');
		// $code_file = array('' => 'Code File');
		// $slide = array('' => 'Slide');
		// $assignment = array('' => 'Assignment');
		// $outline = array_merge($outline, $upload_option);
		// $video = array_merge($video, $upload_option);
		// $script = array_merge($script, $upload_option);
		// $code_file = array_merge($code_file, $upload_option);
		// $slide = array_merge($slide, $upload_option);
		// $assignment = array_merge($assignment, $upload_option);
		// get foss category
		$query = "select * from foss_categories";
		$result = db_query($query);
		$foss_category = array('' => 'FOSS category');
		while($row = db_fetch_object($result)){
			$foss_category[$row->name] =  $row->name;
		}
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english');
	
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'upeng_foss_category_name'),
			'#prefix' => '<div class="uenglish-category-level-name">',
		);
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Leval'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array(''=> 'Level'),
			'#attributes' => array('class' => 'upeng_tutorial_level'),
			'#prefix' => '<div class="uenglish-level-name"><div class="uenglish-level">',
			'#suffix' => '</div>', 
		);
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'upeng_tutorial_name'),
			'#prefix' => '<div class="uenglish-name">',
			'#suffix' => '</div></div></div>', 
		);
		/* $form['outline'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $outline,
			'#attributes' => array('class' => 'upeng_outline'),
			'#prefix' => '<div class="uenglish-outline-video-script">',
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
			'#attributes' => array('class' => 'upeng_script'),
			'#suffix' => '</div>'
		);
		$form['code_file'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $code_file,
			'#attributes' => array('class' => 'upeng_code_file'),
			'#prefix' => '<div class="uenglish-code-slide-assignment">',
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
			'#attributes' => array('class' => 'upeng_assignment'),
			'#suffix' => '</div>'
		); */
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_prerequest_form_validate($form, &$form_state){
		if($form_state['values']['tutorial_name'] != ''){
			$query = "select id from tutorial_details where foss_category_id='".$form_state['values']['foss_category']."' and tutorial_name='".$form_state['values']['tutorial_name']."' and tutorial_level='".$form_state['values']['tutorial_level']."'";

		$result = db_query($query);
		$row = db_fetch_object($result);
			$query = "select tutorial_detail_id from tutorial_resources where tutorial_detail_id='".$row->id."' and language='English'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			if($row){
				form_set_error('tutorial_name', t('This Tutorial Already Exits.'));
			}
		}
		return;
	}
	
	function upload_english_prerequest_form_submit($form, &$form_state){

		$query = "select id from tutorial_details where foss_category_id='".$form_state['values']['foss_category']."' and tutorial_name='".$form_state['values']['tutorial_name']."' and tutorial_level='".$form_state['values']['tutorial_level']."'";

		$result = db_query($query);
		$row = db_fetch_object($result);

		$tutorial_detail_id = $row->id;

		$language = 'English';
		$query = "INSERT INTO `tutorial_resources` (`id`, `tutorial_detail_id`, `language`, `tutorial_outline`, `tutorial_script`, `tutorial_video`) VALUES (NULL, '".$tutorial_detail_id."', '".$language."', 'pending', 'pending', 'pending')";
		if(db_query($query)){
			drupal_set_message('New FOSS Category Added Successfully');
			$query = "select id from tutorial_resources where tutorial_detail_id='".$tutorial_detail_id."' and language='".$language."'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			$redirect = 'stupload/upload_english/outline/?'.$row->id.'='.$tutorial_detail_id;
			var_dump($redirect);
			//die;
			drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while Adding Foss Category, Please try again.');
		}
		

}


?>