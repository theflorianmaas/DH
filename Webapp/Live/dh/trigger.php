<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="content-type" content="text/html;charset=utf-8" />     
    <head>
		<?php  
			require_once('login/config/config.php');
			require_once('login/translations/en.php');
			require_once('login/classes/Login.php'); 
			$login = new Login();
			$pageId = "trigger";
			include_once "php/config.php";
			include_once "php/script.php";
			include_once "php/acl.php";
			$_SESSION['pageId'] = $pageId;
			$_SESSION['currDir'] = $currentDir;
			$_SESSION['id'] = $_GET["id"];	
			IF (isset($_GET["nid"]))
				$_SESSION['nid'] = $_GET["nid"];	
			
			$strArr = explode("/", $_SERVER["REQUEST_URI"]); 
			$home = $strArr[sizeof($strArr) - 3]; 
			$dir = $strArr[sizeof($strArr) - 2]; 
			$currentDir = 'http://' . $_SERVER['HTTP_HOST']. '/' . $home . '/' . $dir ;
		?>				
		<script type="text/javascript">
			$(document).ready( function(){
			$('#rightpanel').load('<?php echo('php/getpanels.php');?>');		
				refresh();
			});
		</script>
	
	</head>
    <body>
	<div data-role="page" id="main">
		
		<?php include "php/getheader.php"; ?>		
		     	
		<div data-role="content">
			
			<div id="wrapper">
				<?php include "php/gettriggers.php"; ?>	
			</div>
			
			<div data-role="panel" data-position="left" data-position-fixed="true" data-display="overlay" data-theme="a" id="leftpanel">
				<?php include "php/getoptions.php"; ?>
			</div>		
			
		</div>
		<div data-role="panel" data-position="right" data-position-fixed="false" data-display="overlay" data-theme="b" id="rightpanel"></div>					
	</div>	

 </body>
</html>
