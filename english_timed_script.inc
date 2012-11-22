<?php
	function english_timed_script($params) {
		$trid = explode('=', $params);
		if($trid[1]){		
			$output = "<h2>Upload English-timed Script</h2>";

			$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$trid[1]."'";

			$result = db_query($query);

			$tutorial_detail = db_fetch_object($result);

			require_once('path.config.inc');

			$wiki_url = $tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/'.'English-timed';
			$output .= "<div class='wiki-script-file'><iframe width='100%' height='100%' src='".$wiki_path.$wiki_url."'></iframe></div>";
			$output .= drupal_get_form('english_timed_script_form', $trid[1], $wiki_url);

			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}


	}


	function english_timed_script_form($form_state, $trid, $wiki_url) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['wiki_url'] = array(
			'#type' => 'hidden',
			'#value' => $wiki_url,
			'#attributes' => array('class' => 'upengtimed_script_wiki'),
		);

		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function english_timed_script_form_validate($form, &$form_state){
	
	  	return;
	}
	
	function english_timed_script_form_submit($form, &$form_state){
		
		global $user;
		$tutorial_script = $form_state['values']['wiki_url'];
		if($form_state['values']['script_status'] == 1){
			$tutorial_script = 'pending';
		}
		$query = "UPDATE tutorial_resources SET tutorial_script='".$tutorial_script."' where id='".$form_state['values']['trid']."'";

		db_query($query);		

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'script')";

		if(db_query($query)){
			drupal_set_message('Tutorial script Updated Successfully');
			$redirect = 'stupload/list';
			drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error while Adding Tutorial Script, Please try again.');
		}
}

?>