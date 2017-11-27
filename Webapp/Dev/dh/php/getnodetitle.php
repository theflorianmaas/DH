<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

 $sql = "SELECT descr FROM tbnode WHERE id = " . $_GET["id"];
 $data = mysqli_query($db,$sql); 
 
 while($info = mysqli_fetch_array( $data )) 
 { 
 echo($info['descr']);	
 } 

?> 


 
 
