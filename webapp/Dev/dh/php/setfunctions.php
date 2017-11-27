<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");
// Connects to your Database
//update limits
$mode = 0;

	IF ($_POST["mode"] == 0)
		$mode = 1;
	
	$sql = "CALL setFunction(" .$_POST["function"]. "," .$mode.")";
	//echo($sql);
	runsql($db, $sql);

header("Location: ../function.php");  
 
?> 


