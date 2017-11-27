<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>php-login-advanced</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
	<script src="http://code.jquery.com/jquery-1.11.1.min.js"></script>
	<link rel="stylesheet" href="http://code.jquery.com/mobile/1.4.3/jquery.mobile.structure-1.4.3.min.css" />
	<script type="text/javascript">
		$(document).bind("mobileinit", function () {
		$.mobile.ajaxEnabled = false;
	});
	</script>
	<script src="http://code.jquery.com/mobile/1.4.3/jquery.mobile-1.4.3.min.js"></script>
	<link rel="stylesheet" href="../themes/dhjqm.min.css" />
	<link rel="stylesheet" href="../themes/jquery.mobile.icons.min.css" />
	
	
	<script>
        $(document).on("mobileinit", function () {
          $.mobile.hashListeningEnabled = false;
          $.mobile.pushStateEnabled = false;
          $.mobile.changePage.defaults.changeHash = false;
        });
    </script>
</head>
<body>



  <div class="loader"></div> 
  
	 <div data-role="page" id="main">
		<div data-role="header">
			<div data-role="navbar" id="navbar">
				<ul><li>&nbsp</li></ul>
			</div>
<?php
// show potential errors / feedback (from login object)
if (isset($login)) {
    if ($login->errors) {
        foreach ($login->errors as $error) {
            echo $error;
        }
    }
    if ($login->messages) {
        foreach ($login->messages as $message) {
            echo $message;
        }
    }
}
?>

</div>
