<?php

function list_tutorials(){

	$file_ext = substr($_REQUEST['q'], strripos($_REQUEST['q'], '/'));
	$name = str_replace('/', '', $file_ext);
	var_dump($name);
	if($name == 'review_tutorial_lists'){
		$flag = '0';
	}else{
		$flag = '1';
	}
	$output = drupal_get_form('list_tutorials_form', $flag);
	return $output;

}

function list_tutorials_form($form_state, $flag){
	global $user;
	
	$path = drupal_get_path('module', 'stupload');
	drupal_add_css($path.'/stupload-view.css');
	drupal_add_js($path."/stupload-view.js", 'module');
	//fetching list of foss category details
	$query = "select distinct foss_category from tutorial_details";
	$result = db_query($query);
	$foss = array();
	$foss[''] = "Select FOSS Category";
	while($row = db_fetch_object($result)){
		$foss[$row->foss_category] = $row->foss_category;
	}
	
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['flag'] = array(
		'#type' => 'hidden',
		'#value' => $flag,
		'#attributes' => array('class' => 'flag')
	);
	$form['foss_category'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $foss,
		'#attributes' => array('class' => 'foss_category'),
		'#prefix' => '<br /><center><b>Select FOSS Category, Select Language and Search for a list of Tutorials</b></center><div class="search_cls_main"><div class="search_cls">',
		'#suffix' => '</div>'
	);
	
	$query = "select name from tutorial_languages";
	$result = db_query($query);
	$lang = array();
	$lang[''] = "Select Language";
	$lang['English'] = "English";
	$lang['English-Timed'] = "English-Timed";
	while($row = db_fetch_object($result)){
		$lang[$row->name] = $row->name;
	}
	
	$form['language'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $lang,
		'#attributes' => array('class' => 'language'),
		'#prefix' => '<div class="search_cls">',
		'#suffix' => '</div><div class="search_cls"><div class="form-item" id="edit-search-wrapper"><input type="button" name="search" id="search" class="search" value="Search" /></div></div></div><div style="text-align : center;" id="list_data" class="list_data"></div>'
	);
	
	return $form;
}
