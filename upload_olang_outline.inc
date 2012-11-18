<?php
	function upload_olang_outline($params) {
		if($params){
			$params = parse_url($params);
			$tr_data = explode('=', $params['query']);
			$output = drupal_get_form('upload_olang_outline_form',  $tr_data[1]);
			return $output;
		}else{
			$output = "<p>Somthing went wrong. Please, try again.</p>";
			return $output;
		}
		
	}
	function upload_olang_outline_form($form_state, $trid) {
		
		// add js file 
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addtutorial-name');

		$form['trid'] = array(
			'#type' => 'hidden',
			'#title' => t('id'),
			'#required' => TRUE,
			'#value' => $trid,
			//'#attributes' => array('class' => 'uptn_outline', '')
		);
		$form['outline_status'] = array(
			'#type' => 'select',
			'#title' => t('Outline Status'),
			'#required' => TRUE,
			'#options' => array('' => 'Status', 0 => 'Required', 1 => 'Will upload later'),
			'#attributes' => array('class' => 'uptn_outline_status', '')
		);
		$form['outline'] = array(
			'#type' => 'textarea',
			'#title' => t('Outline'),
			'#attributes' => array('class' => 'uptn_outline'),
			'#prefix' => '<div class="stupload-outline">',
			'#suffix' => '</div>'
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_olang_outline_form_validate($form, &$form_state){
	
		if ($form_state['values']['outline_status'] == 0){
			if((strlen($form_state['values']['outline']) > 10)){
				
			}else{
				form_set_error('outline', t('Please, enter atleast 10 Characters'));
			}
		}
		
	  	return;
	}
	
	function upload_olang_outline_form_submit($form, &$form_state){
		global $user;
		$tutorial_outline = $form_state['values']['outline'];
		if($form_state['values']['outline_status'] == 1){
			$tutorial_outline = 'pending';
		}


		$query = "UPDATE tutorial_resources SET tutorial_outline='".$form_state['values']['outline']."' WHERE id='".$form_state['values']['trid']."'";

		db_query($query);

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'outline')";

		if(db_query($query)){
			drupal_set_message('Tutorial Outline Updated Successfully');
		}else {
			drupal_set_message('Error while Adding Tutorial Outline, Please try again.');
		}
		$redirect = 'stupload/upload_other_lang/script/?tr='.$form_state['values']['trid'];
		drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);

}


?>
