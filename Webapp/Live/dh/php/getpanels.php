<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("acl.php");
include_once("db.php");
 
 $data = mysqli_query($db,"SELECT id, wording, icon FROM vwcharts") 
 or die(mysqli_error()); 
  
//echo '<div data-role="panel" data-position="right" data-position-fixed="true" data-display="overlay" data-theme="a" id="rightpanel">';
echo '<h4>';
 while($info = mysqli_fetch_array( $data )) 
 { 
 echo ('<form method="POST" action="charts.php" id="f' . $info['id'] . '" name="f' . $info['id'] . '">');
 
//	echo '<a href="charts.php?id=' . $info['id'] . '" target="_self" onclick="play();">';
	echo '<div id="c' . $info['id'] . '" class="footericon">';
	echo '<span class="chart dh-' . $info['icon'] . '"></span>';
	echo '</div>';
	echo '<input type="hidden" name="id" value="'. $info['id'] .'">';
//	echo '</a>';
?>

	<script>
	$("#c<?php echo($info['id']); ?>").click(function() { // changed	
	play();
	$(this).removeClass("run-animation").addClass("run-animation");
	document.getElementById('f<?php echo($info['id']); ?>').submit();
	//return false;						
	});
	</script>	
	
<?php  	
 echo ('</form>');			
 
 } 
 echo "</h4>";
//echo "</div>";

 ?> 
 

	
