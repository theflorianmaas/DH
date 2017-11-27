<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");
 
mysqli_query($db,"set names 'utf8'");
 
$data = mysqli_query($db,"SELECT CONCAT(n.descr,' - ', ss.descr) as title from tbsystem n, tbsubsystem ss WHERE ss.tbsystem_id = n.id and ss.id = " . $_SESSION["id"]) 
or die(mysqli_error()); 
  
while($info = mysqli_fetch_array( $data )) 
{ 
	echo($info['title']);	
} 

?> 


 
 
