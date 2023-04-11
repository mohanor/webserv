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




 <?php
// session_start();
if (isset($_GET['sayfat'])) {
    $korit = $_GET['korit'];
    if (empty($korit))
    {
        echo '<div class="container ">

    <div class="card d-flex align-items-center justify-content-center">
      <div class="card-body text-center ">
       <h1>smiya khawaya 3awd 3amarha </h1>
       <form method="GET">
       <input class="form-control form-control-lg" type="text" placeholder="ktab smitk" name="korit" require >
       <br/>
       <button type="submit" class="btn btn-success" name="sayfat"> sayfat</button>
     
       </form>
     
      
    
      </form>
    
    
      </div>
    </div>
    </div>';
    }

    else
    {
        echo '<div class="container ">

    <div class="card d-flex align-items-center justify-content-center">
      <div class="card-body text-center ">
       <h1>Sma3 Asi Korrrit </h1>
      
    
      </form>
    
    
      </div>
    </div>
    </div>';
    }
   
    // header("Location: index.php"
    
}
else
{
    echo '<div class="container ">

    <div class="card d-flex align-items-center justify-content-center">
      <div class="card-body text-center ">
       <h1>CHNO SMITK ?</h1>
      <form method="GET">
      <input class="form-control form-control-lg" type="text" placeholder="ktab smitk" name="korit" require >
      <br/>
      <button type="submit" class="btn btn-success" name="sayfat"> sayfat</button>
    
      </form>
    
    
      </div>
    </div>
    </div>';
}
?>
    
</body>
</html>