$(document).ready(function(){
	webroot = "http://10.102.152.101/spoken_tutorial_org/stupload/";
	$('.uptn_foss_category_name').change(function(){
		foss = $(this).val();
		level = $('.uptn_tutorial_level').val();
		$.ajax({
			type : 'POST',
			url : webroot + "get_tutorial_levels",
			data : {
				'foss' : foss, 'level' : level
			},
			success : function(data){
				output = JSON.parse(data);
				console.log('ssss');
				console.log(output);
				var html_data = "<option val=''>Order</option>";
				if(output){
					for (var i=0; i < output.length; i++)
					{
						html_data += "<option value='"+ output[i] +"'>" + output[i] + "</option>";	
					}
					console.log(html_data);
					$('.uptn_tutorial_order_no').html(html_data);
				}else{
					
				}
			}
		});
	});
});
