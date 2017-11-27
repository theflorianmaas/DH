<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("php/acl.php");
include_once("php/db.php");

 // Connects to your Database 
 $dataSensors = mysqli_query($db,"SELECT * FROM vwdashboardsensorsdetails WHERE id = '" . $_SESSION["id"] . "'") 
 or die(mysqli_error()); 
 
 $dataActions = mysqli_query($db,"SELECT * FROM vwtriggersensordetails WHERE sensor_id = '" . $_SESSION["id"] . "' order by sequence") 
 or die(mysqli_error()); 
 
 $dataEvents = mysqli_query($db,"SELECT * FROM vwevents WHERE tbsensor_id = " . $_SESSION["id"] . " order by sequence")
 or die(mysqli_error()); 
 
 $dataEventTitle = mysqli_query($db,"SELECT distinct id, event_status, descr as eventTitle FROM vwevents where tbSensor_id = " . $_SESSION["id"] )
 or die(mysqli_error()); 
 
$dataTriggerTitle = mysqli_query($db,"SELECT * FROM vwtriggertitle a, vwtriggersall b where a.id = b.id and sensor_id = " . $_SESSION["id"])
 or die(mysqli_error()); 
 
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
 
echo('<form data-ajax="false" action="php/settriggers.php" method="post">'); 
echo('<fieldset>'); 
echo('<input type="hidden" name="sensor_id" value="' . $_SESSION["id"] . '">');   
echo('<div class="mainframe">');
echo('<div style="width:290px;margin-left:-110px;margin-top:-15px;">');
echo('<input type="submit" data-mini="true" value="Update" >');
echo('</div>');
$top = 10;
$linefeed = 1;	

