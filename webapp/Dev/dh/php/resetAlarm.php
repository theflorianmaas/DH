
<?php 
if(!isset($_SESSION)) 
{ session_start(); } 
// ... ask if we are logged in here:
if (isset($_SESSION['user_logged_in']) and $_SESSION['user_logged_in'] == false) 
	{ header("Location: ../login/index.php"); }
include("db.php");

 // Connects to your Database
$sql = "INSERT INTO tb_dati_out (TYPE,V0,V1,V2,V3,V4) values ('3','" . $_POST["node"] . "','" . $_POST["pin"] . "','" . $_POST["alarm"] . "','" . $_POST["actuator"] . "','-999')";

runsql($db,$sql);

header("Location: ../sensordetails.php?id=". $_POST["sensor"]);  
 
?> 


