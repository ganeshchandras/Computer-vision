<?php
 
/*if($_FILES['userfile']['size'] > 0)
{
$fileName = $_FILES['userfile']['name'];
$tmpName  = $_FILES['userfile']['tmp_name'];
$fileSize = $_FILES['userfile']['size'];
$fileType = $_FILES['userfile']['type'];
*/
$oldCount = $_GET["varname2"];
$filecount = $_GET["varname"];
getData();
function getData()
{
	
	for($i=$GLOBALS['oldCount'];$i<$GLOBALS['filecount']+1;$i++)
	{
		$fileName = $i."face.jpg";
		
		//$tmpName  = "img/images/".$i.".jpg";
		//$fp = fopen("img/images/".$i.".jpg", "r") or exit("Unable to open file!");
		$tmpName  = "/home/dell/opencv_programs/motionTracking/finale/face_cropped/".$i."face.jpg";
		$fp = fopen("/home/dell/opencv_programs/motionTracking/finale/face_cropped/".$i."face.jpg", "r") or exit("Unable to open file!");
//$fp      = fopen($tmpName, 'r');
		$content = fread($fp, filesize($tmpName));
		$content = addslashes($content);
		fclose($fp);
		if(!get_magic_quotes_gpc())
		{
		$fileName = addslashes($fileName);
		}
		include "file_constants1.php";
		mysql_connect($host, $user, $pass) OR DIE (mysql_error());

 // select the db
		mysql_select_db ($db) OR DIE ("Unable to select db".mysql_error());
		$query = "INSERT INTO imagedata (id,Image) "."VALUES (1,'$content')";
		mysql_query($query) or die('Error, query failed'); 
		echo "<br>File $fileName uploaded<br>";
} 
$GLOBALS['oldCount'] = $GLOBALS['filecount'];
}
?>
