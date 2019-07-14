<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

 // Connects to your Database 
 mysqli_query($db,"set names 'utf8'");
 
$query = "SELECT u.descr
		  FROM tbsensor s, tbsensortype p, tbum u
		  WHERE s.tbSensorType_id = p.id
		  AND p.tbum_id = u.id
		  AND s.id = " . $_SESSION["id"]; 
  
$result = mysqli_query($db,$query) or die("SQL Error 1: " . mysqli_error());
  // get data and store in a json array
while ($row = mysqli_fetch_array($result)) {
	echo ("title: '". $row['descr']. "',\n");
	}

?> 




