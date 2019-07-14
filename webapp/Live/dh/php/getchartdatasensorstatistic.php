<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

$query = "SET NAMES 'utf8'"; 

if ($_GET['period'] == "month")	   
{
$sql = "SELECT DATE_FORMAT(tbhistvalstatistic.date,'%d/%m/%Y') as Data 
			 , tbhistvalstatistic.value AS '" .$_SESSION["id"]. "' 
		  FROM tbhistvalstatistic 
		 WHERE tbhistvalstatistic.tbsensor_id = " .$_SESSION["id"]. "
		   AND tbhistvalstatistic.date between DATE_SUB(NOW(), INTERVAL 1 MONTH) AND NOW() 		 
      ORDER BY Data";
}
elseif ($_GET['period'] == "year") 
{
$sql = "SELECT DATE_FORMAT(tbhistvalstatistic.date,'1/%m/%Y') as Data 
			 , AVG(tbhistvalstatistic.value) AS '" .$_SESSION["id"]. "' 
		  FROM tbhistvalstatistic 
		 WHERE tbhistvalstatistic.tbsensor_id = " .$_SESSION["id"]. " 
		   AND tbhistvalstatistic.date between DATE_SUB(NOW(), INTERVAL 1 YEAR) AND NOW() 	
      GROUP BY Data 
      ORDER BY Data";    
}

$result = mysqli_query($db,$sql) or die("SQL Error 1: " . mysqli_error());
$numfields = mysqli_num_fields($result);

while ($row = mysqli_fetch_array($result)) {
	$y = "";
	$i = 0;
	$y = "[";
	do {
		$x = mysqli_fetch_field_direct($result, $i);
		if ($i == 0) {
			$date = date_parse_from_format('d/m/Y',$row[$x->name]);
			$d = "new Date(".$date['year'].",".($date['month']-1).",".$date['day'].")"; //month 0 = jan, 11 = dec in google chart
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
 
?> 

