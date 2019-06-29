<?php 
	$sitename = 'Casadinamica';
	$strArr = explode("/", $_SERVER["REQUEST_URI"]); 
	$home = $strArr[sizeof($strArr) - 3]; 
	$dir = $strArr[sizeof($strArr) - 2]; 
	$currentDir = 'http://' . $_SERVER['HTTP_HOST']. $home . '/' . $dir ;	
	$homeDir = 'http://' . $_SERVER['HTTP_HOST']. '/dh' ;
	
	// constants
	define("ACTIVE",     "1");
	define("INACTIVE",   "0");
	
	define("ONOFF",		 "0");
	
	// Output types
	define("DIGITAL",    "0");
	define("TONE",       "4");
	define("ANALOG",     "1");
	define("SERVO",      "2");
	define("HVAC",       "5");
	define("TV",         "6");
	define("RGB",        "7");
	define("IPCAM",      "8");
	define("THERMOSTAT", "9");
	define("TIME"      , "10");
	define("AWNING"    , "13");
	
	define("SONY",		 "1");
	define("SAMSUNG",	 "2");
	
	define("TMMODE",     "31");
	define("TMPROG",	 "32");
	define("TMFIRE",	 "33");
	define("TMADJUST",	 "34");
	
	define("TMAUTO",	 "0");
	define("TMAUTOPROG", "1");
	define("TMMANUAL",	 "2");
	define("TMHUB",	 	 "3");
	
	define("TMDAILY",	 "0");
	define("TMWDAYS", 	 "1");
	define("TMWEND",	 "2");
	
	define("AWNINGUP",	 "39");
	define("AWNINGDOWN", "40");
	define("AWNINGSTOP", "41");
	define("AWNINGSTATUS0", "42");
	define("AWNINGSTATUS1", "43");

	
	

?>	

<script>
function play() { 
	var click = new Audio('themes/tick.mp3');
	click.play();
	return true;
} 										
</script>

