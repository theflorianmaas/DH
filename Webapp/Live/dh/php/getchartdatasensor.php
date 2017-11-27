<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

$query = "SET NAMES 'utf8'"; 

// check if the chart lines are smooth
$sql = "SELECT tbsensortype.smooth as _RES 
		  FROM tbsensor, tbsensortype 
         WHERE tbsensor.tbsensortype_id = tbsensortype.id
           AND tbsensor.id = " . $_SESSION["id"];
		   
$tchart = mysqli_query($db,$sql);
while ($row = mysqli_fetch_array($tchart, MYSQL_ASSOC)) {
	$smooth =  $row['_RES'];
	//echo($smooth);
	}

$result = mysqli_query($db,"SELECT generatesqlchartseriesSensor(".$_SESSION["id"].",".$smooth.") AS _SQL") or die("SQL Error 1: " . mysql_error());

while ($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
	$sql =  $row['_SQL'];
	}

if ($sql == "0") {	//tbhistval is empty, nothing to execute
	echo("");
}
else { //something to execute
	$result = mysqli_query($db,$sql) or die("SQL Error 1: " . mysqli_error());
	$numfields = mysqli_num_fields($result);

	while ($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
		$y = "";
		$i = 0;
		$y = "[";
		do {
			$x = mysqli_fetch_field_direct($result, $i);
			if ($i == 0) {
				$date = date_parse_from_format('Y/m/d H:i:s',$row[$x->name]);
				$d = "new Date(".$date['year'].",".$date['month'].",".$date['day'].",".$date['hour'].",".$date['minute'].", 00)";
				$y = $y . $d;
			}
			else	{	
				$y = $y . $row[$x->name];
			}		
			$i++;
			if ($i < $numfields )
			{
				$y = $y .",";
			}	
		} while ($i < $numfields);
		$y = $y . "],";
		if ($y != "") echo $y . "\n";
	}
}
?> 

