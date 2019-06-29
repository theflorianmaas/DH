<?php
$datamethods = mysqli_query($db,"SELECT * FROM tbactuatormethodvalue order by 3,2") 
or die(mysqli_error()); 

$Methods = array(); 
while($row = mysqli_fetch_assoc($datamethods)) { 
	$Methods[] = $row; 
} 

?> 
<ul data-role="listview" id="mainlist" data-theme="b">
<?php 


$dataArr = array();

while($info = mysqli_fetch_array($data)) 
{ 
	$id =  "'" . $info['id'] . "'"; 
	$dataArr[] = $info;
	echo('<li style="margin-left:-10px;">');
	echo ('<div data-theme="b" class="ui-grid-b"  data-position="fixed" style="margin: -7px -10px -10px 0;">');
	echo ('<div class="ui-block-a" style="width:10%; margin-left:-0px;">');
	echo ('<span class="actuator ' . $info['currentstatusicon'] . ' dh-' .$info['actuatortypeicon']. '"></span>');
	echo ('</div>');	
	echo ('<div class="ui-block-b" style="width:50%; margin-left:-0px;">');
	echo ('<span class="actuatorname">' .$info['descr']. '</span>');
	echo ('</div>');
	echo ('<div class="ui-block-c" style="width:40%; margin-left:-0px; '); 
	echo ('height:'. $info['websize']);
	echo ('px;">');
	echo ('<div  class="ui-block-c" style="height:62px; width:40%; margin-left:-0px;">');

	//INIZIA loop sui metodi
	foreach ($Methods as $selectedMethod) 
	{ 
		IF ($info['tbactuatortype_id'] == $selectedMethod['tbactuatortype_id'] AND $selectedMethod['tbactuator_id'] == $info['id'])  { 

			switch ($selectedMethod['tbactuatormethod_id'] ) {
			case ONOFF: //On-Off
				
				echo('<form>');											
				switch ($info['currentstatus'])  
				{ 
				case 0:	
					$value = $info['valueon']; 
					echo ('<div value="' . $value . '" name="a'. $info['id'] .'" id="a'. $info['id'] .'" class="method-on-off switch">');
					echo ('</div>');
					break;
				case 1:		
					$value = $info['valueoff'];
					echo ('<div value="' . $value . '" name="a'. $info['id'] .'" id="a'. $info['id'] .'" class="method-on-off switchtemp">');
					echo ('</div>');									
					break;
				}
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $value . '">');
				echo ('<input type="hidden" name="actuatortype" value="' .$selectedMethod['tbactuatortype_id'].'">');	
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $info['currentstatus'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');	
				?> 					
				<script>
				$("#a<?php echo($info['id']); ?>").click(function() { // changed	
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
						type: "POST",
						url: "php/setactuator.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.							
				});
				
				</script>
				
				<?php 

				echo('</form>');	
				break;
			case 1: //Slider
				echo ('<form>');
				echo ('<span class="method-slider" data-mini="true">');
				
				echo('<div id="vx' . $info['id'] . '">');
				//v=prefix for analog and servo input (slider)
				echo('<input  name="v' . $info['id'] . '" id="v' . $info['id'] . '" min="' . $info['valueoff'] . '" max="' . $info['valueon'] . '" value="' . $info['currentvalue'] . '" type="range"> ');
				echo('</div>');		

				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');	
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $info['currentstatus'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('</span>');

				?> 					
				<script>
				$("#vx<?php echo($info['id']); ?>").on( 'slidestop', function( event, ui ) { // changed
					play();	
					$.ajax({
type: "POST",
url: "php/setactuator.php",
data: $(this).parent().serialize()
						//success: function(dx){alert(dx);}// changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;

				// --- AC Methods section           ---//
				// --- Visible only if the AC is ON ---//
				
			case 3: //Fan 1
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="fan1 ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize(), // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
			case 4: //Fan 2
				if ($info['currentstatus']	== 1) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="fan2 ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();					
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
			case 5: //Fan 3
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="fan3 ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
			case 6: //Fan Auto
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="fanauto ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
			case 7: //Cool
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="cool ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;															

			case 8: //Dry
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="dry ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;

			case 9: //Heat
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="heat ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();-
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
			case 10: //Mode Auto
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="auto ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
				
			case 12: //Swing
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="swing ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
				
			case 11: //AC Temperature
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<div class="tempbox">');
					echo('<input type="text" data-wrapper-class="ui-custom" data-mini="false" name="input_temp" class="input_temp" value="' .$selectedMethod['value']. '" id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '"/></div>'); 
					?>
					<span class="plus">+</span>  
					<span class="minus">-</span>

					<script>
					$('.plus').on('click',function(){
						$('.input_temp').val(parseInt($('.input_temp').val())+1);
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});
					});

					$('.minus').on('click',function(){
						$('.input_temp').val(parseInt($('.input_temp').val())-1);
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						});									
					}); 
					</script>
					
					<?php 
					
					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');														
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					echo('</form>');
				} 
				break;	
				
				/* TV Remote section *********************************************/	
				/* TV Remote section *********************************************/	
				/* TV Remote section Sony ****************************************/	
			case 13: //Volume up

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pleft"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="sts" value="1">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;						 				

			case 14: //Volume down

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pright"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;						 				

			case 15: //Channel up

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pup"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;						 				

			case 16: //Channel down

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pdown"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;						 				

			case 17: //Mute

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pmute"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;						 				

			case 18: //Source

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="psource"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;							

			case 19: //Home

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="phome"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;		

			case 20: //red

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pred"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;									

			case 21: //green

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pgreen"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;						

			case 22: //yellow

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pyellow"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;						
				
			case 23: //blue

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pblue"');
				echo ('</span>');
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');													
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');							
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;																																		

			case 24: //up

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pkeyup"');
				echo ('</span>');
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');													
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');							
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;																																		

			case 25: //blue

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pkeydown"');
				echo ('</span>');
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');													
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');							
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;																																		

			case 26: //blue

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pkeyleft"');
				echo ('</span>');
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');													
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');							
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;																																		

			case 27: //right

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pkeyright"');
				echo ('</span>');
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');													
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');							
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;																																		

			case 28: //ok

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pkeyok"');
				echo ('</span>');
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');													
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');							
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;																																		

			case 29: //return

				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="pkeyreturn"');
				echo ('</span>');
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');													
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');							
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;

			case 30: //Color picker	

				if ($info['currentstatus']	== ACTIVE) { // if the RGB is ON
					echo ('<form name="rgb">');
					
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					
					//popup for color picker
					echo ('<div id="popup">');
					echo ('<div data-role="popup" data-theme="b" data-overlay-theme="a" class="ui-content" data-transition="pop" data-position-to="origin" id="cp' . $info['id'] . '">');
					echo ('</div>');
					
					
					echo('<div class="rgb" id="rgb' . $info['id'] . '">');
					echo ('<a href="#cp' . $info['id'] . '" data-rel="popup">');
					//r=prefix for rgb
					//echo(dechex($info['currentvalue']));
					echo ('<input type="hidden" name="value" id="r' . $info['id'] . '"  value="#' . str_pad(dechex($info['currentvalue']),6,"0",STR_PAD_LEFT) . '"/>');
					echo ('</a>');
					echo ('</div>');
					?> 	
					<script>
					$("#r<?php echo($info['id']); ?>").spectrum({
						//color: <?php echo( $info['currentvalue']); ?>,
						cancelText: 'Cancel',
						chooseText: 'Ok',
						flat: false,
						change: function(color) { 								
							document.getElementById("r<?php echo($info['id']); ?>").value = color.toHexString();			 				 
							play(); 
							$.ajax({
							type: "POST",
							url: "php/setactuatorvalue.php",
							data: $('form[name=rgb]').serialize() // changed
								//,success: function(data){ alert(data); }
							});
							return false; // avoid to execute the actual submit of the form.
							
						}
					});
					</script>
					
					<?php 
					echo('</form>');
				}
				break;
				
				// inizia sezione termostato
				
			case TMMODE: //Mode 
				if ($info['currentstatus']	== ACTIVE) { // if the Thermostat is ON
					echo ('<form>');

					if ($selectedMethod['value'] 	  == TMAUTO ) 
					{ $modeicon = "tmodeauto"; }	
					else if ($selectedMethod['value'] == TMAUTOPROG )
					{ $modeicon = "tmodeautoprog"; }
					else if ($selectedMethod['value'] == TMMANUAL )
					{ $modeicon = "tmodemanual"; }
					else if ($selectedMethod['value'] == TMHUB )
					{ $modeicon = "tmodehub"; }
					
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="'.$modeicon.' active">');

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					
					?> 					
					<script>		
					$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
						play();
						$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
							//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});
					</script>
					
					<?php 
					echo('</form>');
				}						
				break;
				
			case TMPROG: //Program
				if ($info['currentstatus']	== ACTIVE) { // if the Thermostat is ON
					if ($selectedMethod['visible'] == 1){
						echo ('<form>');

						if ($selectedMethod['value'] 	  == TMDAILY ) 
						{ $modeicon = "tprogdaily"; }	
						else if ($selectedMethod['value'] == TMWDAYS )
						{ $modeicon = "tprogwdays"; }
						else if ($selectedMethod['value'] == TMWEND )
						{ $modeicon = "tprogwend"; }

						echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="'.$modeicon.' active">');

						echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
						echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
						echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
						echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
						echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
						echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
						echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
						echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
						echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
						echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
						echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
						echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
						echo ('</span>');
						
						?> 					
						<script>		
						$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
							play();
							$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
								//,success: function(data){ alert(data); }
							});
							return false; // avoid to execute the actual submit of the form.
						});
						</script>
						
						<?php 
						echo('</form>');
					}	
					break;
				}
			case TMFIRE: //TOGGLE FIRE
				if ($info['currentstatus']	== ACTIVE) { // if the AC is ON
					echo ('<form>');
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tfire ');
					if ($selectedMethod['value'] == INACTIVE ) 
					{ echo('inactive">');	}	
					else 
					{ echo('active">'); }	

					echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
					echo ('</span>');
					/*
							?> 	
							<script>		
							$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
							play();
							$.ajax({
								type: "POST",
								url: "php/setactuatorvalue.php",
								data: $(this).parent().serialize() // changed
								});
							return false; // avoid to execute the actual submit of the form.
							});
							</script>
							
							<?php 
							*/
					echo('</form>');
				}
				break;	
				
			case TMADJUST: //TM Temperature
				if ($info['currentstatus']	== ACTIVE) { 
					if ($selectedMethod['visible'] == 1){
						echo ('<form>');
						echo ('<div class="tempbox">');
						echo('<input type="text" data-wrapper-class="ui-custom" data-mini="false" name="input_temp_t" class="input_temp_t" value="' .$selectedMethod['value']. '" id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '"/></div>'); 
						?>
						<span class="tplus">+</span>  
						<span class="tminus">-</span>
						<script>
						$('.tplus').on('click',function(){
							$('.input_temp_t').val(parseInt($('.input_temp_t').val())+1);
							$.ajax({
							type: "POST",
							url: "php/setactuatorvalue.php",
							data: $(this).parent().serialize() // changed
								//,success: function(data){ alert(data); }
							});
						});

						$('.tminus').on('click',function(){
							$('.input_temp_t').val(parseInt($('.input_temp_t').val())-1);
							$.ajax({
							type: "POST",
							url: "php/setactuatorvalue.php",
							data: $(this).parent().serialize() // changed
							});									
						}); 
						</script>
						
						<?php 
						
						echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');														
						echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
						echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
						echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
						echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
						echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
						echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
						echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
						echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
						echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
						echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
						echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
						echo ('</span>');
						echo('</form>');
					}
				} 
				break;	
				
			case 35: //smart light color temp
				if ($info['currentstatus']	== ACTIVE)
				{ 	
					echo ('<form>');				
					switch ($selectedMethod['value'])  
					{ 
					case 0:	//cold light
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_cold_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_warm_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_morewarm_off"></div>');
						break;
					case 1:	//cold light
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_cold_on"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_warm_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_morewarm_off"></div>');
						break;
					case 2:	//warm light
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_cold_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_warm_on "></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_morewarm_off"></div>');
						break;
					case 3:	//more warm light
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_cold_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_warm_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_morewarm_on"></div>');
						break;							
					}
					
					echo ('<input type="hidden" name="v' .$info['id']. '" value="' . $info['currentvalue'] . '"> ');													
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					//echo ('<input type="hidden" name="sts" id="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="sts" id="sts" value="0">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');													
					?>
					
					<script>						
					$("#<?php echo("vx1" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 1;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});
					
					$("#<?php echo("vx2" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 2;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});
					
					$("#<?php echo("vx3" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 3;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});					
					</script>
					
					<?php 
					echo('</form>');
				}
				break;	

				case 36: //Smart Light Dimmer
				echo ('<form>');
				echo ('<span class="method-slider" data-mini="true">');
				
				echo('<div id="vxx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '">');
				//v=prefix for analog and servo input (slider)
				echo('<input  name="v' .$info['id']. '" id="v' .$info['id']. '" min="' . $info['valueoff'] . '" max="' . $info['valueon'] . '" value="' . $selectedMethod['value'] . '" type="range"> ');
				echo('</div>');
				
				echo ('</span>');
				
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id']. '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id']. '">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime']. '">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime']. '">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $selectedMethod['value'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');	

				?> 					
				<script>
				$("#<?php echo("vxx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").on( 'slidestop', function( event, ui ) { // changed
					play();	
					$.ajax({
					type: "POST",
					url: "php/setactuatorvalue.php",
					data: $(this).parent().parent().serialize()
					//,success: function(data){alert(data);}// changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>			
				<?php 
				echo('</form>');				
				break;
			
			case 37: //Smart light color picker	

				if ($info['currentstatus']	== ACTIVE) { // if the RGB is ON
					echo ('<form>');				
					if ($selectedMethod['value'] == 1)  
					{ 
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_1_on"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_2_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_3_off"></div>');
						echo ('<div id="vx4' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_4_off"></div>');
						echo ('<div id="vx5' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_5_off"></div>');
						echo ('<div id="vx6' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_6_off"></div>');
					}
					else if ($selectedMethod['value'] == 9)  
					{ 	//green light
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_1_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_2_on"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_3_off"></div>');
						echo ('<div id="vx4' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_4_off"></div>');
						echo ('<div id="vx5' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_5_off"></div>');
						echo ('<div id="vx6' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_6_off"></div>');
					}
					else if ($selectedMethod['value'] == 7)	//yellow light
					{
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_1_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_2_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_3_on"></div>');
						echo ('<div id="vx4' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_4_off"></div>');
						echo ('<div id="vx5' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_5_off"></div>');
						echo ('<div id="vx6' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_6_off"></div>');
					}
					else if ($selectedMethod['value'] == 11)	//red light
					{
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_1_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_2_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_3_off"></div>');
						echo ('<div id="vx4' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_4_on"></div>');
						echo ('<div id="vx5' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_5_off"></div>');
						echo ('<div id="vx6' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_6_off"></div>');
					}
					else if ($selectedMethod['value'] == 16)	//purple light
					{
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_1_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_2_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_3_off"></div>');
						echo ('<div id="vx4' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_4_off"></div>');
						echo ('<div id="vx5' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_5_on"></div>');
						echo ('<div id="vx6' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_6_off"></div>');
					}
					else if ($selectedMethod['value'] == 18)	//blue light
					{
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_1_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_2_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_3_off"></div>');
						echo ('<div id="vx4' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_4_off"></div>');
						echo ('<div id="vx5' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_5_off"></div>');
						echo ('<div id="vx6' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_6_on"></div>');																															
					}
					else //other colors
					{
						echo ('<div id="vx1' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_1_off"></div>');
						echo ('<div id="vx2' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_2_off"></div>');
						echo ('<div id="vx3' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_3_off"></div>');
						echo ('<div id="vx4' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_4_off"></div>');
						echo ('<div id="vx5' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_5_off"></div>');
						echo ('<div id="vx6' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="tempcolor_6_off"></div>');																															
					}
					
					echo ('<input type="hidden" name="v' .$info['id']. '" value="' . $info['currentvalue'] . '"> ');													
					echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
					echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
					echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
					echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
					echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
					echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
					//echo ('<input type="hidden" name="sts" id="sts" value="' . $selectedMethod['value'] . '">');
					echo ('<input type="hidden" name="sts" id="sts" value="0">');
					echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
					echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
					echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
					echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');													
					?>
					
					<script>						
					$("#<?php echo("vx1" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 1;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});
					
					$("#<?php echo("vx2" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 19;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});
					
					$("#<?php echo("vx3" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 7;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});	
					
					
					$("#<?php echo("vx4" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 11;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});	
					
					$("#<?php echo("vx5" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 16;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});	
					
					$("#<?php echo("vx6" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed	
						document.getElementById("sts").value = 18;							
						play();							
						$(this).removeClass("run-animation").addClass("run-animation");
						$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
						});
						return false; // avoid to execute the actual submit of the form.
					});					
					</script>
					
					<?php 
					echo('</form>');
				}
				break;						
					
					
	
			case 38: //On-Off Smartlight
			
				echo('<form>');											
				switch ($info['currentstatus'])  
				{ 
				case 0:	
					$value = $info['valueon']; 
					echo ('<div value="' . $value . '" name="a'. $info['id'] .'" id="a'. $info['id'] .'" class="method-on-off switch">');
					echo ('</div>');
					break;
				case 1:		
					$value = $info['valueoff'];
					echo ('<div value="' . $value . '" name="a'. $info['id'] .'" id="a'. $info['id'] .'" class="method-on-off switchtemp">');
					echo ('</div>');									
					break;
				}
				
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $value . '">');
				echo ('<input type="hidden" name="actuatortype" value="' .$selectedMethod['tbactuatortype_id'].'">');	
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="' . $info['currentstatus'] . '">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');	
				
				?> 					
				<script>
				$("#a<?php echo($info['id']); ?>").click(function() { // changed	
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
						type: "POST",
						url: "php/setactuatorvalue.php",
						data: $(this).parent().serialize() // changed
						//,success: function(data){ alert(data); }
					});
					return false; // avoid to execute the actual submit of the form.							
				});
				
				</script>
				
				<?php 

				echo('</form>');	
				break;
				
				case AWNINGDOWN: //40 Awning down
				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awdown"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="0">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;	
				
				case AWNINGUP: //39 wning up
				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awup"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="1">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;	
				
				case AWNINGSTOP: //41 Awning STOP
				echo ('<form>');
				echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awstop"');
				echo ('</span>');
				echo ('<input type="hidden" name="v' . $info['id'] . '" value="' . $info['currentvalue'] . '"> ');							
				echo ('<input type="hidden" name="actuatortype" value="' . $selectedMethod['tbactuatortype_id'] . '">');
				echo ('<input type="hidden" name="actuatormethod" value="' .$selectedMethod['tbactuatormethod_id'].'">');	
				echo ('<input type="hidden" name="fadingtime" value="' .$info['fadingtime'].'">');
				echo ('<input type="hidden" name="temptime" value="' .$info['temptime'].'">');							
				echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
				echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
				echo ('<input type="hidden" name="sts" value="1">');
				echo ('<input type="hidden" name="node" value="' .  $info['tbnode_id'] . '">');
				echo ('<input type="hidden" name="output" value="' . $info['tboutput_id'] . '">');
				echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
				echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');
				echo ('<input type="hidden" name="brand" value="Sony">');
				
				?> 					
				<script>		
				$("#<?php echo("vx" .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']); ?>").click(function() { // changed
					play();
					$(this).removeClass("run-animation").addClass("run-animation");
					$.ajax({
type: "POST",
url: "php/setactuatorvalue.php",
data: $(this).parent().serialize() // changed
					});
					return false; // avoid to execute the actual submit of the form.
				});
				</script>
				
				<?php 
				echo('</form>');
				
				break;	

				case AWNINGSTATUS0: //42 Awning 0 STATUS
				echo ('<form>');
				if ($selectedMethod['value'] == 0)  { 
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts00"');
				}
				elseif ($selectedMethod['value']> 0 and $selectedMethod['value'] <= 12) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts01"');
				}
				elseif ($selectedMethod['value'] > 12 and $selectedMethod['value'] <= 25) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts02"');
				}
				elseif ($selectedMethod['value']> 25 and $selectedMethod['value'] <= 38) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts03"');
				}
				elseif ($selectedMethod['value'] > 38 and $selectedMethod['value'] <= 50) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts04"');
				}
				elseif ($selectedMethod['value'] > 50 and $selectedMethod['value'] <= 62) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts05"');
				}
				elseif ($selectedMethod['value'] > 62 and $selectedMethod['value'] <= 75) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts06"');
				}
				elseif ($selectedMethod['value'] > 75 and $selectedMethod['value'] <= 88) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts07"');
				}
				elseif ($selectedMethod['value'] > 88 and $selectedMethod['value'] <= 100) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts08"');
				}
				echo ('</span>');
				echo('</form>');
				
				break;	
				
				case AWNINGSTATUS1: //43 Awning 1 STATUS
				echo ('<form>');
				if ($selectedMethod['value'] == 0)  { 
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts10"');
				}
				elseif ($selectedMethod['value']> 0 and $selectedMethod['value'] <= 12) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts11"');
				}
				elseif ($selectedMethod['value'] > 12 and $selectedMethod['value'] <= 25) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts12"');
				}
				elseif ($selectedMethod['value']> 25 and $selectedMethod['value'] <= 38) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts13"');
				}
				elseif ($selectedMethod['value'] > 38 and $selectedMethod['value'] <= 50) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts14"');
				}
				elseif ($selectedMethod['value'] > 50 and $selectedMethod['value'] <= 62) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts15"');
				}
				elseif ($selectedMethod['value'] > 62 and $selectedMethod['value'] <= 75) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts16"');
				}
				elseif ($selectedMethod['value'] > 75 and $selectedMethod['value'] <= 88) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts17"');
				}
				elseif ($selectedMethod['value'] > 88 and $selectedMethod['value'] <= 100) {
					echo ('<span id="vx' .$selectedMethod['tbactuatortype_id'].$selectedMethod['tbactuatormethod_id'].$selectedMethod['tbactuator_id']. '" class="awsts18"');
				}
				echo ('</span>');
				echo('</form>');
				
				break;	


				
			}				
		}
	}

	echo ('</div>');
	echo ('</div>');
	echo('</li>');
} 
?> 
