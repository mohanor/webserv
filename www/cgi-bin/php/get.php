<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
    <title>php</title>
</head>
<body>



       <div class="container ">

    <div class="card d-flex align-items-center justify-content-center">
      <div class="card-body text-center ">
       <h1>Your Name iS <strong class="text-warning"><?php echo @$_GET['name'] ?></strong> </h1>
    
    
      </div>
    </div>
    </div>
 
    
</body>
</html>