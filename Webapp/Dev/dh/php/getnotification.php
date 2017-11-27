<?php 

if(!isset($_SESSION)) 
{ session_start(); }
include_once(getcwd() . "/php/acl.php");
include_once(getcwd() . "/php/db.php");

 $data = mysqli_query($db, "SELECT pvalue FROM tbparam WHERE ptype = 'E'") 
 or die(mysqli_error()); 
  
 while($info = mysqli_fetch_array( $data )) 
 { 
	 
  IF ($info['pvalue'] == 0)	 
   { echo("On");	} 
  IF ($info['pvalue'] == 1)	 
   { echo("Off");	}  
   
 } 

?> 


 
 
