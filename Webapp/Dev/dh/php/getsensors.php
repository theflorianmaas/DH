<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

$sql = 'SELECT * FROM vwdashboardsensorshomepage where tbnode_id = ' . $_SESSION["id"];

 // Connects to your Database 
$dataSensors = mysqli_query($db, $sql)  
 or die(mysql_error()); 

$dataTriggers = mysqli_query($db,"SELECT * FROM vwtriggersall")  
 or die(mysql_error()); 
 
$sendEmails = mysqli_query($db,"SELECT node, tbtrigger_id FROM tbsendemail")  
 or die(mysql_error()); 

$Triggers = array(); 
while($row = mysqli_fetch_assoc($dataTriggers)) { 
    IF ($row['node'] == $_SESSION["id"]){
		$Triggers[] = $row; 
	}
} 

$resetEmails = array(); 
while($row = mysqli_fetch_assoc($sendEmails)) { 
		$resetEmails[] = $row; 
} 

require 'includegetsensors.php'; 
?> 


<script type="text/javascript">
       $('#mainlist').listview().listview('refresh');
</script>


