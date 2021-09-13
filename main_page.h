char main_page[] = R"-=o=-(<!DOCTYPE html>
<html>

  <head>
    <title>LEDPANEL</title>
    <!-- First some libraries -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js"></script>
    <script src="scripts.js"></script>
    <script language="javascript">
       function init_data() {
          var intensity_slider = document.getElementById("intensity_slider");
          
          console.log("onload slider")
          fetch("get_data?item=brightness")
            .then( (response) => response.text() )
            .then( (ant) => intensity_slider.value = 100*ant/255)
       }
    </script>
    
    <style type="text/css">
    button {
        width:48%;
        height:48%;
        padding:5px;
        border:1px solid #000;
        margin: 0 0 1px;
    }
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 45px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 65px;
      height: 65px;
      border-radius: 50%;
      background: #4CAF50;
      cursor: pointer;
    }

    </style>
  </head>

  <body onload="init_data()">

  <br>
    <button type="button" onclick="fetch('clock')">CLOCK</button>
    <button type="button" onclick="fetch('breakout')">BREAKOUT</button>    
    <button type="button" onclick="fetch('digit')">DIGIT</button>    
    <button type="button" onclick="fetch('stream')">STREAM</button>    
    <br>
    <br>
    <div class="slidecontainer">
    <input type="range" min="0" max="100" value="50" class="slider" id="intensity_slider">
    </div>
    <script>
          var intensity_slider = document.getElementById("intensity_slider");
          intensity_slider.oninput = function() {
            let b = Math.round(255 * this.value / 100)
            console.log(b)
            fetch("set_data?item=brightness&value=" + b)
          }
        </script>    
    <br>
    <br>
    <button type="button" onclick="fetch('left')">LEFT</button>
    <button type="button" onclick="fetch('right')">RIGHT</button>    
    <br>
    <br>
    <button type="button" onclick="fetch('toggle_meander')">Toggle Meander Mode</button>    
    <br>
    <button type="button" onclick="fetch('save_eeprom')">SAVE EEPROM</button>    
    <br>
    <button type="button" onclick="fetch('restart')">Restart u-controller</button>    
    <br>
    <div id="uptime" onload="getUpTime()">
      uptime : 
      </div>
      <script> 
          var uptime = fetch("get_data?item=uptime")
              .then( (response) => response.text())
              .then( (uptime) => document.getElementById('uptime').innerHTML = "uptime : " + String(uptime)) 
          
        </script>
  </body>

</html>
)-=o=-";
