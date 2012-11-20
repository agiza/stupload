<?php
	function upload_english_codefile() {
		if($_REQUEST['trid'] && $_REQUEST['tccid']){
			$output .= drupal_get_form('upload_english_codefile_form', $_REQUEST['trid'], $_REQUEST['tccid']);
			return $output;
		}else{
			$output = "<p>Something went wrong. Please, try again.</p>";
			return $output;
		}
	}

	function upload_english_codefile_form($form_state, $trid, $tccid) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');
		$form['#prefix'] = '<div class="stupload-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'addfoss');

		$form['tccid'] = array(
			'#type' => 'hidden',
			'#value' => $tccid,
		);

		$form['trid'] = array(
			'#type' => 'hidden',
			'#value' => $trid,
		);

		$form['codefile_status'] = array(
			'#type' => 'select',
			'#title' => t('Codefile Status'),
			'#required' => TRUE,
			'#options' => array('' => 'Codefile', 0 => 'Required' , 1 => 'Will upload later', 2 => 'Not Required'),
			'#attributes' => array('class' => 'upeng_codefile_status'),
		);
		$form['codefile'] = array(
			'#type' => 'file',
			'#title' => t('Codefile'),
			'#attributes' => array('class' => 'upeng_codefile'),
			'#prefix' => '<div class="stupload-form-codefile">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_codefile_form_validate($form, &$form_state){
		if ($form_state['values']['codefile_status'] == 0){
			if($_FILES['files']['tmp_name']['codefile'] == ''){
				form_set_error('codefile', t('Please select the codefile file.'));
			}else{
				$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.')); // get file name
				$filesize = $_FILES['files']['size']['codefile'];
				$allowed_file_types = array('.zip');
				if (!in_array($file_ext,$allowed_file_types)  &&  ($filesize < 80000000)) {
					form_set_error('codefile', t('Please select proper file. .Zip file only allowed.'));
				}
			}
		}
		
	  	return;
	}
	
	function upload_english_codefile_form_submit($form, &$form_state){
		// select Tutorial categoty
		$query = "select tr.language, td.foss_category, td.tutorial_name, td.tutorial_level from tutorial_resources tr INNER JOIN tutorial_details td ON td.id=tr.tutorial_detail_id where tr.id='".$form_state['values']['trid']."'";

		$result = db_query($query);
		$tutorial_detail = db_fetch_object($result);

		$codefile_path = 'st_videos/'.$tutorial_detail->foss_category.'/'.$tutorial_detail->tutorial_level.'/'.$tutorial_detail->tutorial_name.'/Resource/';
		require_once('path.config.inc');
		$path = $file_upload_path.$codefile_path;
		$file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.'));
		$file_name = $tutorial_detail->tutorial_name.'-Codefile-'.$tutorial_detail->language.$file_ext;
	
		if($_FILES['files']['size']['codefile'] > 0){
			if(move_uploaded_file($_FILES['files']['tmp_name']['codefile'], $path.$file_name)){
				global $user;
				$tutorial_codefile = $codefile_path.$file_name;
				// insert into common data table
				$query = "UPDATE tutorial_common_contents SET tutorial_code='".$tutorial_codefile."' WHERE id='".$form_state['values']['tccid']."'";
				db_query($query);

				// to track uploader 
				$query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['trid']."', CURRENT_TIMESTAMP, '".$user->name."', 'video')";

				if(db_query($query)){
					drupal_set_message('Tutorial Codefile Added Successfully');
					$redirect = 'stupload/english_index/?tr='.$form_state['values']['trid'];
					drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
				}else {
					drupal_set_message('Error while Adding Codefile, Please try again.');
				}
			}else{
				drupal_set_message('Error while Adding Codefile, Please try again..');
			}
		}

		// // select Tutorial categoty
		// $query = "SELECT * FROM tutorial_details WHERE id='".$form_state['values']['tutorial_detail_id']."'";
		// $result = db_query($query);

		// $tutorial_details = db_fetch_object($result);

		// $codefile_path = 'st_videos/'.$tutorial_details->foss_category.'/'.$tutorial_details->tutorial_level.'/'.$tutorial_details->tutorial_name.'/Resource/';
		// require_once('path.config.inc');
		// $path = $file_upload_path.$codefile_path;
		// $file_ext = substr($_FILES['files']['name']['codefile'], strripos($_FILES['files']['name']['codefile'], '.'));
		// $file_name = $tutorial_details->tutorial_name.'-Codefile-English'.$file_ext;
		// if($_FILES['files']['size']['codefile'] > 0){

		// 	move_uploaded_file($_FILES['files']['tmp_name']['codefile'], $path.$file_name);

		// }

		// global $user;

		// $tutorial_code = $codefile_path.$file_name;
		// if($form_state['values']['codefile_status'] == 1){
		// 	$tutorial_code = 'pending';
		// }
		// if($form_state['values']['codefile_status'] == 2){
		// 	$tutorial_code = 'notrequired';
		// }

		// // insert into common data table
		// $query = "UPDATE tutorial_common_contents SET tutorial_code='".$tutorial_code."' where id='".$form_state['values']['tutorial_content_id']."'";

		// db_query($query);

		// // to track uploader 
		// $query = "INSERT INTO `tutorial_update_log` (`id`, `tutorial_resources_id`, `update_time`, `updated_by`, `updated_content`) VALUES (NULL, '".$form_state['values']['tutorial_resourse_id']."', CURRENT_TIMESTAMP, '".$user->name."', 'codefile')";

		// if(db_query($query)){
		// 	drupal_set_message('Tutorial Slide Updated Successfully');
		// }else {
		// 	drupal_set_message('Error while Adding Tutorial Slide, Please try again.');
		// }
		// $redirect = 'stupload/upload_english/assignment/?tutorial_content_id='.$form_state['values']['tutorial_content_id'].'&tutorial_resources='.$form_state['values']['tutorial_resourse_id'].'&tutorial_detail_id='.$form_state['values']['tutorial_detail_id'];

		// drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);

}

?>
