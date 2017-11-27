<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/acl.php");
include_once(getcwd() . "/db.php");

$status = 0; 
IF (trim($_POST['sts']) == 'On') 
{ $status = 1; }
ELSEIF (trim($_POST['sts']) == 'Off') 
{ $status = 0; }		

runsql($db, "UPDATE tbparam SET PVALUE = " . $status . " WHERE PTYPE = 'I'");
header("Location: ../settings.php");  
 
?> 


