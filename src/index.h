const char MAIN_page[] PROGMEM = R"=====(


<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 1000px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>

<link rel="stylesheet" type="text/css" href="style2.css">

<body>

<div class="card">
  <h3>Door Data</h3><br>
  <h1>Run Data:<span id="RUNDATA">Updating</span></h1><br>
  <br><a href="eep">Backup Data</a>
</div>
<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 1000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("RUNDATA").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "runData", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";