<?php 
include_once '../php/config.php';

// ... ask if we are logged in here:
if (isset($_SESSION['user_logged_in'])) 
{ 
	if ($_SESSION['user_logged_in'] == false) 
	{ 
		header("Location: " . $homeDir . "../login/index.php");  
	}
}
else
{ 
 header("Location: " . $homeDir . "../login/index.php");  
}
?> 