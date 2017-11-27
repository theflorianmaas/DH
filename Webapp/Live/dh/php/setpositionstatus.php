

<?php 

if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

$sql = "UPDATE tbcoordinates SET status = " . $_POST['status'] . " WHERE id = " . $_POST['id'] . " AND type = '" . $_POST['type'] . "'"; 
//echo $sql;

mysqli_query($db, $sql);
mysqli_commit($db);

?>
