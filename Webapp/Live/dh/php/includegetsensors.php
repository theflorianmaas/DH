<ul data-role="listview" class="jqm-list" id="mainlist" data-theme="b">
<?php 

 while($infoSensors = mysqli_fetch_array($dataSensors)) 
 { 
?> 
 
<li style="margin-left:-10px;">
<div data-role="footer" data-theme="a" class="ui-grid-c"  data-position="fixed" style="margin: -6px -10px -10px 0;">
<?php 
 
 echo('<div  class="ui-block-a" style="width:5%; height:70px; margin-left:-0px;">');
 
 echo('<a href="#pp' .$infoSensors['id']. '" data-rel="popup">');
 echo('<span class="sensor dh-' . $infoSensors['sensor_type'] .  '" style="color:' . $infoSensors['hexcode']. ';"></span>');
 echo('<span class="roomsensor dh-' . $infoSensors['room'] .  '"></span>');
 echo('</a>');
  
 echo('<span class="name">' .   $infoSensors['roomname'] . ' - ' .$infoSensors['descr'] . '</span>');
 echo('</div>');
 
 echo('<a href="sensordetails.php?id=' . $infoSensors['id'] . '&nid=' . $infoSensors['tbnode_id'] . '" target=_self>');
 echo('<div  class="ui-block-b" style="width:30%; text-align: right;">');
 echo('<span class="value">' . $infoSensors['currentvalue']. '</span>');
 echo('<span class="um">' . $infoSensors['um']. '</span>');
 echo('</div>');

 echo('<div  class="ui-block-c" style="width:25%">');
 
 
 echo('<div class="canvas">');
 echo('<canvas id="c' . $infoSensors['id'] . '" ></canvas>'); 
 echo('<script>');
 echo("sensor('c" . $infoSensors['id'] . "'," . ($infoSensors['currentvalue'] - $infoSensors['scalemin']) . ",0," . ($infoSensors['scalemax'] - $infoSensors['scalemin']) . ",'" . $infoSensors['hexcode'] . "','" . "');");
 echo('</script>');
 foreach ($Triggers as $selectedTrigger) 
 { 
 IF ($selectedTrigger['sensor_id'] == $infoSensors['id'])  { 
  echo('<script>');
  echo("event('c" . $infoSensors['id'] . "'," . $selectedTrigger['valuemin'] . "," . $selectedTrigger['valuemax'] . "," . $infoSensors['scalemin'] . "," . $infoSensors['scalemax'] . ");");
  echo('</script>');
 }
 }
 
 echo('</div>');
 echo('</div></a>');
 
 echo('<div  class="ui-block-d" style="text-align: left;">');
 
 $cnt = 1;
 $cnt2 = 5;
 foreach ($Triggers as $selectedTrigger) 
 { 
 IF ($selectedTrigger['sensor_id'] == $infoSensors['id'])  
 { 
	echo('<a href="php/setresetnotification.php?id=' . $selectedTrigger['id'] . '&nid=' . $_SESSION['nid'] . '" target="_self">');
	echo('<span class="actu' . $cnt  . ' ' . $selectedTrigger['currentstatus_icon'] . ' dh-'  . $selectedTrigger['actuator_icon'] . '"></span>');
	foreach ($resetEmails as $selectedTEmail) 
	{ 
		IF ($selectedTEmail['tbtrigger_id'] == $selectedTrigger['id'] )
		{ 
			echo('<span class="reset' . $cnt  . '"></span>');
		}
	}	
	echo('<span class="actu' . $cnt2 . ' ' . $selectedTrigger['trigger_type'] . '"></span>');
	echo('</a>');
	$cnt = $cnt + 1;
	$cnt2 = $cnt2 + 1;
			
 }
 }

?> 
</a>
</div>

</li>
<?php } ?> 
</ul>
