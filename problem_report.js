$(document).ready(function(){
	webroot = "http://"+location.hostname+"/spoken_tutorial_org/forum/";
	loading_image = "<img src='http://"+location.hostname+"/spoken_tutorial_org/ajax-loader.gif' />";
	// get tutorial name for selected foss
	$('.forum_foss_category').change(function(){
		foss = $(this).val();
		$.ajax({
			type : 'POST',
			url : webroot + "forum_tutorial_name",
			data : {
				'foss' : foss
			},
			beforeSend: function() {
			    field_data = $('.forum-tutname').html();
			    $('.forum-tutname').html(loading_image);
			},
			success : function(data){
				output = JSON.parse(data);
				$('.forum-tutname').html(field_data);
				var html_data = '';
				for (var i=0; i < output.length; i++)
				{
					html_data += "<option value='"+ output[i]+"'>" + output[i] + "</option>";	
				}
				$('.forum_tutorial_name').html(html_data);
			}
		});
	});
	
	$('.prvideo-time').css({'display':'none'});
	$('.rptutorial-resource').change(function(){
		if($(this).val() == 'video' && $('.rptutorial-type').val() != 4){
			$('.prvideo-time').css({'display':'block'});
		}else{
			$('.prvideo-time').css({'display':'none'});
		}
	});
	$('.rpmissing-type #edit-missing-type-0, .rpmissing-type #edit-missing-type-1').change(function(){
		if($('.rpmissing-type #edit-missing-type-1').attr('checked')){
			$('.prvideo-time').css({'display' : 'block'});
		}else{
			$('.prvideo-time').css({'display' : 'none'});
		}
	});
});

function validate_report(){

	if($('#edit-missing-comment').val() == ''){
		alert('Please give valid comment');
		return false;
	}
	if($('.qtitle').val() == ''){
		alert('Please enter the title');
		return false;
	}
}

