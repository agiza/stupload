<?php

	function update_tutorial_prerequest_form($form_state) {
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
		$languages = array('' => 'Select language', 'English' => 'English');
		while($row = db_fetch_object($result)){
			$languages[$row->name] =  $row->name;
		}

		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'upload-english');
		
		$form['flag'] = array(
			'#type' => 'hidden',
			'#value' => 1,
			'#attributes' => array('class' => 'upload_flag'),
		);
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
	
	
	function update_tutorial_prerequest_form_validate($form, &$form_state){

		if($form_state['values']['foss_category'] == ''){
			form_set_error('tutorial_name', t('Please select FOSS Category.'));
		}
		if($form_state['values']['tutorial_level'] == ''){
			form_set_error('tutorial_level', t('Please select Tutorial Level.'));
		}
		if($form_state['values']['tutorial_lang'] == ''){
			form_set_error('tutorial_lang', t('Please select Language.'));
		}
		if($form_state['values']['tutorial_name'] == ''){
			form_set_error('tutorial_name', t('Please select Tutorial Name.'));
		}
		return;
	}
	
	function update_tutorial_prerequest_form_submit($form, &$form_state){
		
		$query = "select tr.id from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.language='".$form_state['values']['tutorial_lang']."' and td.foss_category='".$form_state['values']['foss_category']."' and td.tutorial_name='".$form_state['values']['tutorial_name']."' and tutorial_level='".$form_state['values']['tutorial_level']."'";

		//var_dump($query);
		$result = db_query($query);

		$row = db_fetch_object($result);

		if($row){
			$redirect = 'stupload/update_index/?tr='.$row->id;		

			drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while Tutorial, Please try again.');
		}	

}


?>