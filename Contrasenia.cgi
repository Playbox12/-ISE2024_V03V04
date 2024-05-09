t <html><head><title>PASSWORD</title></head>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
# Define URL and refresh timeout
t function plotRTCGraph() {
t  contrasenia = document.getElementById("Contrasenia").value;
t }
t function sendUpdate() {
# //Esta funcion sirve para guardar el valor de la contraseña desde el elemento HTML
t  contrasenia = document.getElementById("Contrasenia").value; 
t }
t function changeConfirm(f){
t  if(!confirm('Are you sure you want\nto change the password?')) return;
t  f.submit();
t }
t </script></head>
i pg_header.inc
t <h2 align=center><br>PASSWORD Module </h2>
t <form action=Contrasenia.cgi method=post name=cgi>
t <input type=hidden value="Contrasenia" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#aaffff>
t  <th width=40%>Item</th>
t  <th width=60%>Setting</th></tr>
# Here begin data setting which is formatted in HTTP_CGI.C module
t <tr><td><img src=pabb.gif>Contraseña: </td>
c g 1 <td><input type=text name=parking1 id="Contrasenia" value="%s" size=20 maxlength=20"></td></tr>
c g 2 <td><input type=text name=parking2 id="NumPlazas" value="%s" size=20 maxlength=20"></td></tr>
t </font></table>
# Here begin button definitions
t <p align=center>
t <input type=button name=set value="Change" onclick="changeConfirm(this.form)">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.

