<?php 

if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

$dataRooms = mysqli_query($db,"SELECT * FROM vwrooms WHERE status = 1") or die(mysql_error()); 
$dataSensors = mysqli_query($db,"SELECT * FROM vwsensorsmap WHERE status = 1") or die(mysql_error()); 
$dataActuators = mysqli_query($db,"SELECT * FROM vwactuatorsmap WHERE status = 1") or die(mysql_error()); 

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
$cnt = 0;
$cnt_a = 0;
echo('<div id="c'.$selectedRoom['id'].'" style="position:absolute; top:' . ($selectedRoom['x']+0) . 'px; left:'. ($selectedRoom['y']+0) .'px; z-index:1">');
//echo('<img id="sensor" class="sensor dh-time" style="color:#ff0;"></img>');
echo('<canvas id="r'.$selectedRoom['id'].'" width="'.($selectedRoom['w']). '" height="'.($selectedRoom['h']). '" style="border:0px solid #c3c3c3;">');
//echo('<canvas id="r4" width="100" height="105" style="border:1px solid #c3c3c3;">');
echo('Your browser does not support the HTML5 canvas tag.');
echo('</canvas>');
echo('<script>');
echo("room_map('r".$selectedRoom['id']."',".$selectedRoom['w'].",".$selectedRoom['h'].",'".$selectedRoom['code']."');");
echo('</script>');

foreach ($Sensors as $selectedSensor) 
 { 
 IF ($selectedSensor['room'] == $selectedRoom['id'])  { 
  echo('<script>');
  echo("sensor_map('r".$selectedSensor['room']."',".$selectedSensor['x']. "," .$selectedSensor['y']. ",'" . $selectedSensor['code']  . "'," . $selectedSensor['currentvalue']  . ",'" . $selectedSensor['um'] .   "'," . $cnt .");");
  echo('</script>');
  $cnt = $cnt+1;
 }
 }
 
 foreach ($Actuators as $selectedActuator) 
 { 
 IF ($selectedActuator['room'] == $selectedRoom['id'])  { 
  echo('<script>');
  echo("actuator_map('r".$selectedActuator['room']."'," .$selectedActuator['x']. ",".$selectedActuator['y'].",'" . $selectedActuator['code']  . "','" . $selectedActuator['currentstatus']  .   "'," . $cnt_a .");");
  echo('</script>');
  $cnt_a = $cnt_a+1;
 }
 }


echo('</div>');


//var c = document.getElementById("myCanvas");
//var ctx = c.getContext("2d");
//ctx.fillStyle = "#FF0000";
//ctx.fillRect(0,0,100,100);
//ctx.font = '14px Dhicon';
//ctx.textBaseline = 'top';
//ctx.fillStyle = 'blue';
//var h= 'e01a';
//ctx.fillText  (String.fromCharCode(parseInt(h, 16)), 5, 10);
//</script>

}
?>

	


