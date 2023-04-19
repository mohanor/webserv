<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js"></script>

    <title>PHP</title>
</head>
<body>
<div>
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
      <form class="d-flex" role="search" method="post"> 
       <p><?php echo  $_SESSION['name'];  ?></p>
        <button class="btn btn-outline-danger" type="submit" name="logout">Logout </button>
      </form>
      <?php } ?>
    </div>
  </div>
</nav>

<div class="container-sm h-100 d-flex align-items-center justify-content-center mt-5">


<form method="post" action="/profile">

  <div class="mb-3 row">
    <div class="col-sm-10">
      <input type="text" class="form-control" type="text" placeholder="Entre your name here"name="name">
    </div>
  </div>

  <input type="submit" class="btn btn-success" value="what's your name ?" name="submit">

</form>


</div>
    
</div>


    
</body>
</html>