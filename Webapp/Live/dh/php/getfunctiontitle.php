<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

 mysqli_query($db,"set names 'utf8'");
 
 $data = mysqli_query($db,"SELECT descr as title FROM tbfunction WHERE id = " . $_GET["id"]) 
 or die(mysqli_error()); 
 
 while($info = mysqli_fetch_array( $data )) 
 { 
 echo($info['title']);	
 } 

?> 


 
 
