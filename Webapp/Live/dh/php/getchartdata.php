<?php 
if(!isset($_SESSION)) 
{ session_start(); }

include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

$query = "SET NAMES 'utf8'"; 
runsql($db,$query);

switch ($_SESSION['period']) { 
case 'd': //day
	// check if the chart lines are smooth
	$sql = "SELECT smooth as _RES FROM tbchart WHERE id = " . $_SESSION["id"];
	$tchart = mysqli_query($db, $sql) or die("SQL Error 1: " . mysql_error($db));
	while ($row = mysqli_fetch_array($tchart, MYSQL_ASSOC)) {
		$smooth =  $row['_RES'];
	}
	$result = mysqli_query($db,"SELECT generatesqlchartseries(".$_SESSION["id"].",".$smooth.") AS _SQL") or die("SQL Error 2: " . mysql_error($db));
	break;
case 'm': //month
	$result = mysqli_query($db,"SELECT generatesqlchartseriesstatistic(".$_SESSION["id"].",'M') AS _SQL") or die("SQL Error 3: " . mysql_error($db));
	break;
case 'y': //year
	$result = mysqli_query($db,"SELECT generatesqlchartseriesstatistic(".$_SESSION["id"].",'Y') AS _SQL") or die("SQL Error 4: " . mysql_error($db));
	break;	
}

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
				switch ($_SESSION['period']) { 
				case 'd': //day
					$date = date_parse_from_format('Y/m/d H:i:s',$row[$x->name]);
					$d = "new Date(".$date['year'].",".($date['month']-1).",".$date['day'].",".$date['hour'].",".$date['minute'].", 00)";
					$y = $y . $d;
					break;
				case 'm': //month
					$date = date_parse_from_format('Y/m/d',$row[$x->name]);
					$d = "new Date(".$date['year'].",".($date['month']-1).",".$date['day'].")";
					$y = $y . $d;
				break;	
				case 'y': //year
					$date = date_parse_from_format('Y/m/d',$row[$x->name]);
					$d = "new Date(".$date['year'].",".($date['month']-1).",".$date['day'].")";
					$y = $y . $d;
					break;	
				}		
			}
			else	{
				if($row[$x->name] != NULL) {
					$y = $y . $row[$x->name];
				}
				else {
				$y = $y . 0;
				}
			}		
			$i++;
			if ($i < $numfields )
			{
				$y = $y .",";
			}	
		} while ($i < $numfields);
		$y = $y . "],";
		if ($y != "") echo $y . "\n";
	//$dati[] = array( $y );
	//$dati[] = array( $row['Data'] );
	}
}
?> 




