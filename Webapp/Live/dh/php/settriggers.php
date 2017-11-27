
<?php 
include_once("db.php");
include_once("acl.php");
//print_r($_POST);

foreach (array_keys($_POST) as $param)
{
$len = strlen($param);
$par = substr($param,0,$len);
$val = $_POST[$par];

	if ($par == 'p-timer')
	{
		$sql = "UPDATE tbtrigger set tbTimer_id = " .$_POST[$param]. " where id = '" . $_POST["trigger_id"] . "'"; 	
		runsql($db,$sql);
	}

	if ($par == 'p-triggertitle')
	{
		$sql = "UPDATE tbactiongroup set descr = '" .$_POST[$param]. "' where id = '" . $_POST["actiongroup_id"] . "'"; 
		runsql($db,$sql);
	}

	if ($par == 'p-eventtitle')
	{
		$sql = "UPDATE tbevent set descr = '" .$_POST[$param]. "' where id = " . $_POST["event_id"]; 
		runsql($db,$sql);
	}
	
	/*
	if ($par == 'p-eventlogic')
	{
		$sql = "UPDATE tbeventlogic set tbSensor_id = " .$_POST[$param]. " where id = '" . $_POST["eventlogic_id"] . "'"; 
		echo($sql);
		mysqli_query($sql);
	}
	*/
	
	if ($par == 'p-min')
	{
		$sql = "UPDATE tbeventlogic set valmin = " .$_POST[$param]. " where id = " . $_POST["eventlogic_id"]; 
		runsql($db,$sql);
	}
	
	if ($par == 'p-max')
	{
		$sql = "UPDATE tbeventlogic set valmax = " .$_POST[$param]. " where id = " . $_POST["eventlogic_id"]; 
		runsql($db,$sql);
	}
	
	if ($par == 'p-actuator')
	{
		$sql = "UPDATE tbactionitem SET tbActuators_id = " .$_POST[$param]. " WHERE id = " . $_POST["actionitem_id"]; 
		runsql($db,$sql);
	}
	
	if ($par == 'p-actionon')
	{
		$sql = "UPDATE tbactionitem set actiontrue = " .$_POST[$param]. " where id = '" . $_POST["actionitem_id"] . "'"; 
		runsql($db,$sql);
	}
	
	if ($par == 'p-actionoff')
	{
		$sql = "UPDATE tbactionitem set actionfalse = " .$_POST[$param]. " where id = '" . $_POST["actionitem_id"] . "'"; 
		runsql($db,$sql);
	}

}

header("Location: ../sensordetails.php?id=". $_POST["sensor_id"]);  

?> 


