char main_page[] = R"-=o=-(<!DOCTYPE html>
<html>

  <head>
    <!-- First some libraries -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js"></script>

    <!-- Next our own javascript shizzle -->
    <script src="scripts.js"></script>
    <style type="text/css">
    button {
        width:30%;
        height:30%;
        padding:5px;
        border:1px solid #000;
        margin: 0 0 1px;
    }
    </style>
  </head>

  <body>

  <br>
    <button type="button" onclick="fetch('clock')">CLOCK</button>
    <button type="button" onclick="fetch('breakout')">BREAKOUT</button>    
    <button type="button" onclick="fetch('stream')">STREAM</button>    
    <br>
    <br>
    <br>
    <button type="button" onclick="fetch('left')">LEFT</button>
    <button type="button" onclick="fetch('right')">RIGHT</button>    
  </body>

</html>
)-=o=-";
