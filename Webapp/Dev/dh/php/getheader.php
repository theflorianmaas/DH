<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("config.php");
include_once("acl.php");
include_once("db.php");

IF ($_SESSION["pageId"] == "homepage")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar">');
 echo ('<ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">'.$sitename.'</div></div>');
 } 
 
 IF ($_SESSION["pageId"] == "credits")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar">');
 echo ('<ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">Credits</div></div>');
 } 
 
IF ($_SESSION["pageId"] == "favorites")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">'.$sitename.'</div></div>');
 }  
 
IF ($_SESSION["pageId"] == "system")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">Systems</div></div>');
 } 
 
 IF ($_SESSION["pageId"] == "subsystem")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 include 'php/getsystemtitle.php';
 echo ('</div></div>');
 } 
 
 IF ($_SESSION["pageId"] == "subsystemsensor")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 include 'php/getsubsystemtitle.php';
 echo ('</div></div>');
 }   
 
IF ($_SESSION["pageId"] == "function")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">'.$sitename.'</div></div>');
 }   
 
 IF ($_SESSION["pageId"] == "functiontrigger")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 include 'php/getfunctiontitle.php';
 echo ('</div></div>');
 }  
  
IF ($_SESSION["pageId"] == "node")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar">');
 echo ('<ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="back" class="ui-btn headericon dh-103" href="javascript:history.back()" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">Nodes</div></div>');
 } 

IF ($_SESSION["pageId"] == "sensor")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 include 'getnodetitle.php';
 echo ('</div></div>');
 }
 
IF ($_SESSION["pageId"] == "sensordetails")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 if (isset($_SESSION["nid"])) {
 echo ('<li><a id="sensors" class="ui-btn headericon dh-103" href=' . $_SESSION["currDir"] . "/sensor.php?id=" .$_SESSION["nid"].  ' target="_self" ></a></li>');
 }  
 else if (isset($_SESSION["ssid"])) {
 echo ('<li><a id="sensors" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/subsystemsensor.php?id=" .$_SESSION["ssid"].  ' target="_self" ></a></li>');
 }  
 else  { 
 echo ('<li><a id="back" class="ui-btn headericon dh-103" href="javascript:history.back()" ></a></li>');
 }
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 include 'php/getsensortitle.php';
 echo ('</div></div>');
 }
 
 IF ($_SESSION["pageId"] == "trigger")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 if (isset($_SESSION["nid"])) {
 echo ('<li><a id="sensors" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/sensor.php?id=" .$_SESSION["nid"].  ' target="_self" ></a></li>');
 }  else  { 
 echo ('<li><a id="back" class="ui-btn headericon dh-103" href="javascript:history.back()" ></a></li>');
 }
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 include 'php/getsensortitle.php';
 echo ('</div></div>');
 }
 

 
 IF ($_SESSION["pageId"] == "charts")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
 echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="nodes" class="ui-btn headericon dh-165" href=' . $_SESSION["currDir"] . "/node.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 include 'php/getcharttitle.php';
 echo ('</div></div>');
 }
 

 IF ($_SESSION["pageId"] == "settings")
 {
 echo ('<div data-role="header" data-position="fixed" data-fullscreen="false"  id="headersensors" data-theme="a">');
 echo ('<div data-role="navbar" id="navbar"><ul>');
echo ('<li><a id="home" class="ui-btn headericon dh-12c" href=' . $_SESSION["currDir"] . "/homepage.php" . ' target="_self" ></a></li>');
 echo ('<li><a id="back" class="ui-btn headericon dh-103" href="javascript:history.back()" ></a></li>');
 echo ('<li><a id="leftb" class="ui-btn headericon dh-12b" href="#leftpanel" ></a></li>');
 echo ('<li><a id="rightb" class="ui-btn headericon dh-10d" href="#rightpanel" ></a></li>');
 echo ('<li><a id="settings" class="ui-btn headericon dh-128" href=' . $_SESSION["currDir"] . "/settings.php" . ' target="_self" ></a></li>');
 echo ('</ul></div><div class="subtitle">');
 echo ('Settings');
 echo ('</div></div>');
 }

?> 

<script>
$("#menu").bind("vclick",function(){
   $("ul").toggle(); 
});
</script>
