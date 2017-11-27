<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="content-type" content="text/html;charset=utf-8" />     
    <head>
		<?php 
			require_once('login/config/config.php');
			require_once('login/translations/en.php');
			require_once('login/classes/Login.php');
			$login = new Login();
			$pageId = "settings" ;
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
	
		
		<div class="ui-btn ui-btn-inline" onClick="document.getElementById('setNotification').submit();">
		<span>Notification
			<form action="php/setnotification.php" method="post" id="setNotification">
				<input type="hidden" name="sts" value="<?php include 'php/getnotification.php';?>">
				<?php include'php/getnotification.php';?>
			</form>
		</span>
		</div>
		
		<div class="ui-btn ui-btn-inline" onClick="document.getElementById('setinit').submit();">
		<span>Initialize
			<form action="php/setinit.php" method="post" id="setinit">
				<input type="hidden" name="sts" value="<?php include 'php/getinit.php';?>">
				<?php include'php/getinit.php';?>
			</form>
		</span>
		</div>
			
		<div class="ui-btn ui-btn-inline" onClick="document.getElementById('monproc').submit();">
		<span>Monitor procedures
			<form action="monproc.php" method="post" id="monproc">
			</form>
		</span>
		</div>

		<div class="ui-btn ui-btn-inline" onClick="document.getElementById('mapadmin').submit();">
		<span>Map Admin
			<form action="mapadmin.php" method="post" id="mapadmin">
			</form>
		</span>
		</div>	
		
		<div class="ui-btn ui-btn-inline" onClick="document.getElementById('admin').submit();">
		<span>Configuration
			<form action="admin/index.php/main" method="post" id="admin">
			</form>
		</span>
		</div>
		
		<div class="ui-btn ui-btn-inline" onClick="document.getElementById('regeneratemethodvalues').submit();">
		<span>Regenerate Method Values
			<form action="php/regeneratemethodvalues.php" method="post" id="regeneratemethodvalues">
			</form>
		</span>
		</div>
		
		<div class="ui-btn ui-btn-inline" onClick="document.getElementById('deletehist').submit();">
		<span>Delete History data
			<form action="php/deletehistorydata.php" method="post" id="deletehist">
			</form>
		</span>
		</div>
	
		<div class="ui-btn ui-btn-inline">
		<span>
			<a href="login/edit.php" rel="external"><?php echo WORDING_EDIT_USER_DATA; ?></a>
		</span>	
		</div>
	
		<div class="ui-btn ui-btn-inline">
		<span>
			<a href="credits.php" rel="external">Credits</a>
		</span>	
		</div>
	
		<div class="ui-btn ui-btn-inline">
		<span>
			<a href="login/index.php?logout" rel="external"><?php echo WORDING_LOGOUT; ?></a>
		</span>	
		</div>
		
	 	 <div data-role="panel" data-position="right" data-position-fixed="false" data-display="overlay" data-theme="b" id="rightpanel">
		</div>			
			
    
  </div>	
</body>
</html>
