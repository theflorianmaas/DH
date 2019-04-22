<?php
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");
 
//print_r($_POST); 

$cval = $_POST["v".$_POST["id"]];

IF ($cval == $_POST["valueoff"]) 
{	$csts = 0; }
ELSE 
{	$csts = 1; }

IF ($_POST["actuatormethod"] == 0 OR $_POST["actuatormethod"] == 1) {
	$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $cval . "','" . $_POST["output"] .  "','" . $_POST["temptime"] . "','" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	$sql = "UPDATE tbactuator SET currentvalue = " . $cval . ", currentstatus = " . $csts ." WHERE id = '" . $_POST["id"] . "'";
	runsql($db,$sql);
}

// if it is 0 set to 1 and others set to 0
// if it is already 1 do nothing
IF ($_POST["actuatormethod"] == 3) { //fan speed 1
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',4,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (4,5,6)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 4) { //fan speed 2
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',4,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (3,5,6)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 5) { //fan speed 3
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',4,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (3,4,6)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 6) { //fan speed auto
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',4,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (3,4,5)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 7) { //cool
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',3,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (8,9,10)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 8) { //dry
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',3,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (7,9,10)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 9) { //heat
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',3,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (7,8,10)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 10) { //auto
	IF 	($_POST["sts"] == 0){
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "',3,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 1 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (3,4,5,6,7,8,9)";
		runsql($db,$sql);
	}
}

IF ($_POST["actuatormethod"] == 11) { //temperature
	$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["input_temp"] . "','" . $_POST["output"] .  "',2,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	$sql = "UPDATE tbactuatormethodvalue SET value = " .$_POST["input_temp"]. " WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
	runsql($db,$sql);
}


IF ($_POST["actuatormethod"] == 12) { //swing
	IF 	($_POST["sts"] == 0) {
		$sts = 1;
	}
	IF ($_POST["sts"] == 1)	{
		$sts = 0;
	}	
	$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "',1,'" . $_POST["output"] .  "',2,'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = " . $sts . " WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
}


IF ( in_array($_POST["actuatormethod"], array(13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29)) ) { //if TV Methods 
	IF 	($_POST["brand"] == "Sony") {
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "'," .SONY. ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		IF ( in_array($_POST["actuatormethod"], array(15,16))) { //if channel +/- set the TV status On
			$sql = "UPDATE tbactuator SET currentvalue = 1, currentstatus = 1 WHERE id = '" . $_POST["id"] . "'";
			runsql($db,$sql);
		}
	}	
}

IF ( in_array($_POST["actuatormethod"], array(30)) ) { //rgb 
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "'," .hexdec($_POST["value"]). ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";

		runsql($db,$sql);
		$sql = "UPDATE tbactuator SET currentvalue = 1, currentvalue=" . hexdec($_POST["value"]) . ", currentstatus = 1 WHERE id = '" . $_POST["id"] . "'";
				echo($sql);
		runsql($db,$sql);
}

IF ( in_array($_POST["actuatormethod"], array(31)) ) { //thermostat mode 

		$value = 0;
		switch ($_POST["sts"]) {
		case TMAUTO:
			$value = TMAUTOPROG;
			break;
		case TMAUTOPROG:
			$value = TMMANUAL;
			break;			
		case TMMANUAL:
			$value = TMHUB;
			break;
		case TMHUB:
			$value = TMAUTO;
			break;
		}
	
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" .$value. "','" . $_POST["output"] .  "'," . $_POST["actuatormethod"] . ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = ".$value." WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		runsql($db,"CALL updThermostatMethods()");
		//$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (3,4,5,6,7,8,9)";
		//runsql($db,$sql);
}

