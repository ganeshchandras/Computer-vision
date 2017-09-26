<?php
send_sms("9740530968","chintuputta","9740530968","hello loafer");
function send_sms($userID,$userPWD,$recerverNO,$message)
{
     if(strlen($message)>140) // check for message length
         {echo "Error : Message length exceeds 140 characters" ; exit(); }
     if (!function_exists('curl_init')) // check for curl library installation
         {echo "Error : Curl library not installed";  exit(); }
 	echo "fuck1";
     $message_urlencode=rawurlencode($message);
      // message converted into URL encoded form
     $cookie_file_path ="/var/www/cookie.txt";
     // Cookie file location in your machine with full read and write permission
 echo "fuck2";
//START OF Code for getting sessionid
        $url="http://site23.way2sms.com/content/index.html";
        $header_array[0] = "GET /content/index.html HTTP/1.1";
        $header_array[1]= "Host: site23.way2sms.com";
        $header_array[2]= "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:10.0.1) Gecko/20100101 Firefox/10.0.1";
        $header_array[3]= "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        $header_array[4]= "Accept-Language: en-us,en;q=0.5";
        $header_array[5]= "Accept-Encoding: gzip,deflate";
        $header_array[6]= "DNT: 1";
        $header_array[7] = "Connection: keep-alive";
        $ch = curl_init();   //initialise the curl variable
        curl_setopt($ch, CURLOPT_URL,$url);
        //set curl URL for crawling
        curl_setopt($ch, CURLOPT_HTTPHEADER, $header_array); 
        //set the header for http request to URL 
        curl_setopt($ch, CURLOPT_REFERER, $reffer);  
         //set reffer url means it shows from where the request is originated.
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);  
         //it means after crawling data will return
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file_path);
        curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_file_path);
        // store the return cookie data in cookie file 
        $result = curl_exec ($ch); // Execute the curl function 
        curl_close ($ch);
echo "fuck3";
//END OF Code for getting sessionid
 
//START OF Code for automatic login and storing cookies
        $post_data = "username=".$userID."&password=".$userPWD."&button=Login";
        $url = "http://site23.way2sms.com/Login1.action";
        $header_array[0]="POST /Login1.action HTTP/1.1";
        $header_array[1]="Host: site23.way2sms.com";
        $header_array[2]="User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:10.0.1) Gecko/20100101 Firefox/10.0.1";
        $header_array[3]="Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        $header_array[4]="Accept-Language: en-us,en;q=0.5";
        $header_array[5]="Accept-Encoding: gzip, deflate";
        $header_array[6]="DNT: 1";
        $header_array[7]="Connection: keep-alive";
        $header_array[8]="Content-Type: application/x-www-form-urlencoded";
        $ch = curl_init();
        curl_setopt( $ch, CURLOPT_URL, $url );
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $header_array);
        curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file_path);
        curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_file_path);
        curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
        curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
        curl_setopt($ch,CURLOPT_REFERER,"http://site23.way2sms.com/content/index.html");
        $content = curl_exec( $ch );
        $response = curl_getinfo( $ch );
        curl_close ($ch);
echo "fuck4";
//END OF Code for automatic login  and storing cookies
 
// START OF Code is  getting way2sms unique user ID
        $url = "http://site23.way2sms.com/jsp/InstantSMS.jsp";
        $ch = curl_init();
        curl_setopt( $ch, CURLOPT_URL, $url );
        curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file_path);
        curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_file_path);
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
        $content = curl_exec($ch);
        curl_close ($ch);
        $regex = '/input type="hidden" name="Action" id="Action" value="(.*)"/';
        preg_match($regex,$content,$match);
        $userID = $match[1];
echo "fuck5";
// END OF Code for getting way2sms unique user ID
 
// START OF Code for sending SMS to Recever
echo "fuck6";
        $post_data = "HiddenAction=instantsms&catnamedis=Birthday&Action=".$userID."&chkall=on&MobNo=".$recerverNO."&textArea=".$message_urlencode;
        $url = "http://site23.way2sms.com/quicksms.action";
        $header_array[0]="POST /quicksms.action HTTP/1.1";
        $header_array[1]="Host: site23.way2sms.com";
        $header_array[2]="User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:10.0.1) Gecko/20100101 Firefox/10.0.1";
        $header_array[3]="Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        $header_array[4]="Accept-Language: en-us,en;q=0.5";
        $header_array[5]="Accept-Encoding: gzip, deflate";
        $header_array[6]="DNT: 1";
        $header_array[7]="Connection: keep-alive";
        $header_array[8]="Content-Type: application/x-www-form-urlencoded";
        $ch = curl_init();
        curl_setopt( $ch, CURLOPT_URL, $url );
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $header_array);
        curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file_path);
        curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_file_path);
        curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
        curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
   curl_setopt($ch,CURLOPT_REFERER,"Referer: http://site23.way2sms.com/jsp/InstantSMS.jsp");
        $content = curl_exec( $ch );
        $response = curl_getinfo( $ch );
        curl_close ($ch);
echo "fuck7";
// END OF Code for sending SMS to Recever
 
//START OF Code for automatic logout
echo "fuck8";
        $url = "http://site23.way2sms.com/jsp/logout.jsp";
        $header_array[0]="GET /jsp/logout.jsp HTTP/1.1";
        $header_array[1]="Host: site23.way2sms.com";
        $header_array[2]="User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:10.0.1) Gecko/20100101 Firefox/10.0.1";
        $header_array[3]="Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        $header_array[4]="Accept-Language: en-us,en;q=0.5";
        $header_array[5]="Accept-Encoding: gzip, deflate";
        $header_array[6]="DNT: 1";
        $header_array[7]="Connection: keep-alive";
        $cookie_file_path ="/var/www/cookie.txt";
        $ch = curl_init();
        curl_setopt( $ch, CURLOPT_URL, $url );
        curl_setopt($ch, CURLOPT_HTTPHEADER, $header_array);
        curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file_path);
        curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie_file_path);
        curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
        curl_setopt( $ch, CURLOPT_AUTOREFERER, true );
        curl_setopt($ch,CURLOPT_REFERER,"Referer: http://site23.way2sms.com/jsp/InstantSMS.jsp");
        $content = curl_exec( $ch );
        $response = curl_getinfo( $ch );
        curl_close ($ch);
echo "fuck9";
//END OF Code for automatic logout
 
}// end function send_sms
?>
