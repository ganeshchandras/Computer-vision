<?php
$con=mysqli_connect("localhost","root","12345","userimage");
if (mysqli_connect_errno($con))
{
   echo "Failed to connect to MySQL: " . mysqli_connect_error();
}
$id = $_GET['id'];
$count=$_GET['co'];
$i = 1;
$imageToEcho;
$result = mysqli_query($con,"SELECT * FROM imagedata where id='$id'");
while(($row = mysqli_fetch_array($result)) && ($i<=$count))
{
	$imageToEcho = base64_encode($row['Image']);
	$i=$i+1;
}
echo $imageToEcho;
mysqli_close($con);

?>
