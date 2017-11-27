<?php 

if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/acl.php");
include_once(getcwd() . "/db.php");

runsql($db, "CALL regeneratemethodvalue()");
header("Location: ../settings.php");  
 
?> 


