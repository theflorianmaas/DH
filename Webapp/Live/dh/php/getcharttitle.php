<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

 mysqli_query($db,"set names 'utf8'");
 $data = mysqli_query($db,"SELECT wording FROM vwcharts WHERE id = " . $_SESSION["id"]) 
 or die(mysql_error()); 
 
 while($info = mysqli_fetch_array( $data )) 
 { 
 echo($info['wording']);	
 } 

?> 


 
 
