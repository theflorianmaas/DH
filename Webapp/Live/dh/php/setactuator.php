<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

//print_r($_POST); 
$sts = $_POST["sts"];
$cval = $_POST["v".$_POST["id"]]; //v=prefix for analog and servo input (slider)

//echo('valore: ');
//echo($cval);

IF ($_POST["output"] == DIGITAL OR $_POST["output"] == TONE) {	//digital, tone

	IF ($sts == $_POST["valueoff"]) 
	{	
		$value = $_POST["valueon"]; 
		//$csts = 1;
	}
	ELSEIF ($sts == $_POST["valueon"]) 
	{	
		$value = $_POST["valueoff"]; 
		//$csts = 0;
	}
	$sql = "INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5,V6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $value . "','" . $_POST["output"] .  "','" . $_POST["temptime"] . "','" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
}	
ELSEIF ($_POST["output"] == ANALOG OR $_POST["output"] == SERVO) {	//analog, servo

	IF ($cval == $_POST["valueoff"]) 
	{	
		//$value = $cval; 
		$csts = INACTIVE;
	}
	ELSE
	{	
		//$value = $cval; 
		$csts = 1;
	}
	$sql = "INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5,V6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $cval . "','" . $_POST["output"] .  "','" . $_POST["temptime"] . "','" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	$sql = "UPDATE tbactuator SET currentvalue = '" . $cval . "' WHERE id = '" . $_POST["id"] . "'";
	runsql($db,$sql);
}
ELSEIF ($_POST["output"] == HVAC OR $_POST["output"] == TV OR $_POST["output"] == THERMOSTAT) { // air condition or TV or thermostat

	IF ($_POST["actuatormethod"] == ONOFF)
	{
		IF ($sts == $_POST["valueoff"]) 
		{	
			$value = $_POST["valueon"]; 
			$csts = ACTIVE;
		}
		ELSEIF ($sts == $_POST["valueon"]) 
		{	
			$value = $_POST["valueoff"]; 
			$csts = INACTIVE;
		}
	}
	ELSE
	{
		$value = $sts; 
	}
	
	IF ($_POST["output"] != TV){
		$sql = "INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5,V6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $value . "','" . $_POST["output"] .  "','" . $_POST["actuatormethod"] . "','" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";	
		runsql($db,$sql);
	}
	IF ($_POST["output"] == HVAC) { //HVAC 
		$sql = "CALL updACmethods(".$value.",".$_POST["id"].",".$_POST["actuatortype"].")";
		runsql($db,$sql);
	}
	
	IF ($_POST["output"] == THERMOSTAT) { //THERMOSTAT 
		$sql = "CALL updMethodValue(".$value.",".$_POST["id"].",".$_POST["actuatortype"].")";
		runsql($db,$sql);
		$sql = "UPDATE tbactuator SET currentstatus = '" . $value . "' WHERE id = '" . $_POST["id"] . "'";
		runsql($db,$sql);
	}
	ELSE {
		$sql = "INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5,V6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $value . "','" . $_POST["output"] .  "','" . SONY . "','" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";	
		runsql($db,$sql);
	}	
}
ELSEIF ($_POST["output"] == RGB) { // RGB Strip

	IF ($sts == $_POST["valueoff"]) 
	{	
		$sql = "SELECT currentvalue FROM tbactuator WHERE id = " . $_POST["id"] . " LIMIT 1";
		$sql_currval = mysqli_query($db,$sql) or die(mysqli_error()); 
		// retrieve current color
		//turn on with corrent color
		$row = mysqli_fetch_row($sql_currval);
		$value = $row[0];
		$sqlupd = "UPDATE tbactuator SET currentstatus = " .$_POST["valueon"]. " WHERE id = " . $_POST["id"];		
		//$csts = 1;
	}
	ELSEIF ($sts == $_POST["valueon"]) 
	{	
		//turn off
		$value = 0; 
		$sqlupd = "UPDATE tbactuator SET currentstatus = " .$_POST["valueoff"]. " WHERE id = " . $_POST["id"];	
		//$csts = 0;
	}
	$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "'," .$value. ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	//$sql = "INSERT INTO tbdataout (timekey,type,V0,V1,V2,V3,V4,V5,V6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $value . "','" . $_POST["output"] .  "','" . $_POST["temptime"] . "','" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	runsql($db,$sqlupd);
}

IF ($_POST["actuatortype"] == 16 OR $_POST["actuatortype"] == 18) { //Air Condition or TV
 $sql = "UPDATE tbactuator SET currentstatus = '" . $csts . "' WHERE id = '" . $_POST["id"] . "'";
 runsql($db,$sql);
}
?>



