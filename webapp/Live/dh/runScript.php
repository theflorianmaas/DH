<?php


// ?ip="192.168.1.75"&key="FdkbvH1mC3mxUFhb"&group=131073&light=0&command="listlight"&value=0
//

$ip 	 = $_GET["ip"];
$key 	 = $_GET["key"];
$group 	 = $_GET["group"];
$light 	 = $_GET["light"];
$command = $_GET["command"];
$value   = $_GET["value"];

$url = "python3 /var/dhproc/lightmanager.py ".$ip." -K ".$key." ".$group." ".$light." ".$command." ".$value;
 
exec($url, $output, $ret_code);
print("§"]);
print($output[1]);
print("#"]);
?>
