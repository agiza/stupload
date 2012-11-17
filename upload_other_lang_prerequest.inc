<?php

	function upload_other_lang_prerequest_form($form_state) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$upload_option = array('Reqired' => 'Reqired', 'Not Reqired' => 'Not Reqired', 'Upload Later' => 'Upload Later');
		$tutorial_name = array('' => 'Tutorial Name');

		// select FOSS Category
		$query = "select * from foss_categories";
		$result = db_query($query);
		$foss_category = array('' => 'Select FOSS category');
		while($row = db_fetch_object($result)){
			$foss_category[$row->name] =  $row->name;
		}
		// select Languages
		$query = "select * from tutorial_languages";
		$result = db_query($query);
		$languages = array('' => 'Select language');
		while($row = db_fetch_object($result)){
			$languages[$row->name] =  $row->name;
		}

		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english');
	
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#options' => $foss_category,
			'#attributes' => array('class' => 'uolang_foss_category_name'),
			'#prefix' => '<div class="uolang-category-level-name">',
		);
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial level'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array(''=> 'Select Tutorial level'),
			'#attributes' => array('class' => 'uolang_tutorial_level uolang_tutorial_level'),
			'#prefix' => '<div class="uolang-level-name"><div class="uolang-level">',
			'#suffix' => '</div>', 
		);
		$form['tutorial_lang'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Languages'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $languages,
			'#attributes' => array('class' => 'uolang_tutorial_lang'),
			'#prefix' => '<div class="uolang-lang">',
			'#suffix' => '</div>', 
		);
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'uolang_tutorial_name'),
			'#prefix' => '<div class="uolang-name">',
			'#suffix' => '</div></div></div>', 
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_other_lang_prerequest_form_validate($form, &$form_state){
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
	
	function upload_other_lang_prerequest_form_submit($form, &$form_state){

		$query = "select id from tutorial_details where foss_category='".$form_state['values']['foss_category']."' and tutorial_name='".$form_state['values']['tutorial_name']."' and tutorial_level='".$form_state['values']['tutorial_level']."'";

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