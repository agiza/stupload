<?php

function update_tutorial_search_form($form_state){
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
	$lang['English-timed'] = "English-timed";
	while($row = db_fetch_object($result)){
		$lang[$row->name] = $row->name;
	}
	
	$form['language'] = array(
		'#type' => 'select',
		'#title' => t(''),
		'#options' => $lang,
		'#attributes' => array('class' => 'language'),
		'#prefix' => '<div class="search_cls">',
		'#suffix' => '</div><div class="search_cls"><div class="form-item" id="edit-update_search-wrapper"><input type="button" name="update_search" id="update_search" class="update_search" value="Search" /></div></div></div><div style="text-align : center;" id="list_data" class="list_data"></div>'
	);
	
	return $form;
}
