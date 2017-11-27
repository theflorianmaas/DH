<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");
 // Connects to your Database 

$data = mysqli_query($db,"SELECT * FROM _vwfunctions"); 

echo('<ul data-role="listview" class="jqm-list" id="mainlist" data-theme="b">');

while($info = mysqli_fetch_array($data)) 
{ 
 echo('<li style="margin-left:0px;">');
 echo('<div data-role="footer" data-theme="a" class="ui-grid-c"  data-position="fixed" style="margin: -7px -10px -10px 0;">');
 
 echo('<div class="ui-block-a" style="width:30%; height:50px; margin-left:-0px;">');

 echo('<form data-ajax="false" id="myform' .$info['id']. '" method="post" action="php/setfunctions.php">');
 echo('<fieldset>');
 echo('<input type="hidden" name="mode" value="' .$info['currentstatus']. '">');
 echo('<input type="hidden" name="function" value="' .$info['id']. '">');
 echo('<span id="action' .$info['id']. '" onclick="myform' .$info['id']. '.submit()" class="menulist ' .$info['currentstatusicon']. ' dh-'  . $info['icon'] .  '"></span>');
 echo('</fieldset>');
 echo('</form>');
 
 echo('</div>');
 
 echo('<a href="functiontrigger.php?id=' . $info['id'] . '" target=_self>');
	echo('<div  class="ui-block-b" style="width:40%; text-align: right;">');
 	echo('<span class="menuitem">' . $info['function']. '</span>');
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
 
