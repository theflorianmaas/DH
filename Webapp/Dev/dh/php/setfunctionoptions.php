<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

// Connects to your Database
//update limits

IF (isset($_POST["function"])) {

	$sql = "DELETE FROM tbfunctiontrigger WHERE tbfunction_id = " . $_POST["function_id"];
	echo($sql);
	runsql($db,$sql);

	foreach ($_POST["function"] as $row) {
		$sql = "INSERT INTO tbfunctiontrigger (tbfunction_id,tbtrigger_id) VALUES (" .$_POST["function_id"]. "," .$row . ")";
		runsql($db,$sql);
	}

}

header("Location: ../functiontrigger.php?id=". $_POST["function_id"]);  
 
?> 


