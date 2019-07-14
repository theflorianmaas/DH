<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("php/acl.php");
include_once("php/db.php");
  
// get the type of sensors 
$query = "SELECT max(tbsensortype_id) 
			FROM tbsensor 
		   WHERE tbChart_id = " . $_SESSION["id"];
$result = mysqli_query($db,$query) or die("SQL Error 1: " . mysqli_error());

while ($row = mysqli_fetch_array($result)) {
	$sensortypeid = $row['tbsensortype_id'];
	}  
  
$query = "SELECT floor(min(h.value))-2 as minval,
				 floor(max(h.value))+2 as maxval 
		  FROM tbhistval h, tbsensor s
		  WHERE h.tbSensor_id = s.id AND s.tbChart_id = " . $_SESSION["id"]  . " and h.date between DATE_SUB(NOW(), INTERVAL 1 DAY) AND NOW()";
			  
 $result = mysqli_query($db,$query) or die("SQL Error 1: " . mysqli_error());

while ($row = mysqli_fetch_array($result)) {
	if ($sensortypeid != 7 && $row['minval'] < 0 ) 
	{ //sensor type not temperature and less than 0 then minval = 0
		echo "minValue: '0',\n";
	}
	else 
	{
		echo "minValue: '" . $row['minval'] . "',\n";
	}
	
	echo "maxValue: '" . $row['maxval'] . "',\n";
	}
?> 




