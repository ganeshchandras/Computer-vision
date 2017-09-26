<?php
error_reporting(E_ALL);
ob_implicit_flush(true);

//If you get Fatal error: Call to undefined function curl_init() , Then you need to enable the curl extension in php.ini


include_once "class.curl.php";
include_once "class.sms.php";

$smsapp=new sms();
$smsapp->setGateway('way2sms'); // you can set gateway to be '160by2' to use your 160by2 account;


echo "Logging in  ... ";
$smsapp->login('9740530968','chintuputta');

echo "Sending SMS ... ";
$result=$smsapp->send('8147996766','hello loafer');

if($result=='true')
{
	echo "Message sent";
}
else
{	
	echo "Error encountered : ".$smsapp->getLastError();
}

?>
