<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>POST</title>
</head>
<body>

    <form action="php/post.php"  method="POST">
        <input type="text" name="name" placeholder="Enter your name">
        <input type="submit" value="Submit">
    </form>

    <?php
        if (isset($_POST['name'])) {
            echo "Hello " . $_POST['name'];
        }
    ?>
    
</body>
</html>