char main_page[] = R"-=o=-(<!DOCTYPE html>
<html>

  <head>
    <!-- First some libraries -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js"></script>

    <!-- Next our own javascript shizzle -->
    <script src="scripts.js"></script>
  </head>

  <body onload="fill_led_matrix()">
    <div id="color_picker_container">
      <input id="color_picker" class="jscolor" value="ab2567">
    </div>
    <div id="led_matrix" />
  </body>

</html>
)-=o=-";
