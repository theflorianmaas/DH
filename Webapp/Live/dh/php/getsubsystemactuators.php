<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");

 // Connects to your Database 
 $data = mysqli_query($db,"SELECT * FROM vwsubsystemactuators WHERE subsystem_id = '" . $_SESSION["id"] . "' order by 1") 
 or die(mysqli_error()); 
 
?> 
<ul data-role="listview" id="mainlist" data-theme="b">
<?php 
  
 $dataArr = array();
 
 while($info = mysqli_fetch_array($data)) 
 { 
  $id =  "'" . $info['id'] . "'"; 
   $dataArr[] = $info;
   echo('<li style="margin-left:-10px;">');
   	echo('<div data-theme="b" class="ui-grid-b"  data-position="fixed" style="margin: -7px -10px -10px 0;">');
		echo('<div  class="ui-block-a" style="width:10%; margin-left:-0px;">');
 			echo ('<span class="' .$info['actuatortypeicon']. ' ' . $info['currentstatusicon'] . '"></span>');
		echo ('</div>');	
		echo('<div  class="ui-block-b" style="width:50%; margin-left:-0px;">');
		echo ('<span class="actuatorname">' .$info['descr']. '</span>');
		echo ('</div>');
		echo('<div  class="ui-block-c" style="width:40%; margin-left:-0px;">');

			echo('<form action="php/setActuator.php" method="post" id="fa' . $info['id'] . '">');
				echo('<select name="a'. $info['id'] .'" id="a'. $info['id'] .'" data-role="slider" data-mini="true" data-theme="b">');
				IF ($info['currentstatus'] == 0) {
					echo('<option value="' .$info['valueon']. '">On</option>');
					echo('<option selected="" value="' .$info['valueoff']. '">Off</option>');
				}					
				IF ($info['currentstatus'] == 1) {
					echo('<option selected="" value="' .$info['valueon']. '">On</option>');
					echo('<option value="' .$info['valueoff']. '">Off</option>');
				}		
				echo('</select>');
				
			echo('<script>');
			echo("$('#a" . $info['id'] . "').change(function(){");
				echo("$.post('php/setActuator.php', $('#fa" .$info['id']. "').serialize())");
			echo ('});');
			echo('</script>'); 					
			
			echo ('<input type="hidden" name="id" value="' . $info['id'] . '">');
			echo ('<input type="hidden" name="pin" value="' . $info['pinnumber'] . '">');
			echo ('<input type="hidden" name="sts" value="' . $info['currentstatus'] . '">');
			echo ('<input type="hidden" name="node" value="' .  $info['tbNode_id'] . '">');
			echo ('<input type="hidden" name="output" value="' . $info['tbOutput_id'] . '">');
			echo ('<input type="hidden" name="valueon" value="' . $info['valueon'] . '">');
			echo ('<input type="hidden" name="valueoff" value="' . $info['valueoff'] . '">');

			echo('</form>');	

   		echo ('</div>');
	echo ('</div>');
 echo('</li>');
 }
?> 
</ul>
<script type="text/javascript">
	$('#mainlist').listview().listview();
</script>
