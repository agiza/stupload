<?php
	function upload_english_outline() {

		if($_REQUEST['trid']){	
			$output = "<h2>Upload Outline for ".$_REQUEST['tn']."</h2>";	
			$output .= drupal_get_form('upload_english_outline_form', $_REQUEST['trid'], $_REQUEST['outline']);
			return $output;
		}else{
			$output = "<p>Somthing went wrong. Please, try again.</p>";
			return $output;
		}
		
	}
	function upload_english_outline_form($form_state, $trid, $outline) {
		
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
			'#value' => $trid
		);

		$form['outline_status'] = array(
			'#type' => 'select',
			'#title' => t('Outline Status'),
			'#required' => TRUE,
			'#options' => array(0 => 'Required', 1 => 'Will upload later'),
			'#attributes' => array('class' => 'uptn_outline_status', '')
		);
		$form['outline'] = array(
			'#type' => 'textarea',
			'#title' => t('Outline'),
			'#attributes' => array('class' => 'uptn_outline'),
			'#default_value' => $outline, 
			'#prefix' => '<div class="stupload-outline">',
			'#suffix' => '</div>'
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_outline_form_validate($form, &$form_state){
	
		if ($form_state['values']['outline_status'] == 0){
			if((strlen($form_state['values']['outline']) > 10)){
				
			}else{
				form_set_error('outline', t('Please, enter atleast 10 Characters'));
			}
		}
		
	  	return;
	}
	
	function upload_english_outline_form_submit($form, &$form_state){
		global $user;
		$tutorial_outline = $form_state['values']['outline'];
		$query = "UPDATE tutorial_resources SET tutorial_outline_status = 2, tutorial_outline_uid = $user->uid ,tutorial_outline='".db_escape_string($tutorial_outline)."' WHERE id='".$form_state['values']['trid']."'";
		if($form_state['values']['outline_status'] == 1){
			$query = "UPDATE tutorial_resources SET tutorial_outline_status = 0, tutorial_outline='pending', tutorial_outline_uid = $user->uid WHERE id='".$form_state['values']['trid']."'";
		}
		db_query($query);
		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'outline')";

		if(db_query($query)){
			//Mail to contributor
			$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
			$result = db_query($query);
			$tutorial_detail = db_fetch_object($result);

			$a = new stdClass();
			$a->tc = 'Outline';					
			$tds =  (object) array_merge((array) $a, (array) $tutorial_detail);

		    $mailto = $user->mail;
		    $role = 'contributor';
		    $message = "";
			stupload_notify($mailto, $role, $message, $tds, $user->name);

			//Mail to domain and qc 
			$role = 'quality_checker';
			$quality_mailid = get_language_role_emails($tutorial_detail->language);

			$role = 'domain_user';
			$admin_mailid = get_role_emails($role);

			$mailto = $admin_mailid.$quality_mailid;
		    $message = "";
			stupload_notify($mailto, $role, $message, $tds, $user->name);

			drupal_set_message('Tutorial Outline Updated Successfully');
			$query = array('trid' => $form_state['values']['trid']);
			drupal_goto('stupload/upload_english_tutorial_index', $query, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while Adding Tutorial Outline, Please try again.');
		}

}


?>
