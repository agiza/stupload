$(document).ready(function(){
	webroot = "http://10.102.6.113/spoken_tutorial_org/stupload/";
	loading_image = "<img src='http://10.102.6.113/spoken_tutorial_org/ajax-loader.gif' />";
	// for add availabel tutorial levels for add tutorial names
	$('.uptn_tutorial_level').change(function(){
		foss = $('.uptn_foss_category_name').val();
		level = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "get_tutorial_levels",
			data : {
				'foss' : foss, 'level' : level
			},
			beforeSend: function() {
			    field_data = $('.poll-form').html();
			    $('.poll-form').html(loading_image);
			},
			success : function(data){
				output = JSON.parse(data);
				var html_data = "<option val=''>Order</option>";
				if(output){
					$('.poll-form').html(field_data);
					for (var i=0; i < output.length; i++)
					{
						html_data += "<option value='"+ output[i] +"'>" + output[i] + "</option>";	
					}
					console.log(html_data);
					$('.uptn_tutorial_order_no').html(html_data);
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});
	$('.upeng_foss_category_name').change(function(){
		foss = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "get_category_levels",
			data : {
				'foss' : foss
			},
			beforeSend: function() {
			    field_data = $('.uenglish-level-name').html();
			    $('.uenglish-level-name').html(loading_image);
			},
			success : function(data){
				output = JSON.parse(data);
				var html_data = "<option value=''>Level</option>";
				if(output){
					$('.uenglish-level-name').html(field_data);
					for (var i=0; i < output.length; i++)
					{
						html_data += "<option value='"+ output[i].tutorial_level +"'>" + output[i].tutorial_level + "</option>";	
					}
					console.log(html_data);
					$('.upeng_tutorial_level').html(html_data);
					// adding the tutorial name under tutorial level
					$('.upeng_tutorial_level').change(function(){
						level = $(this).val();
						$.ajax({
							type : 'POST',
							url : webroot + "get_category_names",
							data : {
								'level' : level
							},
							beforeSend: function() {
							    field_data = $('.uenglish-name').html();
							    $('.uenglish-name').html(loading_image);
							},
							success : function(data){
								output = JSON.parse(data);
								var html_data = "<option value=''>Tutorial name</option>";
								if(output){
									$('.uenglish-name').html(field_data);
									for (var i=0; i < output.length; i++)
									{
										html_data += "<option value='"+ output[i].tutorial_name +"'>" + output[i].tutorial_name + "</option>";	
									}
									console.log(html_data);
									$('.upeng_tutorial_name').html(html_data);
								}else{
									alert('Somthing wrong, Please refresh page');
								}
							}
						});
					});
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});

	// field hide and show
	$('div.stupload-outline').css({'display' : 'none'});
	$('.uptn_outline_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-outline').css({'display' : 'block'});
		}else if($(this).val() == '' || $(this).val() == 1){
			$('div.stupload-outline').css({'display' : 'none'});
		}
	});

	// for script file
	$('.upeng_script_status').change(function(){
		path = $('.upeng_script_wiki').val();
		wiki_data = "<iframe width='100%' height='100%' src='http://www.spoken-tutorial.org/"+path+"'></iframe>";
		if($(this).val() == 0){
			$('div.wiki-script-file').html(wiki_data);
		}else if($(this).val() == '' || $(this).val() == 1){
			$('div.wiki-script-file').html('<p></p>');
		}
	});
	// for slide file
	$('div.stupload-form-slide').css({'display' : 'none'});
	$('.upeng_slide_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-slide').css({'display' : 'block'});
		}else if($(this).val() == '' || $(this).val() == 1){
			$('div.stupload-form-slide').css({'display' : 'none'});
		}
	});

	// for video file
	$('div.stupload-form-video').css({'display' : 'none'});
	$('.upeng_video_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-video').css({'display' : 'block'});
		}else if($(this).val() == '' || $(this).val() == 1){
			$('div.stupload-form-video').css({'display' : 'none'});
		}
	});

	// for codefile file
	$('div.stupload-form-codefile').css({'display' : 'none'});
	$('.upeng_codefile_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-codefile').css({'display' : 'block'});
		}else if($(this).val() == '' || $(this).val() == 1 || $(this).val() == 2){
			$('div.stupload-form-codefile').css({'display' : 'none'});
		}
	});
	// for assignment file
	$('div.stupload-form-asgmnt').css({'display' : 'none'});
	$('.upeng_asgmnt_status').change(function(){
		if($(this).val() == 0){
			$('div.stupload-form-asgmnt').css({'display' : 'block'});
		}else if($(this).val() == '' || $(this).val() == 1 || $(this).val() == 2){
			$('div.stupload-form-asgmnt').css({'display' : 'none'});
		}
	});
});