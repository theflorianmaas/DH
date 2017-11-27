<?php 

if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");
?>


<?php 

$dataRooms = mysqli_query($db,"SELECT * FROM vwrooms WHERE status = 1") or die(mysql_error()); 
$dataSensors = mysqli_query($db,"SELECT * FROM vwsensorsmap") or die(mysql_error()); 
$dataActuators = mysqli_query($db,"SELECT * FROM vwactuatorsmap") or die(mysql_error()); 

$Sensors = array(); 
 while($row = mysqli_fetch_assoc($dataSensors)) { 
    $Sensors[] = $row; 
} 

$Actuators = array(); 
 while($row = mysqli_fetch_assoc($dataActuators)) { 
    $Actuators[] = $row; 
} 

while($selectedRoom = mysqli_fetch_array($dataRooms)) 
 {
 
echo('<div id="c'.$selectedRoom['id'].'" style="position:absolute; top:' . $selectedRoom['x'] . 'px; left:'. $selectedRoom['y'] .'px; z-index:1">');
//echo('<img id="sensor" class="sensor dh-time" style="color:#ff0;"></img>');
echo('<canvas id="r'.$selectedRoom['id'].'" width="'.$selectedRoom['w']. '" height="'.$selectedRoom['h']. '" style="border:1px solid #c3c3c3;">');
//echo('<canvas id="r4" width="100" height="105" style="border:1px solid #c3c3c3;">');
echo('Your browser does not support the HTML5 canvas tag.');
echo('</canvas>');
echo('<script>');
echo("room('r".$selectedRoom['id']."',".$selectedRoom['w'].",".$selectedRoom['h'].");");
echo('</script>');

foreach ($Sensors as $selectedSensor) 
 { 
 IF ($selectedSensor['room'] == $selectedRoom['id'])  { 
  echo('<script>');
  
  //item('r4','S',10,10,70,"e039","23.1 C°");
  
  echo("item('r".$selectedSensor['room']."','S',".$selectedSensor['id'].",".$selectedSensor['x']. "," .$selectedSensor['y']. ",'" . $selectedSensor['code']  . "','" . $selectedSensor['currentvalue']  . " " . $selectedSensor['um'] . "'," .$selectedSensor['status']. ");");
  echo('</script>');
 }
 }
 
 foreach ($Actuators as $selectedActuator) 
 { 
 IF ($selectedActuator['room'] == $selectedRoom['id'])  { 
  echo('<script>');
  
  echo("item('r".$selectedActuator['room']."','A',".$selectedActuator['id'].",".$selectedActuator['x']. "," .$selectedActuator['y']. ",'" . $selectedActuator['code']  . "',''," .$selectedActuator['status']. ");");
  
  
  //echo("actuator_map('r".$selectedActuator['room']."','2','2','" . $selectedActuator['sensor_type']  . "','" . $selectedActuator['currentstatus']  .   "'," . $cnt_a .");");
  echo('</script>');
 }
 }


echo('</div>');
 } 

?>




<script>
//item('r2','A',9,20,30,"e028","");
//item('A',10,50,70,"e006","");
//item('r4','S',10,10,70,"e039","23.1 C°");
</script>


</script>
