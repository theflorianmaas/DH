<?php 
 	// Connects to your Database 
 	//mysqli_connect("localhost", "root", "topona", "dh") or die(mysql_error()); 
 	//mysqli_select_db("dh") or die(mysql_error()); 
 	//mysqli_set_charset('utf8');
 	
$db = mysqli_init();
if (!$db) {
    die('mysqli_init failed');
}

if (!mysqli_options($db, MYSQLI_INIT_COMMAND, 'SET AUTOCOMMIT = 0')) {
    die('Setting MYSQLI_INIT_COMMAND failed');
}

if (!mysqli_options($db, MYSQLI_OPT_CONNECT_TIMEOUT, 5)) {
    die('Setting MYSQLI_OPT_CONNECT_TIMEOUT failed');
}

if (!mysqli_real_connect($db
						, '192.168.178.33'
						, 'dhuser'
						, 'dkl&%8&=ì23ì01918hsts2gs7825af12zzq5!}ãªÉ'
						, 'dhx')) {
    die('Connect Error (' . mysqli_connect_errno() . ') '
            . mysqli_connect_error());
}
else
{
$db->query("SET NAMES 'utf8'");
}

function runsql($conn,$sqli){
    	mysqli_query($conn,"START TRANSACTION");
		mysqli_query($conn,$sqli);
		mysqli_query($conn,"COMMIT");   
    }

//echo 'Success... ' . mysqli_get_host_info($link) . "\n";

//mysqli_close($link);

?>
