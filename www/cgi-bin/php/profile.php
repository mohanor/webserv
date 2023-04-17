<?php
session_start();

if(isset($_POST['submit']))
{
  $name = $_POST['name'];
  if (empty($name))
  {
    echo "Please enter your name";
    return ;
  }
  else
  {
    setcookie('name', $name, time()+3600);
    $_SESSION['is_logged'] = true;
    $_SESSION['name'] = $name;
  }
}

?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ENjdO4Dr2bkBIFxQpeoTz1HIcje39Wm4jDKdf19U8gI4ddQ3GYNS7NTKfAdVQSZe" crossorigin="anonymous"></script>
        <title>PHP</title>
    </head>
    <body>

        <nav class="navbar navbar-expand-lg bg-body-tertiary">
        <div class="container-fluid">
            <a class="navbar-brand" href="#">WebServ</a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
            <ul class="navbar-nav me-auto mb-2 mb-lg-0">
                <li class="nav-item">
                <a class="nav-link active" aria-current="page" href="#">Home</a>
                </li>
                
                </li>
                <li class="nav-item">
                <a class="nav-link disabled">Disabled</a>
                </li>
            </ul>
            <?php if (@$_SESSION['is_logged']) { ?>
            <form class="d-flex" action="/logout" method="post"> 
            <p><?php echo  $_SESSION['name'];  ?></p>
                <button class="btn btn-outline-danger" type="submit" name="logout">Logout </button>
            </form>
            <?php } ?>
            </div>
        </div>
        </nav>

        <?php if (@$_SESSION['is_logged']) { ?>

        <div>
            <h1>Welcome: ( <?php echo  @$_SESSION['name'];  ?>  )     </h1>

            <img src="php/korit.jpeg"  height="500px" />

            <h2 class="text-danger">Hada Rah  L WebServ</h2>
        
        </div>

        <?php } ?>
    </body>
</html>



