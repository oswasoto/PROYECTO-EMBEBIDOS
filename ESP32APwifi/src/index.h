#include <pgmspace.h>

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
body{
  background-color: #1a1a1aff;
  margin: 0;
  padding: 0;
}
.card{
    max-width: 450px;
     min-height: 250px;
     background: #AFAFAF;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>
<body>

<div class="card">
  <h4 id= "previousMillis"> Tiempo de encendido transcurrido: 0 minutos</h4><br>
  <h4>Ángulos de giro del guante en grados</h4><br>
  <h2>Giro en X ; Giro en Y : </h2>
  <h1><span id="ADCValue">0</span></h1><br>
  <br>
  <p>Sistemas Embebidos P107</p>
</div>
<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 500); //500mSeconds update rate // HERE DELAY

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var data = this.responseText.split(";");
      var angulos = data[0];
      angulos += "°";
      angulos += " ; ";
      angulos += data[1];
      angulos += "°";
      document.getElementById("ADCValue").innerHTML = angulos;
      var minutos = Math.floor(parseInt(data[2])/60000); //to minutes converter
      document.getElementById("previousMillis").innerHTML = "Tiempo de encendido transcurrido: " + minutos + " minutos";
      var wvResponse = data[0];
      wvResponse += ";";
      wvResponse += data[1];
      window.AppInventor.setWebViewString("" + wvResponse);  // RESPUESTA A CadenaDeWebView
    }
  };
  xhttp.open("GET", "/obtenerDatos", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";