IF ( in_array($_POST["actuatormethod"], array(32)) ) { //thermostat prog 

		$value = 0;
		switch ($_POST["sts"]) {
		case TMDAILY:
			$value = TMWDAYS;
			break;
		case TMWDAYS:
			$value = TMWEND;
			break;			
		case TMWEND:
			$value = TMDAILY;
			break;
		}

		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" .$value. "','" . $_POST["output"] .  "'," . $_POST["actuatormethod"] . ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = ".$value." WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
		//$sql = "UPDATE tbactuatormethodvalue SET value = 0 WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id IN (3,4,5,6,7,8,9)";
		//runsql($db,$sql);
}

IF ($_POST["actuatormethod"] == 33) { //fire
	IF 	($_POST["sts"] == 0) {
		$sts = 1;
	}
	IF ($_POST["sts"] == 1)	{
		$sts = 0;
	}	
	
	$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" .$sts. "','" . $_POST["output"] .  "'," . $_POST["actuatormethod"] . ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	$sql = "UPDATE tbactuatormethodvalue SET value = " . $sts . " WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
	runsql($db,$sql);
}

IF ($_POST["actuatormethod"] == 34) { //temperature
	$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" .$_POST["input_temp_t"]. "','" . $_POST["output"] .  "'," . $_POST["actuatormethod"] . ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	$sql = "UPDATE tbactuatormethodvalue SET value = " .$_POST["input_temp_t"]. " WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
	runsql($db,$sql);
	print($sql);
}

IF ($_POST["actuatormethod"] == 35 OR $_POST["actuatormethod"] == 37) { //smart light color temperature
	$sql = "INSERT INTO tbdataoutsmartlight (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),1,'" . $_POST["node"] . "','" . $_POST["id"]  . "','" .$_POST["actuatormethod"]. "','" . $_POST["output"] .  "'," .$_POST["sts"]. ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	$sql = "UPDATE tbactuatormethodvalue SET value = " .$_POST["sts"]. " WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
	runsql($db,$sql);
	print($sql);
}

IF ($_POST["actuatormethod"] == 36) { //Smartlight dimmer
	$sql = "INSERT INTO tbdataoutsmartlight (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),2,'" . $_POST["node"] . "','" . $_POST["id"]  . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "','" . $cval . "','" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
	runsql($db,$sql);
	$sql = "UPDATE tbactuatormethodvalue SET value = " .$cval. " WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatortype_id =" .$_POST["actuatortype"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
	runsql($db,$sql);
	$sql = "UPDATE tbactuator SET currentstatus = " . $csts ." WHERE id = '" . $_POST["id"] . "'";
	runsql($db,$sql);
}

/*
IF ( in_array($_POST["actuatormethod"], array(37)) ) { //Smartlight rgb color picker 
		$sql = "INSERT INTO tbdataoutsmartlight (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),3,'" . $_POST["node"] . "','" . $_POST["id"]  . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "'," .hexdec($_POST["value"]). ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";

		runsql($db,$sql);
		$sql = "UPDATE tbactuator SET currentvalue=" . hexdec($_POST["value"]) . ", currentstatus = 1 WHERE id = '" . $_POST["id"] . "'";
				echo($sql);
		runsql($db,$sql);
}
*/

IF ( in_array($_POST["actuatormethod"], array(38)) ) { //Smartlight on/off 
		$sql = "INSERT INTO tbdataoutsmartlight (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["id"]  . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "'," .$csts. ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";

		runsql($db,$sql);
		$sql = "UPDATE tbactuator SET currentstatus=" . $csts. " WHERE id = '" . $_POST["id"] . "'";
		echo($sql);
		runsql($db,$sql);
}

IF ( in_array($_POST["actuatormethod"], array(39,40)) ) { //awning
		$sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5,v6) values (millis(),0,'" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["actuatormethod"] . "','" . $_POST["output"] .  "'," .SONY. ",'" . $_POST["fadingtime"] . "','" . $_POST["id"] . "')";
		runsql($db,$sql);
		$sql = "UPDATE tbactuatormethodvalue SET value = " . $sts . " WHERE tbactuator_id = " . $_POST["id"] . " AND tbactuatormethod_id =" .$_POST["actuatormethod"];
		runsql($db,$sql);
}


?>



