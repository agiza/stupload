<?php
	function upload_english_script($params) {
		$params_data = parse_url($params);

		$data = explode('&', $params_data['query']);

		$tutorial_resourse = explode('=', $data[0]);
		$tutorial_resourse_id = $tutorial_resourse[1];

		$tutorial_detail = explode('=', $data[1]);
		$tutorial_detail_id = $tutorial_detail[1];

		$query = "SELECT * FROM `tutorial_details` WHERE id='".$tutorial_detail_id."'";

		$result = db_query($query);

		$tutorial_detail = db_fetch_object($result);

		$query = "SELECT name FROM `foss_categories` WHERE id='".$tutorial_detail->foss_category_id."'";

		$result = db_query($query);

		$foss_category = db_fetch_object($result);

		$wiki_url = 'wiki/index.php/'.$foss_category->name.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/'.'English';
		// $f1 = 'http://www.spoken-tutorial.org/wiki/index.php/'.$wiki_url;
		// $file_headers = @get_headers($f1);
		// echo '<pre>';
		// var_dump($file_headers);
		// die;


		$output = "<iframe width='100%' height='100%' src='http://www.spoken-tutorial.org/".$wiki_url."'></iframe>";
		$output .= drupal_get_form('upload_english_script_form', $tutorial_resourse_id, $wiki_url, $tutorial_detail_id);

		return $output;


	}


	function upload_english_script_form($form_state, $tutorial_resourse_id, $wiki_url, $tutorial_detail_id) {
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['wiki_url'] = array(
			'#type' => 'hidden',
			'#value' => $wiki_url,
		);

		$form['tutorial_resourse_id'] = array(
			'#type' => 'hidden',
			'#value' => $tutorial_resourse_id,
		);
		$form['tutorial_detail_id'] = array(
			'#type' => 'hidden',
			'#value' => $tutorial_detail_id,
		);

		$form['script_status'] = array(
			'#type' => 'select',
			'#title' => t('Script'),
			'#required' => TRUE,
			'#options' => array('' => 'Script', 0 => 'Required' , 1 => 'Will upload later'),
			'#attributes' => array('class' => 'upeng_script'),
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_script_form_validate($form, &$form_state){
	
	  	return;
	}
	
	function upload_english_script_form_submit($form, &$form_state){
		
		global $user;
		$tutorial_outline = $form_state['values']['wiki_url'];
		if($form_state['values']['script_status'] == 1){
			$tutorial_outline = 'pending';
		}


		$query = "UPDATE tutorial_resources SET tutorial_script='".$form_state['values']['wiki_url']."' where id='".$form_state['values']['tutorial_resourse_id']."'";


		db_query($query);
		
		//$query = "SELECT id FROM tutorial_common_contents WHERE tutorial_detail_id='".$form_state['values']['tutorial_detail_id']."'";

		//$result = db_query($query);
		//$row = db_fetch_object($result);

		//$query = "UPDATE tutorial_resources SET tutorial_content_id='".$row->id."' WHERE id='".$form_state['values']['tutorial_resource_id']."'";

		//db_query($query);

		$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['tutorial_resourse_id']."', CURRENT_TIMESTAMP, '".$user->name."', 'script')";


		if(db_query($query)){
			drupal_set_message('Tutorial script Updated Successfully');
		}else {
			drupal_set_message('Error while Adding Tutorial Script, Please try again.');
		}
		$redirect = 'stupload/upload_english/slide/?tutorial_resources='.$form_state['values']['tutorial_resourse_id'].'&tutorial_detail_id='.$form_state['values']['tutorial_detail_id'];

		drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);

}

?>