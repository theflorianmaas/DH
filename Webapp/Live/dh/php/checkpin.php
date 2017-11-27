<?php
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

 // Connects to your Database
$sql = "SELECT lockcode FROM tbactuator WHERE id = " . $_POST["id"];
$result = mysqli_query($db, $sql);

while ($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
	if ($row['lockcode'] == $_POST["code"]){
		echo ("1");
		}
	else {	
		echo ("0");
	}
}
?> 
