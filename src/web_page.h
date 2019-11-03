/**
* HTML web skeleton
*
* @return web page string
*/
String formHTML(){

  String form =""
  "<html>\n"
  "<head>\n"
  "<form action='ctl' method='get'>\n"
  "Enter your Strobe Frequency here: <input type='text' name='freq' value='"+server.arg("freq")+"'>\n"
  "<input type='submit' value='Set Frequency'>\n"
  "</form>\n"
  "</body>\n"
  "</html>";
  return (form);
}