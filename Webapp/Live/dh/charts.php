<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">

<meta http-equiv="content-type" content="text/html;charset=utf-8" />    

    <head>
		<?php  
			require_once('login/config/config.php');
			require_once('login/translations/en.php');
			require_once('login/classes/Login.php'); 
			$login = new Login();
			$pageId = "charts";
			include_once "php/config.php";
			include_once "php/script.php";
			include_once "php/acl.php";
			$_SESSION['pageId'] = $pageId;
			$_SESSION['currDir'] = $currentDir;
			$_SESSION['id']	= $_POST["id"];	
			//print_r($_SESSION);
	    ?>
	
		<!--Load the AJAX API-->
		<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
		<script type="text/javascript">
		google.charts.load('current', {'packages':['corechart']});
		google.charts.setOnLoadCallback(drawChartd);
		<?php $_SESSION['period'] = "d"; ?>
		function drawChartd() {
			var data = new google.visualization.DataTable();
			<?php include 'php/getchartdatafields.php'; ?>
			data.addRows([
			<?php include 'php/getchartdata.php'; ?> 
			]);

			var options = {	
			width: "100%",
			chartArea:{left:35,top:10,right:10,width:"90%",height:"80%"},			
			curveType: 'function',
			legend: { position: 'bottom' },
			vAxis: {
					format: '##',
					<?php include 'php/getunitmeasure.php'; ?>
					count: 6
			},			
			hAxis:  { gridlines: { count: 6 } },	
			hAxis:  { format: 'H' },	
			title: 'Day',	
			titlePosition: 'in', 
			lineWidth: 2
			};
		
			var chart = new google.visualization.LineChart(document.getElementById('chartd'));
			chart.draw(data, options);
		}

		google.charts.setOnLoadCallback(drawChartm);
		<?php $_SESSION['period'] = "m"; ?>
		function drawChartm() {
			var data = new google.visualization.DataTable();
			<?php include 'php/getchartdatafields.php'; ?>
			data.addRows([
			<?php include 'php/getchartdata.php'; ?> 
			]);

			var options = {	
			width: "100%",
			chartArea:{left:35,top:10,right:10,width:"90%",height:"80%"},			
			curveType: 'function',
			legend: { position: 'bottom' },
			vAxis: {
					format: '##',
					<?php include 'php/getunitmeasure.php'; ?>
					count: 6
			},			
			hAxis:  { gridlines: { count: 6 } },	
			hAxis:  { format: 'd/MM' },	
			title: 'Month',	
			titlePosition: 'in',
			lineWidth: 2
			};
		
			var chart = new google.visualization.LineChart(document.getElementById('chartm'));
			chart.draw(data, options);
			$("#chartm").hide();
		}

		google.charts.setOnLoadCallback(drawCharty);
		<?php $_SESSION['period'] = "y"; ?>
		function drawCharty() {
			var data = new google.visualization.DataTable();
			<?php include 'php/getchartdatafields.php'; ?>
			data.addRows([
			<?php include 'php/getchartdata.php'; ?> 
			]);

			var options = {	
			width: "100%",
			chartArea:{left:35,top:10,right:10,width:"90%",height:"80%"},			
			curveType: 'function',
			legend: { position: 'bottom' },
			vAxis: {
					format: '##',
					<?php include 'php/getunitmeasure.php'; ?>
					count: 6
			},			
			hAxis:  { gridlines: { count: 6 } },	
			hAxis:  { format: 'MMM' },	
			title: 'Year',	
			titlePosition: 'in',
			lineWidth: 2
			};
		
			var chart = new google.visualization.LineChart(document.getElementById('charty'));
			chart.draw(data, options);
			$("#charty").hide();
		}	
		</script>		
   	
	<?php 
		$strArr = explode("/", $_SERVER["REQUEST_URI"]); 
		$home = $strArr[sizeof($strArr) - 3]; 
		$dir = $strArr[sizeof($strArr) - 2]; 
		$currentDir = 'http://' . $_SERVER['HTTP_HOST']. '/' . $home . '/' . $dir ;
	?>		

	<script type="text/javascript">
		$(document).ready( function(){
			$('#rightpanel').load('<?php echo('php/getpanels.php'); ?>');
			
			$("#toggled").click(function(){
				$("#statsd").prop('disabled', true);
				$("#statsm").prop('disabled', false);
				$("#statsy").prop('disabled', false);
				$("#chartd").show();
				$("#chartm").hide();
				$("#charty").hide();
			});
	
			$("#togglem").click(function(){
				$("#statsd").prop('disabled', false);
				$("#statsm").prop('disabled', true);
				$("#statsy").prop('disabled', false);
				$("#chartd").hide();
				$("#chartm").show();
				$("#charty").hide();
			});
	
			$("#toggley").click(function(){
				$("#statsd").prop('disabled', false);
				$("#statsm").prop('disabled', false);
				$("#statsy").prop('disabled', true);
				$("#chartd").hide();
				$("#chartm").hide();
				$("#charty").show();
			});	
		});		
	</script>		

</head>
	
<body>
	<div data-role="page" id="main">	
		<?php include "php/getheader.php"; ?>	
		<div data-role="content">
			<div id="wrapper" class="wrapper"></div>	
			<div id="chartd" style="border: 0px solid ; position: absolute; left: 1px; top: 100px; width: 99%; height: 260px;"></div>	
			<div id="chartm" style="border: 0px solid ; position: absolute; left: 1px; top: 100px; width: 99%; height: 260px;"></div>	
			<div id="charty" style="border: 0px solid ; position: absolute; left: 1px; top: 100px; width: 99%; height: 260px;"></div>	
			<div class="statscd"><input type='button' id='toggled' value='24h'></div>
			<div class="statscm"><input type='button' id='togglem' value='1 Month'></div>
			<div class="statscy"><input type='button' id='toggley' value='1 Year'></div>
		</div>
		
		<div data-role="panel" data-position="right" data-position-fixed="false" data-display="overlay" data-theme="b" id="rightpanel">
		
		</div>
    
	</div>		
 </body>
</html>
