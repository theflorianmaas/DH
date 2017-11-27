<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

 // Connects to your Database 
 $data = mysqli_query($db,"SELECT * FROM vwdashboardactuators WHERE currentstatus = 1") 
 or die(mysqli_error()); 

require 'includegetactuators.php'; 

?> 
</ul>
<script type="text/javascript">
	$('#mainlist').listview().listview();
</script>
 