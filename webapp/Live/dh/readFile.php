<?php
$file = file_get_contents('/var/dhproc/files/datafile.json', true);
print("§statuslight,");
print($file);
print("#");
?>