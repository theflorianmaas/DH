<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

$sql = "SELECT tbroom.descr as room FROM tbsensor, tbnode, tbroom 
		WHERE tbsensor.tbnode_id = tbnode.id
		AND tbnode.tbroom_id = tbroom.id
		AND tbsensor.tbchart_id = " . $_SESSION["id"] . " ORDER BY tbsensor.id";
	
$result = mysqli_query($db, $sql);

//echo the chart line names

if ($_SESSION['period'] == 'd')
{ echo ("data.addColumn('datetime', 'Date');");	}
else 
{ echo ("data.addColumn('date', 'Date');");	}
while ($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
	//$sql[] = array( 'sql' => $row['_HAEADER'] );
	//$sql =  $row['room'];
	echo ("data.addColumn('number', '" .$row['room']. "');");
	}

?> 




