<!DOCTYPE html>
<html lang="en">
<head>
 
<?php 
$title = "";
foreach($css_files as $file): ?>
    <link type="text/css" rel="stylesheet" href="<?php echo $file; ?>" />
<?php endforeach; ?>
<?php foreach($js_files as $file): ?>
    <script src="<?php echo $file; ?>"></script>
<?php endforeach; ?>


<style type='text/css'>
body
{
    font-family: Arial;
    font-size: 14px;
}
a {
    color: blue;
    text-decoration: none;
    font-size: 14px;
}
a:hover
{
    text-decoration: underline;
}
</style>

</head>
<body>

 <body>
		
<div>
	    <a href='<?php echo site_url('../../')?>'>-- Back -- </a> 
		
		<a href='<?php echo site_url('main/tblocation')?>'>Location</a> |
	    <a href='<?php echo site_url('main/tbroom')?>'>Room</a> |
        <a href='<?php echo site_url('main/tbnode')?>'>Node</a> |	
        <a href='<?php echo site_url('main/tbsensor')?>'>Sensor</a> | 
		<a href='<?php echo site_url('main/tbactuator')?>'>Actuator</a> | 
		<a href='<?php echo site_url('main/tbtrigger')?>'>Trigger</a> | 	
		<a href='<?php echo site_url('main/tbtimer')?>'>Timer</a> | 		
		<a href='<?php echo site_url('main/tbevent')?>'>Event</a> |  			
 
 Master tables-> 
 <select id="dynamic_select">
    <option value="" selected></option>
 	<option value="tbnodetype">Node type</option>
 	<option value="tbsensortype">Sensor type</option>
 	<option value="tbactuatortype">Actuator type</option>
 	<option value="tbactiongroup">Action Group</option>
 	<option value="tbactionitem">Action Item</option>
 	<option value="tbeventlogic">Event logic</option>
 	<option value="tbtimerdaily">Timer Daily</option>
 	<option value="tbtimerweekly">Timer Weekly</option>
 	<option value="tbtimermonthly">Timer Monthly</option>		
    <option value="tbaction">Action</option>
	<option value="tbmenu">Initial Menu</option>
	<option value="tbfunctions">Functions</option>
	<option value="tbfunctiontriggers">Function Triggers</option>
	<option value="tbsystems">Systems</option>
	<option value="tbsubsystems">Sub Systems</option>
	<option value="tbsubsystemsensors">Sub System Sensors</option>
	<option value="tbsubsystemactuators">Sub System Actuators</option>
	<option value="tbums">Unit measures</option>
	<option value="tbtriggertype">Trigger type</option>
	
	tbsubsystems
    <option value="users">Users</option>
</select>
 Settings->
<select id="dynamic_select2">
    <option value="" selected></option>
 	<option value="tbconvformula">Conversion formula</option>
 	<option value="tbparam">Parameters</option>
 	<option value="tblanguage">Language</option>
 	<option value="tbtranslation">Translation</option>
 	<option value="tbtranslationitem">Translation item</option>
    <option value="tboutput">Output</option>
    <option value="tbstatus">Status</option>
    <option value="tbicon">Icons</option>
    <option value="tbcolor">Colors</option>
    <option value="tbchart">Chart</option>
	<option value="tbcustomparameters">Custom parameters</option>
	<option value="tbproctl">Proctl</option>
	<option value="tbcoordinates">Positions</option>
</select>

<script>
    $(function(){
      // bind change event to select
      $('#dynamic_select').on('change', function () {
          var url = $(this).val(); // get selected value
          if (url) { // require a URL
              window.location = url; // redirect
          }
          return false;
      });
      $('#dynamic_select2').on('change', function () {
          var url = $(this).val(); // get selected value
          if (url) { // require a URL
              window.location = url; // redirect
          }
          return false;
      });
    });
</script>
 
 
</div>
<!-- End of header-->
		

<!-- Beginning header -->
     <div>
       <h3><?php echo $title;?></h3>
       <?php echo $output; ?>
    </div>
	
<!-- Beginning footer -->
<div></div>
<!-- End of Footer -->
</body>
</html>
 
