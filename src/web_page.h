/**
* HTML web skeleton
*
* @return web page string
*/
String formHTML(){

  String form =""
"<html>\n"
  "<head>\n"
    "<style>\n"

    "body{\n"
      "text-align: center;\n"
      "font-size: 20px;\n"
    "}\n"
    "input{\n"
      "margin: 5px;\n"
      "padding: 10px;\n"
      "border: 1px solid rgba(0,0,0,0.25);\n"
    "}\n"
    "input[type=submit]{\n"
      "border: 1px solid rgba(0,0,0,0.125);\n"
      "background: #2ecc71;\n"
    "}\n"
    "input[type=submit]:hover{\n"
      "background: #27ae60;\n"
    "}\n"
    "form{\n"
      "position: absolute;\n"
      "transform: translate(-50%,-50%);\n"
      "left: 50%;\n"
      "top: 50%;\n"
    "}\n"
    "@media only screen and (max-width: 998px) {\n"
      "form{\n"
        "position: absolute;\n"
        "transform: translate(-50%,-50%) scale(3);\n"
        "left: 50%;\n"
        "top: 50%;\n"
      "}\n"
    "}\n"
    "</style>\n"
  "</head>\n"
  "<body>\n"
    "<form action='ctl' method='get'>\n"
    "Enter your Strobe Frequency here:\n"
    "<br>\n"
    "<input type='text' name='freq' value='"+server.arg("freq")+"'>\n"
    "<br>\n"
    "<input type='submit' value='Set Frequency'>\n"
    "</form>\n"
 "</body>\n"
"</html>\n";

  return (form);
}