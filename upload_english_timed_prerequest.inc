<?php

	function upload_english_timed_prerequest_form($form_state) {
		$path = drupal_get_path('module', 'stupload');
		drupal_add_js($path . '/stupload.js');
		drupal_add_css($path . '/stupload.css');

		$upload_option = array('Reqired' => 'Reqired', 'Not Reqired' => 'Not Reqired', 'Upload Later' => 'Upload Later');
		$tutorial_name = array('' => 'Tutorial Name');
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
			'#attributes' => array('class' => 'upeng_timed_foss_category_name'),
			'#prefix' => '<div class="uenglish-category-level-name">',
		);
		$form['tutorial_level'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Level'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array(''=> 'Level'),
			'#attributes' => array('class' => 'upeng_timed_tutorial_level'),
			'#prefix' => '<div class="uenglish-timed-level-name"><div class="uenglish-level">',
			'#suffix' => '</div>', 
		);
		$form['tutorial_name'] = array(
			'#type' => 'select',
			'#title' => t('Tutorial Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => $tutorial_name,
			'#attributes' => array('class' => 'upeng_timed_tutorial_name'),
			'#prefix' => '<div class="uenglish-name">',
			'#suffix' => '</div></div></div>', 
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
	   );
	
	return $form;
	
	}
	
	
	function upload_english_timed_prerequest_form_validate($form, &$form_state){
		if($form_state['values']['tutorial_name'] != ''){
			$query = "select id from tutorial_details where foss_category='".$form_state['values']['foss_category']."' and tutorial_name='".$form_state['values']['tutorial_name']."' and tutorial_level='".$form_state['values']['tutorial_level']."'";

			$result = db_query($query);
			$row = db_fetch_object($result);
			$query = "select tutorial_detail_id from tutorial_resources where tutorial_detail_id='".$row->id."' and language='English-timed'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			if($row){
				form_set_error('tutorial_name', t('This tutorial is already exits.'));
			}
		}
		return;
	}
	
	function upload_english_timed_prerequest_form_submit($form, &$form_state){
		
		global $user;	
		
		$query = "select td.id as tdid, tr.id as trid, tr.tutorial_outline, tr.tutorial_script, tr.tutorial_video, tr.cvideo_version, tc.id as tcid from tutorial_details td, tutorial_resources tr, tutorial_common_contents tc where td.foss_category='".$form_state['values']['foss_category']."' and td.tutorial_level='".$form_state['values']['tutorial_level']."' and td.tutorial_name='".$form_state['values']['tutorial_name']."'";
		$query .= " and tr.tutorial_detail_id=td.id and tr.language='English' and tc.tutorial_detail_id=td.id";
		
		if($res = db_query($query)){
			$tut_detail = db_fetch_object($res);
			
			$query = "insert into tutorial_resources values('', ".$tut_detail->tdid.", ".$user->uid.", 'English-timed', '', '', ".$tut_detail->tcid.", '".$tut_detail->tutorial_outline."', 'pending', '".$tut_detail->tutorial_video."', 'pending', ".$tut_detail->cvideo_version.")";
			if(db_query($query)){
				$query = "select id from tutorial_resources where tutorial_detail_id='".$tut_detail->tdid."' and language='English-timed'";
				$result = db_query($query);
				$row = db_fetch_object($result);
				$redirect = 'stupload/english_timed_script/?tr='.$row->id;
				drupal_goto($path = $redirect, $query = NULL, $fragment = NULL, $http_response_code = 302);
			}else{
				drupal_set_message('Something went wrong, Please try again.');
			}
		}else {
			drupal_set_message('Something went wrong, Please try again.');
		}
	}
?>