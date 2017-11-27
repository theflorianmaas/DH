<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("php/acl.php");
include_once("php/db.php");

 // Connects to your Database 

$dataPopup = mysqli_query($db,"SELECT a.id, b.cwhy, b.cwhat, b.cwhen FROM vwdashboardsensorshomepage a, tbpopup b WHERE a.id = b.tbSensor_id AND a.addtohomepage = 1") 
  or die(mysqli_error()); 
  

$Popups = array(); 
 while($row = mysqli_fetch_assoc($dataPopup)) { 
    $Popups[] = $row; 
} 

	foreach ($Popups as $selectedPopup) 
	{ 
	echo('<div data-role="popup" data-theme="b" data-overlay-theme="a" class="ui-content" data-transition="pop" data-position-to="origin" id="pp' .$selectedPopup['id']. '">');
 	echo('<ul data-role="listview" class="popup">');

		
			echo('<li class="popup">');
			echo('<span>' .$selectedPopup['cwhy']. ' '.$selectedPopup['cwhen']. ' '.$selectedPopup['cwhat'].'</span>');
			echo('</li>');
		

 	echo('</ul>');
 	echo('</div>');
 	}

?> 

