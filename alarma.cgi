t <html><head><title>ALARMA</title></head>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
# Define URL and refresh timeout
t function plotRTCGraph() {
t  alarma = document.getElementById("Alarma").value;
t }
t function sendUpdate() {
t  alarma = document.getElementById("Alarma").value;
t }
t function periodicUpdate() {
t  if(document.getElementById("AlarmChkBox").checked == true) {
t   updateMultiple(formUpdate,plotRTCGraph);
t  }
t </script></head>
i pg_header.inc
t <h2 align=center><br>ALARM Module </h2>
t <form action=alarma.cgi method=post name=cgi>
t <input type=hidden value="Alarma" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#aaffff>
t  <th width=40%>Item</th>
t  <th width=60%>Setting</th></tr>
# Here begin data setting which is formatted in HTTP_CGI.C module
t <tr><td><img src=pabb.gif>Alarma: </td>
c b 1 <td><input type=text name=alarm1 id="Alarma" value="%s" size=20 maxlength=20"></td></tr>
t </font></table>
t Activar:<input type="checkbox" id="AlarmChkBox" onclick="periodicUpdate()">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.

