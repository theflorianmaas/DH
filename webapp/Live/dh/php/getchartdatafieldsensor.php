<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

$sql = "SELECT tbsensor.descr as _HAEADER FROM tbsensor 
		WHERE tbsensor.id = " . $_SESSION["id"];
			
$result = mysqli_query($db, $sql);

echo ("data.addColumn('datetime', 'Date');");
while ($row = mysqli_fetch_array($result)) {
	echo ("data.addColumn('number', '" .$row['_HAEADER']. "');");
}

?> 




