
#!/usr/bin/php-cgi
<?php
ob_start();
header('Content-Type: text/html');
header('Cache-Control: no-cache');
// Output HTML content
echo "<html>\n";
echo "<head><title>PHP CGI Script</title></head>\n";
echo "<body>\n";
echo "<h1>Hello, world!</h1>\n";
echo "</body>\n";
echo "</html>\n";
ob_end_flush();
?>

<!-- <?php
    setcookie("name", "John Watkin", time()+3600, "/","", 0);
    // ob_start();
    
    
    // if (isset($_POST['name'])) {
        //     echo "Hello " . $_POST['name'];
        // }
        
        ?>
        <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>POST</title>
</head>
<body>


    <form   method="POST">
        <input type="text" name="name" placeholder="Enter your name">
        <input type="submit" value="Submit">
    </form>
    
</body>
</html> -->