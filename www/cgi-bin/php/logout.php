<?php
session_start();
if (isset($_POST['logout']))
{
    session_destroy();
   
    // echo $_SESSION['name'];

    echo "logout successfuly <a href='/php'>go HOME</a>";
    
}