<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>php</title>
</head>
<body>


 <?php
// session_start();

// Set a session variable
$_SESSION['username'] = 'john';

// Set a session cookie

// Output data to the browser
echo "Hello, world!";
echo "Hello, " . $_SESSION['username'] . "!";
?>
?>
    
</body>
</html>