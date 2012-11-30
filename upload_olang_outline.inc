<?php
	function upload_olang_outline() {
		if($_POST['trid']){
			$output = "<h2>Upload English Outline for ".$_REQUEST['tn']."</h2>";
			$output .= drupal_get_form('upload_olang_outline_form',  $_POST['trid']);
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
			'#options' => array(0 => 'Required', 1 => 'Will upload later'),
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


		$query = "UPDATE tutorial_resources SET tutorial_status='pending', tutorial_outline='".$tutorial_outline."' WHERE id='".$form_state['values']['trid']."'";

		db_query($query);

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'outline')";

		if(db_query($query)){
			//Mail to contributor
			$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";
			$result = db_query($query);
			$td = db_fetch_array($result, MYSQL_ASSOC);
			$tds = array_merge($td, array('tc' => 'Outline'));
		    $mailto = $user->mail;
		    $role = 'contributor';
		    $message = "";
			stupload_notify($mailto, $role, $message, $tds);

			//Mail to admin_review_user 
			$role = 'admin_review_user';
			$mailto = get_role_emails($role);
		    $message = "";
			stupload_notify($mailto, $role, $message, $tds);

			drupal_set_message('Tutorial Outline Updated Successfully');
			$redirect = 'stupload/olang_index/?tr='.$form_state['values']['trid'];
			drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while Adding Tutorial Outline, Please try again.');
		}

}


?>
