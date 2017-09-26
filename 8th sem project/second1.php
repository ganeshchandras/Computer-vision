<?php
$directory = "/home/dell/opencv_programs/motionTracking/finale/face_cropped/";
$oldCount = 1;
$filecount = 0;
$files = glob($directory . "*");
if ($files)
{
	$filecount = count($files);
}
echo $filecount;
?>
