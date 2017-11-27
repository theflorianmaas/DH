<?php 

if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/acl.php");
include_once(getcwd() . "/db.php");
$sql = "DELETE FROM tbsendemail WHERE tbTrigger_id = " . $_GET['id'];
runsql($db, $sql);
//header("Location: ../sensor.php?id=" . $_GET['nid']); 
header('Location: ' . $_SERVER['HTTP_REFERER']); 
 
?> 


