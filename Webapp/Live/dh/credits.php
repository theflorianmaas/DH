<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="content-type" content="text/html;charset=utf-8" />     
    <head>
		<?php 
			require_once('login/config/config.php');
			require_once('login/translations/en.php');
			require_once('login/classes/Login.php');
			$login = new Login();
			$pageId = "credits" ;
			include_once "php/config.php";
			include_once "php/script.php";
			include_once "php/acl.php";
			$_SESSION['pageId'] = $pageId;
			$_SESSION['currDir'] = $currentDir;
		?>
		
		<script type="text/javascript">
			$(document).ready( function(){
			    $('#rightpanel').load('<?php echo('php/getpanels.php');?>');	
				//refresh();
			});
		</script>	
		
</head>
<body>
   
	<div data-role="page" id="main">
		
		<?php include "php/getheader.php"; ?>		
		     	
	    <div data-role="content">
	
			Thanks to:
			
			<div>
			<span><a href="https://jquerymobile.com/" rel="external">JQuery Mobile</a></span>
			</div><div>
			<span><a href="https://php.net/" rel="external">PHP</a></span>
			</div><div>
			<span><a href="https://www.mysql.it/" rel="external">MySQL</a></span>
			</div><div>
			<span><a href="http://www.jqwidgets.com/" rel="external">JQWidget</a></span>
			</div><div>
			<span><a href="http://www.flaticon.com/" rel="external">Flaticon</a></span>
			</div><div>
			<span><a href="https://www.grocerycrud.com/" rel="external">Grocery CRUD</a></span>
			</div>
			
	
	
		
	 	 <div data-role="panel" data-position="right" data-position-fixed="false" data-display="overlay" data-theme="b" id="rightpanel">
		</div>			
			
    
  </div>	
</body>
</html>
