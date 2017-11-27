<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

 // Connects to your Database 

$dataSubSystems = mysqli_query($db,"SELECT sb.id,sb.descr,icon FROM tbsubsystem sb,tbicon ic WHERE sb.tbicon_id = ic.id AND tbsystem_id = '" . $_SESSION["id"] . "'") 
 or die(mysqli_error()); 
 
echo('<ul data-role="listview" class="jqm-list" id="mainlist" data-theme="b">');

while($infoSubSystems = mysqli_fetch_array($dataSubSystems)) 
{ 
 echo('<li style="margin-left:0px;">');
 echo('<div data-role="footer" data-theme="a" class="ui-grid-c"  data-position="fixed" style="margin: -7px -10px -10px 0;">');
 
 echo('<div class="ui-block-a" style="width:30%; height:50px; margin-left:-0px;">');
 echo('<span class="menulist dh-' . $infoSubSystems['icon'] .  '"></span>');
 echo('</div>');
 
 echo('<a href="subsystemsensor.php?id=' . $infoSubSystems['id'] . '" target=_self>');
 echo('<div  class="ui-block-b" style="width:40%; text-align: right;">');
 echo('<span class="menuitem">' . $infoSubSystems['descr']. '</span>');
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
 
