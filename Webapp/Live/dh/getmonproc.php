<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");
 // Connects to your Database 

$data = mysqli_query($db,"SELECT * FROM tbproctl where id > 100"); 

echo('<ul data-role="listview" class="jqm-list" id="mainlist" data-theme="b">');

while($info = mysqli_fetch_array($data)) 
{ 
 echo('<li style="margin-left:0px;">');
 echo('<div data-role="footer" data-theme="a" class="ui-grid-c"  data-position="fixed" style="margin: -7px -10px -10px 0;">');
  echo('<div class="ui-block-a" style="width:30%; height:50px; margin-left:-0px;">');
	echo ($info['proc']);
	echo ("<br>");
	if ($info['sts'] == 1) 
		echo ("Running");
	else
		echo ("NOT Running");
	echo ("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp");	
	echo ($info['lastupdate']);

 echo('</div>');
 
 
 echo('</li>');
 echo('</div>');
}
?>   
</ul>
<script type="text/javascript">
$('#mainlist').listview().listview('refresh');
</script>
 
