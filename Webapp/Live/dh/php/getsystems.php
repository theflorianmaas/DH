<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

 // Connects to your Database 

$dataSensors = mysqli_query($db,"SELECT * FROM _vwsystems")  
 or die(mysqli_error()); 
 
echo('<ul data-role="listview" class="jqm-list" id="mainlist" data-theme="b">');

while($infoSensors = mysqli_fetch_array($dataSensors)) 
{ 
 echo('<li style="margin-left:0px;">');
 echo('<div data-role="footer" data-theme="a" class="ui-grid-c"  data-position="fixed" style="margin: -7px -10px -10px 0;">');
 
 echo('<div class="ui-block-a" style="width:30%; height:50px; margin-left:-0px;">');
 echo('<span class="menulist dh-' . $infoSensors['icon'] .  '"></span>');
 echo('</div>');
 
 echo('<a href="subsystem.php?id=' . $infoSensors['id'] . '" target=_self>');
 echo('<div  class="ui-block-b" style="width:40%; text-align: right;">');
 echo('<span class="menuitem">' . $infoSensors['system']. '</span>');
 echo('</div>');
 echo('</a>');
 echo('</li>');
  echo('</div>');
}
?>   
</ul>
<script type="text/javascript">
$('#mainlist').listview().listview('refresh');
</script>
 
