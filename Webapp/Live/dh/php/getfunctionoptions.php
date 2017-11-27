<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("php/acl.php");
include_once("php/db.php");

 // Connects to your Database 
echo('<div data-role="panel" data-position="left" data-position-fixed="true" data-display="overlay" data-theme="a" id="leftpanel">');
echo('<form data-ajax="false" action="php/setfunctionoptions.php" method="post">'); 
echo('<fieldset>'); 
echo('<input type="hidden" name="function_id" value="' . $_SESSION["id"] . '">');  
 
echo('<div class="mainframe">');
$top = -70;
$linefeed = 1;	

// triggers to functions
$res_fn = mysqli_query($db,"SELECT id FROM vwtriggerfunctiontitle WHERE function_id = '" . $_SESSION["id"] . "'")   or die(mysqli_error()); 

$functions = array(); 
 while($row = mysqli_fetch_assoc($res_fn)) { 
    $functions[] = $row; 
 }  

$res = mysqli_query($db,"SELECT trigger_id as id,concat(room,'-',action) as triggertitle FROM vwtriggers")   or die(mysqli_error());
echo('<div class="optionleftpanelframe" style="top:' . $top . 'px;">');  
echo ("<select multiple='multiple' data-native-menu='false' data-mini='true' name='function[]'>");	
while($row_y = mysqli_fetch_assoc($res)) 
{
	echo ("<option value = '".$row_y['id']."'");
	
	foreach ($functions as $selectedfunction) {	
		if ($selectedfunction['id'] == $row_y['id'])
			echo ("selected = 'selected'");	
	}	
		echo (">". $row_y['triggertitle']. "</option>");
}	
echo ("</select>");
echo('</div>');	
echo('</div>');		

$top =  300;
echo('<div class="optionleftpanelbuttonframe" style="top:' . $top . 'px;">');
echo('<input type="submit" data-mini="true" value="Update">');
echo('</div>');

echo('</fieldset>');
echo('</form>');
echo('</div>');
?>
