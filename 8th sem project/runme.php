<?php
require 'class-Clockwork.php';
 
try
{
    // Create a Clockwork object using your API key
    $API_KEY = "6609d27ddaeb20630b5037ab9e5d47d54d8fadb9";
    $clockwork = new Clockwork( $API_KEY );
 
    // Setup and send a message
    $message = array( 'to' => '9740530968', 'message' => 'This is a test!' );
    $result = $clockwork->send( $message );
 
    // Check if the send was successful
    if($result['success']) {
        echo 'Message sent - ID: ' . $result['id'];
    } else {
        echo 'Message failed - Error: ' . $result['error_message'];
    }
}
catch (ClockworkException $e)
{
    echo 'Exception sending SMS: ' . $e->getMessage();
}
?>
