<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

// Connects to your Database 
 $dataSensors = mysqli_query($db,"SELECT * FROM vwdashboardsensorsdetails WHERE id = '" . $_SESSION["id"] . "'") 
 or die(mysqli_error()); 

 $dataActions = mysqli_query($db,"SELECT * FROM vwtriggersensordetails WHERE sensor_id = '" . $_SESSION["id"] . "' order by sequence") 
 or die(mysqli_error()); 
 
 $dataEvents = mysqli_query($db,"SELECT * FROM vwevents WHERE tbsensor_id = " . $_SESSION["id"] . " order by sequence")
 or die(mysqli_error()); 
 
 $dataEventTitle = mysqli_query($db,"SELECT distinct id, event_status, descr as eventTitle FROM vwevents where tbsensor_id = " . $_SESSION["id"] )
 or die(mysqli_error()); 

 $dataTriggerTitle = mysqli_query($db,"SELECT * FROM vwtriggersall b where sensor_id = " . $_SESSION["id"])
 or die(mysqli_error()); 

 //$dataTriggerTitle = mysqli_query($db,"SELECT * FROM vwtriggertitle where id in (select id from vwtriggersall where sensor_id = '" . $_SESSION["id"] . "')")
 //or die(mysqli_error()); 

 
$Actions = array(); 
 while($row = mysqli_fetch_assoc($dataActions)) { 
    $Actions[] = $row; 
 } 
 
$Events = array(); 
 while($row = mysqli_fetch_assoc($dataEvents)) { 
    $Events[] = $row; 
 }

$EventTitle = array(); 
 while($row = mysqli_fetch_assoc($dataEventTitle)) { 
    $EventTitle[] = $row; 
 }  
 
$TriggerTitle = array(); 
 while($row = mysqli_fetch_assoc($dataTriggerTitle)) { 
    $TriggerTitle[] = $row; 
 }   
  
 while($infoSensors = mysqli_fetch_array($dataSensors)) 
 { 
	echo('<div class="canvasbig">');
		echo('<canvas id="c"></canvas>');
		echo('<script>');
		 echo("sensor('c'," . ($infoSensors['currentvalue'] - $infoSensors['scalemin']) . ",0," . ($infoSensors['scalemax'] - $infoSensors['scalemin']) . ",'" . $infoSensors['hexcode'] . "','" . "');");	 
		//foreach ($Events as $selectedEvent) 
		//{ 
			//echo("event('c'," . $selectedEvent['valmin'] . "," . $selectedEvent['valmax'] . "," . $infoSensors['scalemin'] . "," . $infoSensors['scalemax'] . ");");
		//}
		echo('</script>');
	 echo('<span class="sensorbig dh-' .$infoSensors['sensor_type']. '"></span>');	
	 echo('</div>');	 
	 echo('<div class="valueframe">');
		echo('<span class="umbig">' . $infoSensors['um']. '</span>');
		echo('<span class="valuebig">' . $infoSensors['currentvalue']. '</span>');
		//echo('<span class="pin">Pin: ' . $infoSensors['pin_number'] .'</span>');
	 echo('</div>');
 }

echo('<a href="trigger.php?id=' . $_SESSION["id"] . '" target="_self" >');
echo('<div class="mainframe">');
$top = 0;	

if (count($TriggerTitle) > 0) { 

foreach ($TriggerTitle as $selectedTriggerTitle) 
{ 
//trigger title
	echo('<div class="timerframe" style="top:' . $top . 'px;">');
	echo('<span class="eventt">' .$selectedTriggerTitle['timer']. '</span>');
	echo('<span class="triggertime ' .$selectedTriggerTitle['timer_currentstatus'].  '"></span>');
	echo('</div>');
		
//events
	$top = $top+15;	
	foreach ($EventTitle as $selectedEventTitle) 
	{ 
		echo('<div class="eventframe" style="top:' . $top . 'px;">');
		echo('<span class="eventt">If ' .$selectedEventTitle['eventTitle']. '</span>');
		echo('<span class="eventicon ' .$selectedEventTitle['event_status']. '"></span>'); 
		echo('</div>');		
		$top = $top+15;	
		foreach ($Events as $selectedEvent) 
		{ 
			IF ($selectedEvent['id'] = $selectedEventTitle['id']) { 
				echo('<div class="eventlogicframe" style="top:' . $top . 'px;">');
				$str = $selectedEvent['sensor'] . ' | ' . $selectedEvent['node'] . ' ' . $selectedEvent['s_logic'] . ' '. $selectedEvent['bracket'] . '</span>';
				echo('<span class="eventt">' . $str);
				echo('<span class="sensortiny ' . $selectedEvent['sensorcurrentstatus'] . ' dh-' .$selectedEvent['sensor_type']. '"></span>');		
				echo('</div>');	
				$top = $top+12;	
				echo('<div class="actionframe" style="top:' . $top . 'px;right: -80px;">');
				$str = 'min:' . $selectedEvent['valmin'] . ' max:' . $selectedEvent['valmax'] . '</span>';			
				echo('<span class="eventt">' . $str);						
				echo('</div>');	
				$top =  $top + 12;
			}		
		}
	}
//action group		
		echo('<div class="triggerframe" style="top:' . $top . 'px;">');
	    echo('<span class="eventt">' .$selectedTriggerTitle['trigger_title']. '</span>');
		echo('<span class="triggertiny ' .$selectedTriggerTitle['currentstatus_icon']. ' dh-' .$selectedTriggerTitle['trigger_type']. '"></span>');
		echo('</div>');
		$top = $top+15;	
		foreach ($Actions as $selectedAction) 
			{ 
				echo('<div class="actionframe" style="top:' . $top . 'px;">');
				echo('<span class="eventt">' .$selectedAction['actuator']. ' | ' .$selectedAction['node_name']. '</span>');
				echo('<span class="actuatortiny ' .$selectedAction['actuator_currentstatus']. ' dh-' .$selectedAction['actuator_icon']. '"></span>');
				echo('</div>');
				$top =  $top + 13;
				echo('<div class="actiondetailframe" style="top:' . $top . 'px;">');
				echo('<span class="eventt">' .$selectedAction['actionon']. '</span>');
				echo('<span class="inrange"></span>');
				echo('</div>');
				$top =  $top + 13;
				echo('<div class="actiondetailframe" style="top:' . $top . 'px;">');
				echo('<span class="eventt">' .$selectedAction['actionoff']. '</span>');
				echo('<span class="outrange"></span>');
				echo('</div>');	
				$top =  $top + 15;	
			}
			
	}

}
Else 
{ 
	echo('<div class="timerframe" style="top:' . $top . 'px;">');
	echo('<span class="eventt">No event defined</span>');
	echo('</div>');
}

echo('</div>');	 
echo('</a>');
?> 
