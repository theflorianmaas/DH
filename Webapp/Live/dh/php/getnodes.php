<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

 // Connects to your Database 
$data = mysqli_query($db,"SELECT * FROM vwnodesdashboard") 
 or die(mysqli_error()); 
 
?> 
<ul data-role="listview" class="jqm-list" id="mainlist" data-theme="b">
<?php 

 while($infoNodes = mysqli_fetch_array($data)) 
 { 
?> 
 
<li style="margin-left:-10px;">
<div data-role="footer" data-theme="a" class="ui-grid-c"  data-position="fixed" style="margin: -6px -10px -10px 0;">
<?php 
 echo('<a href="sensor.php?id=' . $infoNodes['id'] . '" target=_self>');
 echo('<div  class="ui-block-a" style="width:5%; height:70px; margin-left:-0px;">');
 echo('<span class="node ' . $infoNodes['currentstatus']. ' dh-' . $infoNodes['tipo'] .  '"></span>');	
 echo('<span class="room dh-' . $infoNodes['room'] .  '"></span>');	
 echo('<span class="nodename">' .  $infoNodes['descr'] . '</span>');
 echo('</div>');
 
 echo('<div  class="ui-block-b" style="width:30%; text-align: right;">');
 echo('<span class="value">' . $infoNodes['signal']. '</span>');
 echo('</div>');

 echo('<div  class="ui-block-c" style="width:25%">');
 
 
 echo('<div class="canvas">');
 echo('<canvas id="c' . $infoNodes['id'] . '" ></canvas>'); 
 echo('<script>');
 echo("sensor('c" . $infoNodes['id'] . "'," . $infoNodes['signal'] . ",0,100,'" . $infoNodes['color'] . "','" . "');");
 echo('</script>');
 echo('</div>');
 echo('</div>');

echo('<div class="ui-block-d" style="text-align: left;">');
 
 $dataA = mysqli_query($db,"SELECT * FROM vwdashboardactuators WHERE tbNode_id = " . $infoNodes['id'] . " LIMIT 8")
 //$dataA = mysql_query("SELECT 'active' as currentstatusicon, icon as actuatortypeicon FROM tbicon LIMIT 8")
 or die(mysqli_error()); 
 $cnt = 1;
 while($infoActuators = mysqli_fetch_array($dataA)) 
 { 
	echo('<span class="actu' . $cnt  . ' ' . $infoActuators['currentstatusicon'] . ' dh-'  . $infoActuators['actuatortypeicon'] . '"></span>');
 	//echo('<span class="actu dh-'  . $infoActuators['actuatortypeicon'] . ' ' . $infoActuators['currentstatusicon'] . '" margin-left="'. $cnt*30 .'px"  top="' . $y . 'px"></span>');
 	//echo('<span class="actu' . $cnt . ' ' . $infoActuators['actuatortypeicon'] . ' ' . $infoActuators['currentstatusicon'] . '"></span>');
 	$cnt = $cnt + 1;
 }
   
?> 
</div>
</a>
</li>

<?php 
  
}
  
?> 
</ul>
<script type="text/javascript">
       $('#mainlist').listview().listview('refresh');
</script>

