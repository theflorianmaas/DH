<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

 // Connects to your Database 

 // Connects to your Database 
 
 $dataTriggerFunctionTitle = mysqli_query($db,"SELECT * FROM vwtriggerfunctiontitle where function_id = " . $_SESSION["id"] )
 or die(mysqli_error()); 

 $dataTriggerFunction = mysqli_query($db,"SELECT * FROM vwtriggerfunctionall where function_id = " . $_SESSION["id"] )
 or die(mysqli_error()); 
 

$TriggerFunctions = array(); 
 while($row = mysqli_fetch_assoc($dataTriggerFunction)) { 
    $TriggerFunctions[] = $row; 
 }  
 
?> 
<ul data-role="listview" class="jqm-list" id="mainlist" data-theme="b">
<?php 

while($infoTriggerFunctionTitle = mysqli_fetch_array($dataTriggerFunctionTitle)) 
{ 
?> 
 
<li style="margin-left:-10px;">
<div data-role="footer" data-theme="a" class="ui-grid-c"  data-position="fixed" style="margin: -7px -10px -10px 0;">
<?php 
 
 echo('<div  class="ui-block-a" style="width:5%; height:50px; margin-left:-0px;">');
 
 //('<a href="#pp' .$infoTriggerFunctionTitle['id']. '" data-rel="popup">');
 echo('<span class="sensor ' . $infoTriggerFunctionTitle['currentstatus_icon'] . ' dh-' . $infoTriggerFunctionTitle['trigger_type'] .  '"></span>');
 echo('<span class="timer '  . $infoTriggerFunctionTitle['timer_currentstatus'] . '">' . $infoTriggerFunctionTitle['timer'] .  '</span>');
 echo('<span class="evento '  . $infoTriggerFunctionTitle['event_status'] . '">' . $infoTriggerFunctionTitle['event'] .  '</span>');

 echo('</a>');
  
 echo('<span class="name">' .   $infoTriggerFunctionTitle['triggerTitle'] . '</span>');
 echo('</div>');
 
 //echo('<a href="sensordetails.php?id=' . $dataTriggerFunctionTitle['id'] . '" target=_self>');
 //echo('<div  class="ui-block-b" style="width:30%; text-align: right;">');
 //echo('<span class="value">' . $infoSensors['currentvalue']. '</span>');
 //echo('<span class="um">' . $infoSensors['um']. '</span>');
 //echo('</div>');

 echo('<div  class="ui-block-c" style="width:25%">');
 
 /*
 echo('<div class="canvas">');
 echo('<canvas id="c' . $infoSensors['id'] . '" ></canvas>'); 
 echo('<script>');
 echo("sensor('c" . $infoSensors['id'] . "'," . ($infoSensors['currentvalue'] - $infoSensors['scalemin']) . ",0," . ($infoSensors['scalemax'] - $infoSensors['scalemin']) . ",'" . $infoSensors['hexCode'] . "','" . "');");
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
 */
 echo('</div>');

echo('<div  class="ui-block-d" style="text-align: left;">');
 
 $cnt = 1;
 $cnt2 = 5;
 foreach ($TriggerFunctions as $selectedTriggerFunctions) 
 { 
 IF ($selectedTriggerFunctions['id'] == $infoTriggerFunctionTitle['id'])  {  
 echo('<span class="actu' . $cnt  . ' ' . $selectedTriggerFunctions['currentstatus_icon'] . ' dh-'  . $selectedTriggerFunctions['actuator_icon'] . '"></span>');
 echo('<span class="room' . $cnt . '" '  . ' dh-'  . $selectedTriggerFunctions['room'] . '"></span>');

 $cnt = $cnt + 1;
 $cnt2 = $cnt2 + 1;
 }
 }
//</a>

?> 

</div>

</li>
<?php } ?> 
</ul>
<script type="text/javascript">
       $('#mainlist').listview().listview('refresh');
</script>


