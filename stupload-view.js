
$(document).ready(function(){
	
	webroot = "http://localhost/tester/stupload/";
	loading_image = "<img src='http://localhost/tester/ajax-loader.gif' />";

	$('.search').click(function(){
		var foss = $('.foss_category').val();
		var lang = $('.language').val();
		if(foss == '' || lang == ''){
			alert('Please select FOSS category and language');
		}else{
			$.ajax({
				type : 'POST',
				url : webroot + "get_tutorials_list",
				data : {
					'foss' : foss, 'lang' : lang
				},
				beforeSend: function() {
				    $('.list_data').html(loading_image);
				},
				success : function(data){
					output = JSON.parse(data);
					if(output){
						flag = $('.flag').val();
						var html_data = "<table align='center' cellpadding='5' style='width : 100%;'><tr><th>Sno</th><th>FOSS Category</th><th>Level</th><th>Language</th><th colspan='2'>Tutorial Name</th>";
						html_data += "</tr>";
						for (var i=0; i < output.length; i++)
						{
							html_data += "<tr style='text-align : left;'><td>"+ output[i].order_code +"</td><td>" + output[i].foss_category + "</td><td>" + output[i].tutorial_level + "</td><td>" ;
							html_data += output[i].language + "</td><td>" + output[i].tutorial_name + "</td><td>";

							if(flag == '0'){
								html_data += "<a href='" + webroot +"upload/review/" + output[i].trid + "'>Review</a></td></tr>";
							}else{
								html_data += "<a href='" + webroot +"play_video?tr=" + output[i].trid + "'>View</a></td></tr>";
							}
						}
						html_data += "</table>";
						$('.list_data').html(html_data);
					}else{
						$('.list_data').html('Somthing went wrong, Please refresh the page');
					}
				}
			});	
		}
	});
	$('.update_search').click(function(){
		var foss = $('.foss_category').val();
		var lang = $('.language').val();
		if(foss == '' || lang == ''){
			alert('Please select FOSS category and language');
		}else{
			$.ajax({
				type : 'POST',
				url : webroot + "get_update_tutorial_search",
				data : {
					'foss' : foss, 'lang' : lang
				},
				beforeSend: function() {
				    $('.list_data').html(loading_image);
				},
				success : function(data){
					output = JSON.parse(data);
					if(output){
						console.log(output);
						var html_data = '';
						if(output.length == 0){
							html_data = "<center><h4>List is empty</h4></center>";
						}else{
							html_data = "<table align='center' cellpadding='5' style='width : 100%;'><caption style='text-align : left;'><b>FOSS Category: " + output[0].foss_category + "<br />Language: " + output[0].language + "</b><br /></caption><tr>";
							html_data += "<th>Sno</th><th>Level</th><th>Tutorial Name</th><th>Outline</th><th>Script</th><th>Video</th><th>Slides</th>";
							html_data += "<th>Assignment</th><th>Code</th><th></th></tr>";
							for (var i=0; i < output.length; i++)
							{
								html_data += "<tr style='text-align : left;'><td>"+ output[i].order_code +"</td><td>" + output[i].tutorial_level + "</td><td>" + output[i].tutorial_name + "</td>";
								if(output[i].tutorial_outline == 'pending')
									html_data += "<td class='code_red'>&#8855;</td>";
								else
									html_data += "<td class='code_green'>&#10004</td>";
								if(output[i].tutorial_script == 'pending')
									html_data += "<td class='code_red'>&#8855;</td>";
								else
									html_data += "<td class='code_green'>&#10004</td>";
								if(output[i].tutorial_video == 'pending')
									html_data += "<td class='code_red'>&#8855;</td>";
								else
									html_data += "<td class='code_green'>&#10004</td>";
								if(output[i].tutorial_slide == 'pending')
									html_data += "<td class='code_red'>&#8855;</td>";
								else
									html_data += "<td class='code_green'>&#10004</td>";
								if(output[i].tutorial_assignment == 'pending')
									html_data += "<td class='code_red'>&#8855;</td>";
								else if(output[i].tutorial_assignment == 'notrequired')
									html_data += "<td class='code_green'>&#8722;</td>";
								else
									html_data += "<td class='code_green'>&#10004</td>";
								if(output[i].tutorial_code == 'pending')
									html_data += "<td class='code_red'>&#8855;</td>";
								else if(output[i].tutorial_code == 'notrequired')
									html_data += "<td class='code_green'>&#8722;</td>";
								else
									html_data += "<td class='code_green'>&#10004</td>";
								html_data += "<td><a href='" + webroot +"update_index/%3Ftr%3D" + output[i].trid + "'>Update</a></td></tr>";
							}
							html_data += "</table>";
						}
						
						$('.list_data').html(html_data);
					}else{
						$('.list_data').html('Somthing went wrong, Please refresh the page');
					}
				}
			});	
		}
	});
});
