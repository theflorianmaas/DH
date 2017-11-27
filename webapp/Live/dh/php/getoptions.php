<?php 
if(!isset($_SESSION)) 
{ session_start(); }
include_once("php/acl.php");
include_once("php/db.php");

 // Connects to your Database 
 
 $dataTriggerTitle = mysqli_query($db,"SELECT * FROM vwtriggertitle a, vwtriggersall b where a.id = b.id and sensor_id = " . $_SESSION["id"])
 or die(mysqli_error()); 

$TriggerTitle = array(); 
 while($row = mysqli_fetch_assoc($dataTriggerTitle)) { 
    $TriggerTitle[] = $row; 
 }  
 
echo('<form data-ajax="false" action="php/setoptions.php" method="post">'); 
echo('<fieldset>'); 
echo('<input type="hidden" name="sensor_id" value="' . $_SESSION["id"] . '">');   
echo('<div class="mainframe">');
$top = -80;
$linefeed = 1;	

$res = mysqli_query($db,"SELECT id, addtohomepage FROM tbsensor WHERE id = " . $_SESSION["id"])   or die(mysqli_error()); 
echo('<div class="optionleftpanelframe" style="top:' . $top . 'px;">'); 
while($row_h = mysqli_fetch_assoc($res)) 
{
    echo('<fieldset data-role="controlgroup" data-iconpos="right">');
	echo('<label for="addhomepage">Add to homepage</label>');
	echo('<input type="checkbox" name="addhomepage" id="addhomepage" data-mini="true"');
	IF ($row_h["addtohomepage"] == 1)
	{  echo(' checked'); }
	echo('/>');	
	echo('</fieldset>');
}	
echo('</div>');	


// sensors to systems
$res_sy = mysqli_query($db,"SELECT subsystem_id FROM _vwsubsystems WHERE sensor_id = '" . $_SESSION["id"] . "'")   or die(mysqli_error()); 

$systems = array(); 
 while($row = mysqli_fetch_assoc($res_sy)) { 
    $systems[] = $row; 
 }  

echo('<div class="optionleftpanelframe" style="top:' . $top . 'px;">'); 
$res = mysqli_query($db,"SELECT sb.id, concat(s.descr,'-',sb.descr) as system FROM tbsystem s,tbsubsystem sb WHERE sb.tbsystem_id = s.id")   or die(mysqli_error()); 
echo ("<select multiple='multiple' data-native-menu='false' data-mini='true' name = 'system[]'>");	
echo ('<option value = ""></option>');
while($row_y = mysqli_fetch_assoc($res)) 
{
	echo ("<option value = '".$row_y['id']."'");
	
	foreach ($systems as $selectedSystem) {	
		if ($selectedSystem['subsystem_id'] == $row_y['id'])
			echo ("selected = 'selected'");	
	}	
		echo (">". $row_y['system']. "</option>");
}	
echo ("</select>");
echo('</div>');		

// sensors to chart
$res_ch = mysqli_query($db,"SELECT tbChart_id FROM tbsensor WHERE id = '" . $_SESSION["id"] . "'")   or die(mysqli_error()); 

$chart = array(); 
 while($row = mysqli_fetch_assoc($res_ch)) { 
    $chart[] = $row; 
 }  

echo('<div class="optionleftpanelframe" style="top:' . $top . 'px;">'); 
$res = mysqli_query($db,"SELECT * FROM _tbchart")   or die(mysqli_error()); 
echo ("<select data-native-menu='false' multiple='multiple' data-mini='true' name = 'chart'>");	
//echo ('<option value = "">    </option>');
while($row_z = mysqli_fetch_assoc($res)) 
{
	echo ("<option value = '".$row_z['id']."'");
	
	foreach ($chart as $selectedChart) {	
		if ($selectedChart['tbChart_id'] == $row_z['id'])
			echo ("selected = 'selected'");	
	}	
		echo (">". $row_z['wording']. "</option>");
}	
echo ("</select>");
echo('</div>');		

//trigger section

if (count($TriggerTitle) > 0) { 

foreach ($TriggerTitle as $selectedTriggerTitle) 
{ 
	echo('<input type="hidden" name="trigger_id" value="' . $selectedTriggerTitle["id"] . '">');  
//trigger status
	echo('<div class="optionleftpanelframe" style="top:' . $top . 'px;">'); 
	$res = mysqli_query($db,"SELECT id,wording FROM _tbstatus") or die(mysqli_error()); 
	//echo('<label for="status">Trigger status</label>');
	echo ("<select data-mini='true' name = 'status'>");	
	while($row_s = mysqli_fetch_assoc($res)) 
	{
		echo ("<option value = '".$row_s['id']."'");
		if ($selectedTriggerTitle['trigger_status'] == $row_s['id'])
			echo ("selected = 'selected'");
		echo (">". $row_s['wording']. "</option>");
	}	
	echo ("</select>");
	echo('</div>');		
	
	//echo('<span class="eventt">' .$selectedTriggerTitle['timer']. '</span>');
	//echo('<span class="triggertime ' .$selectedTriggerTitle['timer_currentstatus'].  '"></span>');
	//$top = $top-$linefeed;		
	echo('<div class="optionleftpanelframe" style="top:' . $top . 'px;">');
    echo('<fieldset data-role="controlgroup" data-iconpos="right">');
    echo(' <label for="sendalert">Send alert</label>');
    echo(' <input type="checkbox" name="sendalert" id="sendalert" data-mini="true"');
    IF ($selectedTriggerTitle["sendalert"] == 1)
		{  echo(' checked'); }
	echo('/>');	
    echo('</div>');
}
}
Else 
{ 
	echo('<div class="optionleftpanelframe" style="top:' . $top . 'px;">');
	echo('<span class="eventt">No event defined</span>');
	echo('</div>');
}
echo('</div>');	
$top =  300;
echo('<div class="optionleftpanelbuttonframe" style="top:' . $top . 'px;">');
echo('<input type="submit" data-mini="true" value="Update">');
echo('</div>');
 
echo('</fieldset>');
echo('</form>');
?>
