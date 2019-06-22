<?php 
?>	
<meta name="viewport" content="width=device-width, initial-scale=1" />  
<meta name="HandheldFriendly" content="True">
<meta name="MobileOptimized" content="240"/>
<meta name="PalmComputingPlatform" content="true"/>
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="mobile-web-app-capable" content="yes">  

<link rel="stylesheet" href="http://code.jquery.com/mobile/1.4.5/jquery.mobile.structure-1.4.5.min.css" />
<script src="http://code.jquery.com/jquery-1.11.1.min.js"></script>
<script src="http://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.js"></script>	
<link rel="stylesheet" href="themes/dhjqm.min.css" />
<link rel="stylesheet" href="themes/icons2.css" />
<link rel="stylesheet" href="themes/dhicon.css" />
<link rel="stylesheet" href="themes/icons.css" />
<link rel="stylesheet" href="themes/awicon.css" />
<script type="text/javascript" src="js/gauge.js"></script>	
<script type="text/javascript" src="js/map.js"></script>
<link rel="stylesheet" href="js/jquery.numpad.css">
<script src="js/jquery.numpad.js"></script>	
<script src="js/spectrum.js"></script>
<link rel="stylesheet" href="themes/spectrum.css" />

<script>
function play() { 
	var click = new Audio('themes/tick.mp3');
	click.play();
	return true;
} 	

function checkCode(id, code)
{
	$.post('php/checkpin.php', { id: id, code : code}, 
   	function(result){
       	submitForm(id, result);
	});
}

function submitForm(id, res)
{
	if (res == 1) //if code Ok else do nothing
		{
			play();
			$(this).removeClass("run-animation").addClass("run-animation");
			$.ajax({
				type: "POST",
					url: "php/setactuator.php",
					data: $('#f'+ id).serialize()
					//success: function(data){ alert(id)}
			});
			return false; // avoid to execute the actual submit of the form.
		}	
}							
										
</script>

