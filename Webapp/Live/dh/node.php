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
			$pageId = "node";
			include_once "php/config.php";
			include_once "php/script.php";			
			include_once "php/acl.php";
			$_SESSION['pageId'] = $pageId;
			$_SESSION['currDir'] = $currentDir;
	    ?>

		<script type="text/javascript">

			$(document).ready( function(){
				$('#wrapper').load('<?php echo('php/getnodes.php'); ?>');
				$('#rightpanel').load('<?php echo('php/getpanels.php');?>');
				$('#leftpanel').load('<?php echo('php/getactuatorshomepage.php'); ?>', function(){
					$(this).trigger('create');
				});		
				refresh();
			});
 
			function refresh()
			{
				setTimeout( function() {
				$('#wrapper').load('<?php echo('php/getnodes.php'); ?>');
				$('#leftpanel').load('<?php echo('php/getactuatorshomepage.php'); ?>', function(){
					$(this).trigger('create');
				});		
				refresh();
				}, 2000);
			}
		</script>	
	
							
    </head>
 
 
 <body>
   <div class="loader"></div> 
	 <div data-role="page" id="main">
 	 
 	  	<?php include "php/getheader.php"; ?>
	 
		
	    <div data-role="content">
			<div id="wrapper" class="wrapper">
			</div>
		</div>

	    <div data-role="panel" data-position="right" data-position-fixed="false" data-display="overlay" data-theme="b" id="rightpanel"></div>
		<div data-role="panel" data-position="left" data-position-fixed="false" data-display="overlay" data-theme="b" id="leftpanel"></div>

		</div>
				
	</div>	
 </body>

</html>
