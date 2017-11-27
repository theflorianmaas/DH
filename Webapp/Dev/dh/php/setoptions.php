<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

// Connects to your Database
//update limits

IF (isset($_POST["system"])) {

	$sql = "DELETE FROM tbsubsystemsensor WHERE tbsensor_id = " . $_POST["sensor_id"];
	runsql($db,$sql);

	foreach ($_POST["system"] as $row) {
		$sql = "INSERT INTO tbsubsystemsensor VALUES (" .$row . "," .$_POST["sensor_id"]. ",1)";
		runsql($db,$sql);
	}

}


IF (isset($_POST["chart"])) {

	$sql = "UPDATE tbsensor SET tbChart_id = " . $_POST["chart"]. " WHERE id = " . $_POST["sensor_id"];
	//echo($sql);
	runsql($db,$sql);

}

IF (isset($_POST["trigger_id"])) {

	IF ($_POST["sendalert"] == true) 
	{ $sendalert = 1; }
	else
	{ $sendalert = 0; }

	$sql = "UPDATE tbtrigger set sendalert = " . $sendalert . " where id = " . $_POST["trigger_id"];
	runsql($db,$sql);

	$sql = "UPDATE tbtrigger set tbStatus_id = " . $_POST["status"] . " where id = " . $_POST["trigger_id"];
	runsql($db,$sql);
}

//update homepage
IF (isset($_POST["addhomepage"])) 
{ $addhomepage = 1; }
else
{ $addhomepage = 0; }

$sql = "UPDATE tbsensor set addtohomepage = " .$addhomepage. " where id = '" . $_POST["sensor_id"] . "'"; 
//echo($sql);

runsql($db,$sql);

header("Location: ../sensordetails.php?id=". $_POST["sensor_id"]);  
 
?> 


