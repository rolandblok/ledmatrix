char scripts[] = R"-=o=-(
var width = undefined;
var height = undefined;
var server = ""

function fill_led_matrix() {
  fetch("get_data?item=width")
    .then( (response) => response.text() )
    .then( (txt) => width = parseInt(txt) )
    .then(add_matrix_to_document)
    .catch( (ex) => console.log(ex) );

  fetch("get_data?item=height")
    .then( (response) => response.text() )
    .then( (txt) => height = parseInt(txt) )
    .then(add_matrix_to_document)
    .catch( (ex) => console.log(ex) );
}

function get_id_from_xy(x, y) {
  return "x=" + x + "&y=" + y;
}

function convert_color_to_hex(triplet_txt) {
  var parts = triplet_txt.split(",");
  var hex_parts = parts.map((i) => parseInt(i).toString(16).padStart(2, '0'));

  return "#" + hex_parts.join('');
}

function add_matrix_to_document() {
  // Verify that both width and height have been received
  if (width === undefined)
    return;
  if (height === undefined)
    return;

  // Get the element in which the matrix resides
  var container = document.getElementById("led_matrix");

  // Clear the element
  container.innerHTML = "";

  // Fill the element with buttons
  for (var x = 1; x <= width; x++)
  {
    for (var y = 1; y <= height; y++)
    {
      var element = document.createElement("input");
      element.type = "button";
      element.id = get_id_from_xy(x, y);
      element.addEventListener('click', update_color_of_led);

      container.appendChild(element);
    }
    let linebreak = document.createElement("br");
    container.appendChild(linebreak);
  }

  // Give buttons same color as leds
  update_color_of_all_buttons()
}

function update_color_of_led(event) {
  var color_picker = document.getElementById("color_picker");
  var color = color_picker.jscolor.toRGBString().replace("rgb(", "").replace(")", "");
  var buttom_element_id = event.srcElement.id;
  fetch("set_data?item=color&" + buttom_element_id + "&value=" + color)
    .then( () => update_color_of_button(buttom_element_id) )
    .catch( (ex) => console.log(ex) );
}

function update_color_of_all_buttons() {
  fetch("get_data?item=color")
    .then( (response) => response.text() )
    .then( change_background_color_of_buttons )
    .catch( (ex) => console.log(ex) );
}

function update_color_of_button(element_id) {
  var element = document.getElementById(element_id);
  fetch("get_data?item=color&" + element_id)
    .then( (response) => response.text() )
    .then( change_background_color_of_single_button )
    .catch( (ex) => console.log(ex) );
}

function change_background_color_of_buttons(txt) {
  var lines = txt.split('\n');
  lines.forEach( (line) => change_background_color_of_single_button(line) );
}

function change_background_color_of_single_button(line) {
  var tokens = line.split(';');
  if (tokens.length == 3) {
    var x = tokens[0];
    var y = tokens[1];
    var c = tokens[2];
    var id = get_id_from_xy(x,y);    
    var element = document.getElementById(id);
    element.style.backgroundColor = convert_color_to_hex(c);
  }
}

)-=o=-";
