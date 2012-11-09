$(document).ready(function(){
	webroot = "http://10.102.6.113/spoken_tutorial_org/stupload/";
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
			    $('.poll-form').html("<img src='http://10.102.6.113/spoken_tutorial_org/ajax-loader.gif' />");
			},
			success : function(data){
				output = JSON.parse(data);
				console.log('ssss');
				console.log(output);
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
});
