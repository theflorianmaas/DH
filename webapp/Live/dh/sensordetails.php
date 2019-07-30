<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="content-type" content="text/html;charset=utf-8" />     
    <head>
		<?php  
		    session_start();
			require_once('login/config/config.php');
			require_once('login/translations/en.php');
			require_once('login/classes/Login.php'); 
			$login = new Login();
			$pageId = "sensordetails";
			include_once "php/config.php";
			include_once "php/script.php";
			include_once "php/acl.php";
			$_SESSION['pageId'] = $pageId;
			$_SESSION['currDir'] = $currentDir;
			$_SESSION['id'] = $_GET["id"];	
			if(isset($_GET["nid"]))	
				$_SESSION['nid'] = $_GET["nid"];					
	    ?>
		
		<script type="text/javascript" src="js/gaugebig.js"></script>
		
		<!--Load the AJAX API-->
		<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
		<script type="text/javascript">
		google.charts.load('current', {'packages':['corechart']});
		google.charts.setOnLoadCallback(drawChartd);	
		function drawChartd() {
			var data = new google.visualization.DataTable();
			<?php include 'php/getchartdatafieldsensor.php'; ?>
			data.addRows([ 
			<?php include 'php/getchartdatasensor.php'; ?> 
			]);

			var options = {	
			chartArea:{left:45,top:10,width:"85%",height:"80%"},			
			curveType: 'function',
			legend: { position: 'bottom' },
			vAxis: {
					format: '####',
					<?php include 'php/getunitmeasuresensor.php'; ?>
					<?php include 'php/getminmaxvaluesensor.php'; ?>
					count: 6
			},				
			hAxis:  { gridlines: { count: -1 } },	
			hAxis:  { format: 'H' },	
			title: 'Day',
			titlePosition: 'in',
			lineWidth: 2,
			title: "Day"
			};
		
			var chart = new google.visualization.LineChart(document.getElementById('chartd'));
			chart.draw(data, options);
		}
		
		google.charts.setOnLoadCallback(drawChartm);
		<?php $_GET['period'] = "month"; ?>
		function drawChartm() {
			var data = new google.visualization.DataTable();
			<?php include 'php/getchartdatafieldstatistic.php'; ?>
			data.addRows([
			<?php include 'php/getchartdatasensorstatistic.php'; ?> 
			]);

			var options = {	
			chartArea:{left:45,top:10,width:"85%",height:"80%"},			
			curveType: 'function',
			legend: { position: 'bottom' },
			vAxis: {
					format: '####',
					<?php include 'php/getunitmeasuresensor.php'; ?>
					count: 6
			},				
			hAxis:  { gridlines: { count: -1 } },	
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
		<?php $_GET['period'] = "year"; ?>
		var period = "year";
		function drawCharty() {
			var data = new google.visualization.DataTable();
			<?php include 'php/getchartdatafieldstatistic.php'; ?>
			data.addRows([
			<?php include 'php/getchartdatasensorstatistic.php'; ?> 
			]);

			var options = {	
			chartArea:{left:45,top:10,width:"85%",height:"80%"},			
			curveType: 'function',
			legend: { position: 'bottom' },
			vAxis: {
					format: '####',
					<?php include 'php/getunitmeasuresensor.php'; ?>
					count: 6
			},				
			hAxis:  { gridlines: { count: -1 } },	
			hAxis:  { format: 'MMM' },	
			title: 'Year',
			titlePosition: 'in',
			lineWidth: 2
			};
		
			var chart = new google.visualization.LineChart(document.getElementById('charty'));
			chart.draw(data, options);
			$("#charty").hide();
		}				

		$(document).ready( function(){
			$('#wrapper').load('<?php echo('php/getsensordetails.php'); ?>');
			$('#rightpanel').load('<?php echo('php/getpanels.php'); ?>');
			refresh();
			
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
 
		function refresh()
		{
			setTimeout( function() {
			$('#wrapper').load('<?php echo('php/getsensordetails.php'); ?>');
			refresh();
			}, 2000);
		}
		
	</script>	
	
	</head>
    <body>

	<div data-role="page" id="main">

		<?php include "php/getheader.php"; ?>	
				     	
		<div data-role="content">
			<div id="wrapper" class="wrapper"></div>
			
			<div id="chartd" style="border: 0px solid; position: absolute; left: 0px; top: 215px; width: 100%; height: 240px;"></div>	
			<div id="chartm" style="border: 0px solid; position: absolute; left: 0px; top: 215px; width: 100%; height: 240px;"></div>	
			<div id="charty" style="border: 0px solid; position: absolute; left: 0px; top: 215px; width: 100%; height: 240px;"></div>	
			<div data-role="panel" data-position="left" data-position-fixed="true" data-display="overlay" data-theme="a" id="leftpanel">
				<?php include "php/getoptions.php"; ?>
			</div>				
			<div data-role="panel" data-position="right" data-position-fixed="false" data-display="overlay" data-theme="b" id="rightpanel"></div>	
			<div class="statsd"><input type='button' id='toggled' value='24h'></div>
			<div class="statsm"><input type='button' id='togglem' value='1 Month'></div>
			<div class="statsy"><input type='button' id='toggley' value='1 Year'></div>
		</div>				
	</div>	
	
 </body>
</html>