if (count($TriggerTitle) > 0) { 

foreach ($TriggerTitle as $selectedTriggerTitle) 
{ 
//trigger title
	echo('<input type="hidden" name="trigger_id" value="' . $selectedTriggerTitle["id"] . '">');   
	echo('<div class="optiontimerframe" style="top:' . $top . 'px;">'); 
	$res = mysqli_query($db,"SELECT id,descr FROM tbtimer") or die(mysqli_error()); 
	echo ("<select data-native-menu='false' data-mini='true' name = 'p-timer'>");	
	while($row = mysqli_fetch_assoc($res)) 
	{
		echo ("<option value = '".$row['id']."'");
		if ($selectedTriggerTitle['timer'] == $row['descr'])
			echo ("selected = 'selected'");
		echo (">". $row['descr']. "</option>");
	}	
	echo ("</select>");
	
	//echo('<span class="eventt">' .$selectedTriggerTitle['timer']. '</span>');
	echo('<span class="triggertime2"></span>');
	echo('</div>');
		
//events
	$top = $top+$linefeed;	
	foreach ($EventTitle as $selectedEventTitle) 
	{ 
		echo('<input type="hidden" name="event_id" value="' . $selectedEventTitle["id"] . '">'); 
		echo('<div class="optiontimerframe" style="top:' . $top . 'px;">');
		//echo('<label for="eventTitle">Event:</label>');
		echo('<input type="text" name="p-eventtitle" value="' .$selectedEventTitle['eventTitle']. '" data-mini="true">');

		//echo('<span class="eventt">Se ' .$selectedEventTitle['eventTitle']. '</span>');
		echo('<span class="eventicon2"></span>'); 
		echo('</div>');		
		$top = $top+$linefeed;	
		foreach ($Events as $selectedEvent) 
		{ 
			IF ($selectedEvent['id'] = $selectedEventTitle['id']) { 
				$res_e = mysqli_query($db,"SELECT * FROM vwsensorslist WHERE id = " .$selectedEvent['tbSensor_id'] ) or die(mysqli_error()); 
				echo('<input type="hidden" name="eventlogic_id" value="' . $selectedEvent["eventlogic_id"] . '">');   
				echo('<div class="optionsensorframe" style="top:' . $top . 'px;">');			
				echo ("<select data-mini='true' name = 'p-eventlogic'>");	
				while($row_e = mysqli_fetch_assoc($res_e)) 
					{
					echo ("<option value = '".$row_e['id']."'");
					if ($selectedEvent['tbSensor_id'] == $row_e['id'])
					echo ("selected = 'selected'");
					echo (">". $row_e['node']. ' - ' .$row_e['sensor']. "</option>");
					}	
				echo ("</select>");
				
				//$str = $selectedEvent['sensor'] . ' | ' . $selectedEvent['node'] . ' ' . $selectedEvent['s_logic'] . ' '. $selectedEvent['bracket'] . '</span>';
				//echo('<span class="optiontimerframe" class="eventt">' . $str);
				echo('<span class="sensortiny2 dh-' .$selectedEvent['sensor_type']. '"></span>');	
				echo('</div>');	
				$top = $top+$linefeed;	
				echo('<div class="optionsensorframe" style="top:' . $top . 'px;">');
				echo('<div data-role="rangeslider" data-mini="false">');
	//<label for="min">Trigger:</label>
				echo('<input type="range" name="p-min" id="range-6a" min="' .$selectedEvent["scalemin"]. '" max="' .$selectedEvent["scalemax"]. '" value="'.$selectedEvent["valmin"]. '">');
	//<label for="max">Trigger:</label>
				echo('<input type="range" name="p-max" id="range-6b" min="' .$selectedEvent["scalemin"]. '" max="' .$selectedEvent["scalemax"]. '" value="'.$selectedEvent["valmax"]. '">');
				echo('</div>');	
				//$str = 'min:' . $selectedEvent['valmin'] . ' max:' . $selectedEvent['valmax'] . '</span>';			
				//echo('<span class="eventt">' . $str);						
				echo('</div>');	
				$top =  $top + $linefeed;
			}		
		}
	}
//action group		
		echo('<div class="optiontimerframe" style="top:' . $top . 'px;">');
		//echo('<label for="triggertitle">Trigger:</label>');
		echo('<input type="hidden" name="actiongroup_id" value="' . $selectedTriggerTitle["actiongroup_id"] . '">');   
		echo('<input type="text" name="p-triggertitle" id="triggertitle" value="' .$selectedTriggerTitle['triggerTitle']. '" data-mini="true">');
	    //echo('<span class="eventt">' .$selectedTriggerTitle['triggerTitle']. '</span>');
		echo('<span class="triggertiny2 dh-' .$selectedTriggerTitle['trigger_type']. '"></span>');
		echo('</div>');
		$top = $top+$linefeed;	
		$res_a = mysqli_query($db,"SELECT * FROM vwactuatorlist") or die(mysqli_error()); 
		$res_x = mysqli_query($db,"SELECT * FROM tbaction") or die(mysqli_error()); 
		$res_y = mysqli_query($db,"SELECT * FROM tbaction") or die(mysqli_error()); 
		foreach ($Actions as $selectedAction) 
			{ 
				echo('<input type="hidden" name="actionitem_id" value="' . $selectedAction["actionitem_id"] . '">');   
				echo('<div class="optionsensorframe" style="top:' . $top . 'px;">');
				echo ("<select data-mini='true' name='p-actuator'>");	
				while($row_a = mysqli_fetch_assoc($res_a)) 
					{
					echo ("<option value = '".$row_a['id']."'");					
					if ($selectedAction['actuator_id'] == $row_a['id'])
						echo ("selected = 'selected'");
					echo (">". $row_a['node']. ' - ' .$row_a['actuator']. "</option>");
					}	
				echo ("</select>");
				
				//echo('<span class="eventt">' .$selectedAction['actuator']. ' | ' .$selectedAction['node_name']. '</span>');
				echo('<span class="actuatortiny2 dh-' .$selectedAction['actuator_icon']. '"></span>');
				echo('</div>');
				$top =  $top + $linefeed;
				echo('<div class="optiontimerframeon" style="top:' . $top . 'px;">');
				echo ("<select data-native-menu='false' data-mini='true' name='p-actionon'>");	
				while($row_x = mysqli_fetch_assoc($res_y)) 
					{
					echo ("<option value = '".$row_x['id']."'");
					if ($selectedAction['actionon'] == $row_x['descr'])
					echo ("selected = 'selected'");
					echo (">". $row_x['descr'] . "</option>");
					}	
				echo ("</select>");
				//echo('<span class="eventt">' .$selectedAction['actionoff']. '</span>');
				echo('<span class="inrange2"></span>');
				echo('</div>');
				
				//$top =  $top + $linefeed;
				echo('<div class="optiontimerframeoff" style="top:' . $top . 'px;">');
				echo ("<select data-native-menu='false' data-mini='true' name='p-actionoff'>");	
				while($row_y = mysqli_fetch_assoc($res_x)) 
					{
					echo ("<option value = '".$row_y['id']."'");
					if ($selectedAction['actionoff'] == $row_y['descr'])
					echo ("selected = 'selected'");
					echo (">". $row_y['descr'] . "</option>");
					}	
				echo ("</select>");
				
				//echo('<span class="eventt">' .$selectedAction['actionoff']. '</span>');
				echo('<span class="outrange2"></span>');
				echo('</div>');	
				$top =  $top + $linefeed;	
			}
			
	}

}
Else 
{ 
	echo('<div class="optiontimerframe" style="top:' . $top . 'px;">');
	echo('<span class="eventt">No event defined</span>');
	echo('</div>');
}
echo('</div>');	
 
