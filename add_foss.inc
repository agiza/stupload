<?php

	function add_foss_form($form_state) {
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['name'] = array(
			'#type' => 'textfield',
			'#title' => t('FOSS Category Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category_name')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function add_foss_form_validate($form, &$form_state){
	
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
	
	function add_foss_form_submit($form, &$form_state){
		$query = "INSERT INTO `foss_categories` (`id`, `name`) VALUES (NULL, '".$form_state['values']['name']."')";
		if(db_query($query)){
			drupal_set_message('New FOSS Category Added Successfully');
		}else {
			drupal_set_message('Error while Adding Foss Category, Please try again.');
		}
		drupal_goto($path = 'stupload/addfoss', $query = NULL, $fragment = NULL, $http_response_code = 302);

}


?>