echo('</fieldset>');
echo('</form>');

/*
include("db.php");
// Connects to your Database 

$dataSensors = mysqli_query("SELECT * FROM vwdashboardsensorsdetails WHERE id = '" . $_SESSION["id"] . "'") 
or die(mysqli_error()); 

$dataActions = mysqli_query("SELECT * FROM vwtriggersensordetails WHERE sensor_id = '" . $_SESSION["id"] . "' order by sequence") 
or die(mysqli_error()); 

$dataEvents = mysqli_query("SELECT *, if(logic is not null,if(logic=0,'AND',if(logic=1,'OR',null)),null) as s_logic FROM vwevents WHERE id in (SELECT id FROM vwevents WHERE tbSensor_id = " . $_SESSION["id"] . ") order by sequence")
or die(mysqli_error()); 

$dataEventTitle = mysqli_query("SELECT distinct id, event_status, descr as eventTitle FROM dh.vwevents where tbSensor_id = " . $_SESSION["id"] )
or die(mysqli_error()); 

$dataTriggerTitle = mysqli_query("SELECT * FROM dh.vwtriggertitle where id in (select id from dh.vwtriggersall where sensor_id = '" . $_SESSION["id"] . "')")
or die(mysqli_error());  


$data = mysqli_query("SELECT * FROM dashboard_sensors_details where sensor_id = '" . $_SESSION["id"] . "'") ;
$data_h = mysqli_query("SELECT addhomepage FROM tbsensor where ID = '" . $_SESSION["id"] . "'"); 
$data_s = mysqli_query("SELECT tbStatus_id FROM tbsensor where ID = '" . $_SESSION["id"] . "'") ;

?> 
<form data-ajax="false" action="php/setOptions.php" method="post">
<fieldset>
<input type="hidden" name="sensor_id" value="<?php echo $_SESSION["id"]; ?> ">
<?php

while($info = mysqli_fetch_array($dataEvents)) 
{ 
	// set treshold (Trigger)
	?> 
	<input type="hidden" name="event_id" value=" <?php echo $info["id"] ?> ">
	<div data-role="rangeslider" data-mini="false">
	<label for="min">Trigger:</label>
	<input type="range" name="min" id="range-6a" min="0" max="100" value="<?php echo $info["valmin"] ?>">
	<label for="max">Trigger:</label>
	<input type="range" name="max" id="range-6b" min="0" max="100" value="<?php echo $info["valmax"] ?>">
	</div> 
}

	<div id="triggerStatus">	 
	<label for="triggerStatus">Trigger status</label>
	<input type="checkbox" name="triggerStatus" data-role="flipswitch" id="id2" data-mini="false" data-on-text="On" data-off-text="Off"
	<?php
	IF ($info["triggerStatus"] == 0)
	{  echo(' checked>'); 
	}
	?>
	</div> 

	<?php
	IF ($info["output"] == 1)
	{
		echo ('<div id="fadingTime">');
		echo ('<label for="fTime">Fading time:</label>');
		echo ('<input type="range" name="fTime" id="fTime" value="');
		echo ($info["timer_time"]);
		echo '" min="0" max="100" data-highlight="true" />';
		echo ('</div>');
	}
	?>

	<div id="sendemail">	 
	<label for="semail">Send email</label>
	<input type="checkbox" name="semail" data-role="flipswitch" id="id2" data-mini="false" data-on-text="Yes" data-off-text="No"
	<?php
	IF ($info["sendemail"] == 1)
	{  echo(' checked>'); }
	?>
	</div> 
	<?php
} 


while($info_h = mysqli_fetch_array($data_h)) 
{ 
	//set add to homepage
	?>
	<div id="addhomepage">
	<label for="homepage">Add to homepage</label>
	<input type="checkbox" name="homepage" data-role="flipswitch" id="id" data-mini="false" data-on-text="Yes" data-off-text="No"
	<?php
	IF ($info_h["addhomepage"] == 1)
	{ echo(' checked>'); }   
	?>
	</div>
	<?php
}

//set status
while($info_s = mysqli_fetch_array($data_s)) 
{ 
	?>
	<div id="status">	 
	<label for="sts">Sensor status</label>
	<input type="checkbox" name="sts" data-role="flipswitch" id="id2" data-mini="false" data-on-text="On" data-off-text="Off"
	<?php
	IF ($info_s["TB_STATUS_ID"] == 0)
	{ echo(' checked>'); }  
	?>
	</div>
	<?php
}  
?>
<input type="submit" value="Update">
</fieldset>
</form>

*/
?>
