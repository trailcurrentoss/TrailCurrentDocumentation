#include "WS_WIFI.h"

// The name and password of the WiFi access point
const char *ssid = APSSID;                
const char *password = APPSK;               
IPAddress apIP(192, 168, 4, 1);    // Set the IP address of the AP

char ipStr[16];
WebServer server(80);                          

void handleRoot() {
  String myhtmlPage =
    String("") +
    "<html>"+
    "<head>"+
    "    <meta charset=\"utf-8\">"+
    "    <title>ESP32-S3-RS485-CAN</title>"+
    "    <style>" +
    "        body {" +
    "            font-family: Arial, sans-serif;" +
    "            background-color: #f0f0f0;" +
    "            margin: 0;" +
    "            padding: 0;" +
    "        }" +
    "        .header {" +
    "            text-align: center;" +
    "            padding: 20px 0;" +
    "            background-color: #333;" +
    "            color: #fff;" +
    "            margin-bottom: 20px;" +
    "        }" +
    "        .container {" +
    "            max-width: 600px;" +
    "            margin: 10 auto;" +
    "            padding: 20px;" +
    "            background-color: #fff;" +
    "            border-radius: 5px;" +
    "            box-shadow: 0 0 5px rgba(0, 0, 0, 0.3);" +
    "        }" +
    
    "        .input-container {//" +
    "            display: flex;" +
    "            align-items: center;" +
    "            margin-bottom: 15px;" +
    "        }" +
    "        .input-container label {" +
    "            width: 80px;" + 
    "            margin-right: 10px;" +
    "        }" +
    "        .input-container input[type=\"text\"] {" +
    "            flex: 1;" +
    "            padding: 5px;" +
    "            border: 1px solid #ccc;" +
    "            border-radius: 3px;" +
    "            margin-right: 10px; "+ 
    "        }" +
    "        .input-container button {" +
    "            padding: 5px 10px;" +
    "            background-color: #333;" +
    "            color: #fff;" +
    "            font-size: 14px;" +
    "            font-weight: bold;" +
    "            border: none;" +
    "            border-radius: 3px;" +
    "            text-transform: uppercase;" +
    "            cursor: pointer;" +
    "        }" +
    "        .button-container {" +
    "            margin-top: 20px;" +
    "            text-align: center;" +
    "        }" +
    "        .button-container button {" +
    "            margin: 0 5px;" +
    "            padding: 10px 15px;" +
    "            background-color: #333;" +
    "            color: #fff;" +
    "            font-size: 14px;" +
    "            font-weight: bold;" +
    "            border: none;" +
    "            border-radius: 3px;" +
    "            text-transform: uppercase;" +
    "            cursor: pointer;" +
    "        }" +
    "        .button-container button:hover {" +
    "            background-color: #555;" +
    "        }" +
    "        .form-group label {" + 
    "            display: block;" + 
    "            font-weight: bold;" + 
    "        }" + 
    "        nav {" +
    "            margin: 15px 0;" +
    "            text-align: center;" +
    "        }" +
    "        nav a {" +
    "            padding: 10px 50px;" +
    "            background-color: #333;" +
    "            color: white;" +
    "            text-decoration: none;" +
    "            font-weight: bold;" +
    "            border-radius: 5px;" +
    "        }" +
    "        nav a.relayControlActive {" + 
    "            background-color: #fff;" +   
    "            color: #333;" +
    "            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3), 0 1px 3px rgba(0, 0, 0, 0.1);" +
    "            transform: translateY(-4px);" +
    "            transition: all 0.2s ease-in-out;" +
    "        }" + 
    "        nav a.rtcEventActive {" + 
    "            background-color: #555;" +
    "        }" + 
    "    </style>" +
    "</head>"+
    "<body>"+
    "    <script defer=\"defer\">"+
    "        function SetRS485BaudRate() {"+
    "            var dataType = document.getElementById('RS485BaudRate').value;" +
    "            var WebData = " +
    "                'RS485 BaudRate: ' + dataType + '  ' + '\\n' + " + 
    "                'Web End' + '\\n' ;" + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/RS485SetBaudRate?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function RS485Config() {"+
    "            var dataType = document.getElementById('RS485ReadDataType').value;" +
    "            var WebData = " +
    "                'Data Type: ' + dataType + '  ' + '\\n' + " + 
    "                'Web End' + '\\n' ;" + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/RS485SetConfig?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function RS485Send() {"+
    "            var dataType = document.getElementById('DataType').value;" +
    "            var rs485Data = document.getElementById('RS485SendData').value;" +
    "            var WebData = " +
    "                'Data Type: ' + dataType + '  ' + '\\n' + " + 
    "                'RS485 Data: ' + rs485Data + '  ' + '\\n' + " + 
    "                'Web End' + '\\n' ;" + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/RS485Send?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function handleRS485Input(input) {"+
    "            const dataType = document.getElementById(\"DataType\").value;"+
    "            if (dataType === \"1\") {"+
    "                let raw = input.value.replace(/[^0-9a-fA-F]/g, '');"+
    "                let spaced = raw.match(/.{1,2}/g);"+
    "                input.value = spaced ? spaced.join(' ') : '';"+
    "            }"+
    "        }"+
    "        function RS485Read() {"+
    "            var xhr = new XMLHttpRequest();"+
    "            xhr.open('GET', '/getRS485Data', true);"+
    "            xhr.onreadystatechange = function() {"+
    "              if (xhr.readyState === 4 && xhr.status === 200) {"+
    "                var dataArray = JSON.parse(xhr.responseText);"+
    "                if (dataArray.length > 0 && dataArray[0] !== '') {" +
    "                  var textarea = document.getElementById('RS485ReadData');"+
    "                  var isAtBottom = (textarea.scrollHeight - textarea.scrollTop - textarea.clientHeight) < 10;"+
    "                  textarea.value += dataArray;"+
    "                  if (isAtBottom) {"+
    "                      textarea.scrollTop = textarea.scrollHeight;"+
    "                  }"+
    // "                  var currentData = document.getElementById('RS485ReadData').value;"+
    // "                  document.getElementById('RS485ReadData').value = currentData + dataArray;"+
    "                }"+
    "              }"+
    "            };"+
    "            xhr.send();"+
    "        }"+
    "        function SetCANRate() {"+
    "            var dataType = document.getElementById('CANUpdateRate').value;" +
    "            var WebData = " +
    "                'CAN Rate: ' + dataType + '  ' + '\\n' + " + 
    "                'Web End' + '\\n' ;" + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/CANSetRate?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function CANSend() {"+
    "            var canID = document.getElementById('CANid').value;" +
    "            var canExtd = document.getElementById('CANextd').value;" +
    "            var canData = document.getElementById('CANSendData').value;" +
    "            var WebData = " +
    "                'CAN ID: ' + '0x' + canID.toUpperCase() + '  ' + '\\n' + " + 
    "                'CAN Extd: ' + canExtd + '  ' + '\\n' + " + 
    "                'CAN Data: ' + canData + '  ' + '\\n' + " + 
    "                'Web End' + '\\n' ;" + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/CANSend?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function CANRead() {"+
    "            var xhr = new XMLHttpRequest();"+
    "            xhr.open('GET', '/getCANData', true);"+
    "            xhr.onreadystatechange = function() {"+
    "              if (xhr.readyState === 4 && xhr.status === 200) {"+
    "                var dataArray = JSON.parse(xhr.responseText);"+
    "                if (dataArray.length > 0 && dataArray[0] !== '') {" +
    "                  var textarea = document.getElementById('CANReadData');"+
    "                  var isAtBottom = (textarea.scrollHeight - textarea.scrollTop - textarea.clientHeight) < 10;"+
    "                  textarea.value += dataArray;"+
    "                  if (isAtBottom) {"+
    "                      textarea.scrollTop = textarea.scrollHeight;"+
    "                  }"+
    // "                  var currentData = document.getElementById('CANReadData').value;"+
    // "                  document.getElementById('CANReadData').value = currentData + dataArray;"+
    "                }"+
    "              }"+
    "            };"+
    "            xhr.send();"+
    "        }"+
    "        function ReadConfig() {"+
    "            var xhr = new XMLHttpRequest();"+
    "            xhr.open('GET', '/getRateConfig', true);"+
    "            xhr.onreadystatechange = function() {"+
    "              if (xhr.readyState === 4 && xhr.status === 200) {"+
    "                var dataArray = JSON.parse(xhr.responseText);"+
    "                if (dataArray.rs485_baud !== undefined) {" +
    "                  document.getElementById('RS485BaudRate').value = dataArray.rs485_baud"+
    "                }"+
    "                if (dataArray.can_rate  !== undefined) {" +
    "                  document.getElementById('CANUpdateRate').value = dataArray.can_rate"+
    "                }"+
    "              }"+
    "            };"+
    "            xhr.send();"+
    "        }"+                           
    "        ReadConfig();"+     
    "        var refreshInterval = 500;"+                            
    "        setInterval(RS485Read, refreshInterval);"+                      
    "        setInterval(CANRead, refreshInterval);"+       
    "    </script>" +
    "    <div class=\"header\">"+
    "        <h1>ESP32-S3-RS485-CAN</h1>"+
    "    </div>"+
    "    <nav>" +
    "        <a href=\"/\" id=\"SerialControlLink\" class=\"SerialControlActive\">Serial Control</a>" +  
    "        <a href=\"/RTC_Event\" id=\"rtcEventLink\" class=\"rtcEventActive\">RTC Event</a>" + 
    "    </nav>" +
    "    <div class=\"container\">"+
    "        <div class=\"form-group\">" + 
    "            <label for=\"RS485\">RS485:</label>" +
    "            <select id=\"RS485BaudRate\" style=\"width: 120px; text-align: left;\">" + 
    "                <option value=\"110\">110bps</option>" + 
    "                <option value=\"300\">300bps</option>" + 
    "                <option value=\"600\">600bps</option>" + 
    "                <option value=\"1200\">1200bps</option>" + 
    "                <option value=\"2400\">2400bps</option>" + 
    "                <option value=\"4800\">4800bps</option>" + 
    "                <option value=\"9600\">9600bps</option>" + 
    "                <option value=\"14400\">14400bps</option>" + 
    "                <option value=\"19200\">19200bps</option>" + 
    "                <option value=\"38400\">38400bps</option>" + 
    "                <option value=\"56000\">56000bps</option>" + 
    "                <option value=\"57600\">57600bps</option>" + 
    "                <option value=\"115200\">115200bps</option>" + 
    "                <option value=\"128000\">128000bps</option>" + 
    "                <option value=\"230400\">230400bps</option>" + 
    "                <option value=\"460800\">460800bps</option>" + 
    "                <option value=\"500000\">500000bps</option>" + 
    "                <option value=\"512000\">512000bps</option>" + 
    "                <option value=\"600000\">600000bps</option>" + 
    "                <option value=\"750000\">750000bps</option>" + 
    "                <option value=\"921600\">921600bps</option>" + 
    "                <option value=\"1000000\">1000000bps</option>" + 
    "                <option value=\"1500000\">1500000bps</option>" + 
    "                <option value=\"2000000\">2000000bps</option>" + 
    "                <option value=\"3000000\">3000000bps</option>" + 
    "                <option value=\"5000000\">5000000bps</option>" + 
    "            </select>" +   
    "            <button class=\"btn\" id=\"RS485RateConfig\" onclick=\"SetRS485BaudRate()\">Set BaudRate</button>" + 
    "            <label for=\"RS485Send\" style=\"font-weight:normal;\">RS485 Send Data: </label> "+
    "            <select id=\"DataType\" style=\"width: 120px; text-align: left;\">" + 
    "                <option value=\"0\">字符串(char)</option>" + 
    "                <option value=\"1\">十六进制(hex)</option>" + 
    "            </select>" + 
    "            <span><br style=\"margin-bottom: 20px;\"></span>" + 
    "            <input type=\"text\" id=\"RS485SendData\" style=\"width: 500px;\" value=\"12\" oninput=\"handleRS485Input(this)\">" +   
    "            <button class=\"btn\" id=\"SendData\" onclick=\"RS485Send()\">Send Data</button>" +   
    "            <span><br></span>" + 
    "            <span><br></span>" + 
    "            <label for=\"RS485Read\" style=\"font-weight:normal;\">RS485 Receive Data: </label> "+
    "            <select id=\"RS485ReadDataType\" style=\"width: 120px; text-align: left;\">" + 
    "                <option value=\"0\">字符串(char)</option>" + 
    "                <option value=\"1\">十六进制(hex)</option>" + 
    "            </select>" +   
    "            <button class=\"btn\" id=\"RS485ReadDataTypeConfig\" onclick=\"RS485Config()\">Set Config</button>" +
    "            <span><br></span>" +  
    "            <span style=\"font-size: 8px;\">  注意：配置为字符串时，发送端发送数据若存在不可打印字符(0x00~0x1F, 0x7F)将会出现空白数据</span>" + 
    "            <span><br></span>" +  
    "            <span style=\"font-size: 8px;\">  Note: When configured as a string, if the data sent by the sender contains non-printable characters (0x00~0x1F, 0x7F), blank data will appear</span>" +  
    "            <textarea  id=\"RS485ReadData\" style=\"width: 500px; height: 100px; resize: vertical; word-break: break-all;\" placeholder=\"No data was received...\" ></textarea>" + 
    "        </div>" + 
    "    </div>"+
    "    <div class=\"container\">"+
    "        <div class=\"form-group\">" + 
    "            <label for=\"CAN\">CAN:</label>" + 
    "            <select id=\"CANUpdateRate\" style=\"width: 120px; text-align: left;\">" + 
    "                <option value=\"25\">25Kbps</option>" + 
    "                <option value=\"50\">50Kbps</option>" + 
    "                <option value=\"100\">100Kbps</option>" + 
    "                <option value=\"125\">125Kbps</option>" + 
    "                <option value=\"250\">250Kbps</option>" + 
    "                <option value=\"500\">500Kbps</option>" + 
    "                <option value=\"800\">800Kbps</option>" + 
    "                <option value=\"1000\">1Mbps</option>" +  
    "            </select>" +   
    "            <button class=\"btn\" id=\"CANRateConfig\" onclick=\"SetCANRate()\">Set Rate</button>" + 
    "            <label for=\"CANSend\" style=\"font-weight:normal;\">CAN Send Data: </label> "+
    "            <label for=\"CAN\" style=\"font-weight:normal;\">CAN frame information:</label>" + 
    "            <span>CAN ID : 0x</span>" + 
    "            <input type=\"text\" id=\"CANid\" style=\"width: 160px; text-align: left;\" value=\"00000000\">" + 
    "            <select id=\"CANextd\" style=\"width: 200px; text-align: left;\">" + 
    "                <option value=\"0\">标准帧(Standard frames)</option>" + 
    "                <option value=\"1\">扩展帧(Extended frames)</option>" + 
    "            </select>" + 
    "            <span><br style=\"margin-bottom: 20px;\"></span>" + 
    "            <input type=\"text\" id=\"CANSendData\" style=\"width: 500px;\" value=\"12\" pattern=\"[0-9a-fA-F]*\" oninput=\"this.value = this.value.replace(/[^0-9a-fA-F]/g, '').match(/.{1,2}/g)?.join(' ') || ''\">" +   
    "            <button class=\"btn\" id=\"SendData\" onclick=\"CANSend()\">Send Data</button>" +   
    "            <span><br></span>" + 
    "            <span><br></span>" + 
    "            <label for=\"CANRead\" style=\"font-weight:normal;\">CAN Receive Data: </label> "+
    "            <textarea  id=\"CANReadData\" style=\"width: 500px; height: 100px; resize: vertical; word-break: break-word;\" placeholder=\"No data was received...\"></textarea>" + 
    "        </div>" + 
    "    </div>"+
    "</body>"+
    "</html>";
    
  server.send(200, "text/html", myhtmlPage); 
  printf("The user visited the home page\r\n");
  
}
void handleRTCPage() {      
    String rtcPage = String("") + 
    "<html>" + 
    "<head>" + 
    "    <meta charset=\"utf-8\">" + 
    "    <title>ESP32-S3-RS485-CAN</title>" + 
    "    <style>" + 
    "        body {" + 
    "            font-family: Arial, sans-serif;" + 
    "            background-color: #f0f0f0;" + 
    "            margin: 0;" + 
    "            padding: 0;" + 
    "        }" + 
    "        .header {" + 
    "            text-align: center;" + 
    "            padding: 20px 0;" + 
    "            background-color: #333;" + 
    "            color: #fff;" + 
    "            margin-bottom: 20px;" + 
    "        }" + 
    "        .container {" + 
    "            max-width: 600px;" + 
    "            margin: 10px auto;" + 
    "            padding: 20px;" + 
    "            background-color: #fff;" + 
    "            border-radius: 5px;" + 
    "            box-shadow: 0 0 5px rgba(0, 0, 0, 0.3);" + 
    "        }" + 
    "        .form-group {" + 
    "            margin-bottom: 15px;" + 
    "        }" + 
    "        .form-group label {" + 
    "            display: block;" + 
    "            font-weight: bold;" + 
    "        }" + 
    "        .form-group input {" + 
    "            width: 80px;" + 
    "            height: 25px;" + 
    "            padding: 4px;" + 
    "            margin-top: 5px;" + 
    "            border: 1px solid #ddd;" + 
    "            border-radius: 4px;" + 
    "            box-sizing: border-box;" + 
    "            text-align: right; " + 
    "        }" + 
    "        .form-group select {" + 
    "            width: 80px;" + 
    "            height: 25px;" + 
    "            padding: 4px;" + 
    "            margin-top: 5px;" + 
    "            border: 1px solid #ddd;" + 
    "            border-radius: 4px;" + 
    "            box-sizing: border-box;" + 
    "            text-align: right; " + 
    "        }" + 
    "        .form-group .btn {" + 
    "            padding: 10px 20px;" + 
    "            background-color: #333;" + 
    "            color: white;" + 
    "            border: none;" + 
    "            border-radius: 5px;" + 
    "            cursor: pointer;" + 
    "        }" + 
    "        .form-group .btn:hover {" + 
    "            background-color: #555;" + 
    "        }" + 
    "        .Events{"+
    "            font-size: 13px;"+
    "            word-wrap: break-word;"+
    "            overflow-wrap: break-word;"+
    "            max-width: 100%;"+
    "            white-space: nowrap;"+
    "            padding: 2px;"+
    "        }"+
    "        .Events button {"+
    "            float: right;" + 
    "            margin-left: 1px;" + 
    "        }"+
    "        .Events li {"+
    "            font-size: 13px;" + 
    "        }"+
    "        nav {" + 
    "            margin: 15px 0;" + 
    "            text-align: center;" + 
    "        }" + 
    "        nav a {" + 
    "            padding: 10px 50px;" + 
    "            background-color: #333;" + 
    "            color: white;" + 
    "            text-decoration: none;" + 
    "            font-weight: bold;" + 
    "            border-radius: 5px;" + 
    "        }" + 
    "        nav a.relayControlActive {" + 
    "            background-color: #555;" + 
    "        }" + 
    "        nav a.rtcEventActive {" + 
    "            background-color: #fff;" + 
    "            color: #333;" + 
    "            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3), 0 1px 3px rgba(0, 0, 0, 0.1);" + 
    "            transform: translateY(-4px);" + 
    "            transition: all 0.2s ease-in-out;" + 
    "        }" + 
    "    </style>" + 
    "</head>" + 
    "<body>" +
    "    <script defer=\"defer\">" +
    "        function getRtcEventData() {" +
    "            var dateBox1 = document.getElementById('DateBox1').value;" +
    "            var dateBox2 = document.getElementById('DateBox2').value;" +
    "            var dateBox3 = document.getElementById('DateBox3').value;" +
    "            var week = document.getElementById('Week').value;" +
    "            var timeBox1 = document.getElementById('TimeBox1').value;" +
    "            var timeBox2 = document.getElementById('TimeBox2').value;" +
    "            var timeBox3 = document.getElementById('TimeBox3').value;" +
    "            var serialPort = document.getElementById('SerialPort').value;" +
    "            var canID = document.getElementById('CANid').value;" +
    "            var canExtd = document.getElementById('CANextd').value;" +
    "            var serialData = document.getElementById('SerialData').value;" +
    "            var dataType = document.getElementById('DataType').value;" +
    "            var cycleDuration = document.getElementById('CycleDuration').value;" +
    "            var cycleBox = document.getElementById('CycleBox1').value;" +
    "            var WebData = " +
    "                'Date: ' + dateBox1 + '/' + dateBox2 + '/' + dateBox3 + '  ' + '\\n' + " + 
    "                'Week: ' + week + '  ' + '\\n' + " + 
    "                'Time: ' + timeBox1 + ':' + timeBox2 + ':' + timeBox3 +  '  ' + '\\n' + " + 
    "                'Serial Port: ' + serialPort + '  ' + '\\n' + " + 
    "                'CAN ID: ' + '0x' + canID.toUpperCase() + '  ' + '\\n' + " + 
    "                'CAN Extd: ' + canExtd + '  ' + '\\n' + " + 
    "                'Serial Data: ' + serialData + '  ' + '\\n' + " + 
    "                'Data Type: ' + dataType + '  ' + '\\n' + " + 
    "                'Cycle Duration: ' + cycleDuration + '  ' + '\\n' + " + 
    "                'Cycle: ' + cycleBox + '  ' + '\\n' ;" + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/NewEvent?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function handleSerialInput(input) {"+
    "            const dataType = document.getElementById(\"DataType\").value;"+
    "            if (dataType === \"1\") {"+
    "                let raw = input.value.replace(/[^0-9a-fA-F]/g, '');"+
    "                let spaced = raw.match(/.{1,2}/g);"+
    "                input.value = spaced ? spaced.join(' ') : '';"+
    "            }"+
    "        }"+
    "        function deleteEvent(eventId) {"+
    "            var xhr = new XMLHttpRequest();"+
    "            var EventId = eventId;"+
    "            xhr.open('GET', '/DeleteEvent?id=' + EventId, true);"+
    "            xhr.send();"+
    "        }"+
    "        function updateList(data) {"+
    "            var list = document.getElementById(\"myList\");"+
    "            list.innerHTML = \'\'; "+
    "            for (let i = 0; i < data.eventCount; i++) {"+
    "                var newItem = document.createElement(\"li\");"+
    "                var eventContent = data[\"eventStr\" + (i + 1)].replace(/\\n/g, \"<br>\");"+
    "                newItem.innerHTML = eventContent;"+
    "                var eventButton = document.createElement(\"button\");"+
    "                eventButton.textContent = \"Delete\" + \"Event\" + (i + 1);"+
    "                eventButton.onclick = function() {"+
    "                    deleteEvent(i + 1);"+
    "                };"+
    "                newItem.style.display = 'flex';"+
    "                newItem.style.justifyContent = 'space-between';"+
    "                newItem.style.alignItems = 'center';"+
    "                newItem.appendChild(eventButton);"+
    "                list.appendChild(newItem);"+
    "            }"+
    "        }"+
    "        function upTime() {"+
    "            var xhr = new XMLHttpRequest();"+
    "            xhr.open('GET', '/getTimeAndEvent', true); "+
    "            xhr.onreadystatechange = function() {"+
    "                if (xhr.readyState === 4 && xhr.status === 200) {"+
    "                    var data = JSON.parse(xhr.responseText); "+
    "                    document.getElementById(\"Time\").textContent = data.time;"+
    "                    updateList(data); "+
    "                }"+
    "            };"+
    "            xhr.send();"+
    // "        fetch(\'/getTimeAndEvent\')"+
    // "            .then(response => response.json())"+
    // "            .then(data => {"+
    // "                document.getElementById(\"Time\").textContent = data.time;"+
    // "                updateList(data);"+
    // "            })"+
    // "            .catch(error => {"+
    // "                console.error(\'Error fetching time and events:\', error);"+
    // "            });"+
    "        }"+
    "        function toggleCANFrame() {"+   
    "            var serialPort = document.getElementById('SerialPort').value;"+   
    "            var canFrameDiv = document.getElementById('CANFrame');"+   
    "            var dataTypeDiv = document.getElementById('DataType');"+   
    "            if (serialPort === '1') { "+   
    "                canFrameDiv.style.display = 'block';"+   
    "                dataTypeDiv.value = \"1\";  "+  
    "                dataTypeDiv.disabled = true; "+  
    "            } else {"+   
    "                canFrameDiv.style.display = 'none';"+   
    "                dataTypeDiv.disabled = false;"+ 
    "            }"+   
    "        }"+      
    "        function UpDataRtcTime() {" +
    "            var dateBox1 = document.getElementById('RtcDateBox1').value;" +
    "            var dateBox2 = document.getElementById('RtcDateBox2').value;" +
    "            var dateBox3 = document.getElementById('RtcDateBox3').value;" +
    "            var week = document.getElementById('RtcWeek').value;" +
    "            var timeBox1 = document.getElementById('RtcTimeBox1').value;" +
    "            var timeBox2 = document.getElementById('RtcTimeBox2').value;" +
    "            var timeBox3 = document.getElementById('RtcTimeBox3').value;" +
    "            var WebData = " +
    "                'Date: ' + dateBox1 + '/' + dateBox2 + '/' + dateBox3 + '  ' + '\\n' + " + 
    "                'Week: ' + week + '  ' + '\\n' + " + 
    "                'Time: ' + timeBox1 + ':' + timeBox2 + ':' + timeBox3 +  '  ' + '\\n' ; " + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/SetRtcTime?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function DisplayRtcConfig() {"+    
    "            var RtcConfigDiv = document.getElementById('RtcTimeConfig');"+   
    "            RtcConfigDiv.style.display = (RtcConfigDiv.style.display === 'none' || RtcConfigDiv.style.display === '') ? 'block' : 'none';"+    
    "        }"+  
    // "        function HideRtcConfig() {"+    
    // "            var RtcConfigDiv = document.getElementById('RtcTimeConfig');"+   
    // "            RtcConfigDiv.style.display = 'none';"+    
    // "        }"+  
    "        function DisplayCycleDuration() {"+    
    "            var CycleBoxDiv = document.getElementById('CycleBox1').value;"+   
    "            var cycleDurationDiv = document.getElementById('CycleDuration');"+    
    "            cycleDurationDiv.style.display = (CycleBoxDiv === '1' || CycleBoxDiv === '2' || CycleBoxDiv === '3' || CycleBoxDiv === '4' ) ? 'block' : 'none';"+    
    "        }"+ 
    "        var refreshInterval = 400;"+                                     
    "        setInterval(upTime, refreshInterval);"+                        
    "        setInterval(DisplayCycleDuration, refreshInterval);"+                          
    "        setInterval(toggleCANFrame, refreshInterval);"+         
    "    </script>" +
    "    <div class=\"header\">"+
    "        <h1>ESP32-S3-RS485-CAN</h1>" + 
    "    </div>" + 
    "    <nav>" + 
    "        <a href=\"/\" id=\"SerialControlLink\" class=\"SerialControlActive\">Serial Control</a>" +  
    "        <a href=\"/RTC_Event\" id=\"rtcEventLink\" class=\"rtcEventActive\">RTC Event</a>" +  
    "    </nav>" + 
    "    <div class=\"container\">" +  
    "        <div class=\"form-group\">" + 
    "            <label for=\"Date\">Date:(example:2024/12/20)</label>" + 
    "            <input type=\"text\" id=\"DateBox1\" style=\"width: 50px;\" value=\"2024\">" + 
    "            <span>/</span>" + 
    "            <input type=\"text\" id=\"DateBox2\" style=\"width: 50px;\" value=\"12\">" + 
    "            <span>/</span>" + 
    "            <input type=\"text\" id=\"DateBox3\" style=\"width: 50px;\" value=\"20\">" + 
    "            <span>&nbsp;&nbsp;&nbsp;</span>" + 
    "            <select id=\"Week\" style=\"width: 150px;\">" + 
    "                <option value=\"1\">星期一(Monday)</option>" + 
    "                <option value=\"2\">星期二(Tuesday)</option>" + 
    "                <option value=\"3\">星期三(Wednesday)</option>" + 
    "                <option value=\"4\">星期四(Thursday)</option>" + 
    "                <option value=\"5\">星期五(Friday)</option>" + 
    "                <option value=\"6\">星期六(Saturday)</option>" + 
    "                <option value=\"0\">星期日(Sunday)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"Time\">Time:(example:16:51:21)</label>" + 
    "            <input type=\"text\" id=\"TimeBox1\" style=\"width: 50px;\" value=\"0\">" + 
    "            <span>:</span>" + 
    "            <input type=\"text\" id=\"TimeBox2\" style=\"width: 50px;\" value=\"0\">" + 
    "            <span>:</span>" + 
    "            <input type=\"text\" id=\"TimeBox3\" style=\"width: 50px;\" value=\"0\">" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"SerialBox\">SerialPort:</label>" +
    "            <select id=\"SerialPort\" style=\"width: 200px; text-align: left;\">" + 
    "                <option value=\"0\">RS485 发送(RS485 Send)</option>" + 
    "                <option value=\"1\">CAN 发送(CAN Send)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\"  id=\"CANFrame\" style=\"display:none;\">" + 
    "            <label for=\"CAN\">CAN frame information:</label>" + 
    "            <span>CAN ID :0x</span>" + 
    "            <input type=\"text\" id=\"CANid\" style=\"width: 160px; text-align: left;\" value=\"00000000\">" + 
    "            <select id=\"CANextd\" style=\"width: 200px; text-align: left;\">" + 
    "                <option value=\"0\">标准帧(Standard frames)</option>" + 
    "                <option value=\"1\">扩展帧(Extended frames)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"SendDate\">Date:(example:hello waveshare)</label>" + 
    "            <input type=\"text\" id=\"SerialData\" style=\"width: 500px; text-align: left;\" value=\"12\" oninput=\"handleSerialInput(this)\">" + 
    "            <span><br></span>" + 
    "            <select id=\"DataType\" style=\"width: 120px; text-align: left;\">" + 
    "                <option value=\"0\">字符串(char)</option>" + 
    "                <option value=\"1\">十六进制(hex)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"cycleBox\">Cycle:</label>" + 
    "            <div style=\"display: flex; align-items: center; gap: 10px; margin-top: 5px;\">" +
    "                <input type=\"text\" id=\"CycleDuration\" style=\"width: 100px; display:none;\" value=\"100\">" + 
    "                <select id=\"CycleBox1\" style=\"width: 150px; text-align: left;\">" + 
    "                    <option value=\"0\">无重复(Aperiodicity)</option>" + 
    "                    <option value=\"1\">毫秒(Milliseconds)</option>" +  
    "                    <option value=\"2\">秒(Seconds)</option>" + 
    "                    <option value=\"3\">分(Minutes)</option>" + 
    "                    <option value=\"4\">时(Hours)</option>" + 
    "                    <option value=\"5\">每天(Everyday)</option>" + 
    "                    <option value=\"6\">每周(Weekly)</option>" + 
    "                    <option value=\"7\">每月(Monthly)</option>" + 
    "                </select>" + 
    "            </div>" +
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <button class=\"btn\" id=\"NewEvent\" onclick=\"getRtcEventData()\">New Event</button>" + 
    "        </div>" + 
    "    </div>" + 
    "    <div class=\"container\"  id=\"RtcTimeConfig\" style=\"display:none;\">" +  
    "        <div class=\"form-group\">" + 
    "            <label for=\"Date\">Date:(example:2024/12/20)</label>" + 
    "            <input type=\"text\" id=\"RtcDateBox1\" style=\"width: 50px;\" value=\"2024\">" + 
    "            <span>/</span>" + 
    "            <input type=\"text\" id=\"RtcDateBox2\" style=\"width: 50px;\" value=\"12\">" + 
    "            <span>/</span>" + 
    "            <input type=\"text\" id=\"RtcDateBox3\" style=\"width: 50px;\" value=\"20\">" + 
    "            <span>&nbsp;&nbsp;&nbsp;</span>" + 
    "            <select id=\"RtcWeek\" style=\"width: 150px;\">" + 
    "                <option value=\"1\">星期一(Monday)</option>" + 
    "                <option value=\"2\">星期二(Tuesday)</option>" + 
    "                <option value=\"3\">星期三(Wednesday)</option>" + 
    "                <option value=\"4\">星期四(Thursday)</option>" + 
    "                <option value=\"5\">星期五(Friday)</option>" + 
    "                <option value=\"6\">星期六(Saturday)</option>" + 
    "                <option value=\"0\">星期日(Sunday)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"Time\">Time:(example:16:51:21)</label>" + 
    "            <input type=\"text\" id=\"RtcTimeBox1\" style=\"width: 50px;\" value=\"0\">" + 
    "            <span>:</span>" + 
    "            <input type=\"text\" id=\"RtcTimeBox2\" style=\"width: 50px;\" value=\"0\">" + 
    "            <span>:</span>" + 
    "            <input type=\"text\" id=\"RtcTimeBox3\" style=\"width: 50px;\" value=\"0\">" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <button class=\"btn\" id=\"UpDateTime\" style=\"margin-right: 50px;\" onclick=\"UpDataRtcTime()\">UpDate Time</button>" + 
    // "            <button class=\"btn\" id=\"HideConfig\" onclick=\"HideRtcConfig()\">Hide Config</button>" + 
    "        </div>" + 
    "    </div>" + 
    "    <div class=\"container\">" +  
    "        <div class=\"form-group\">" + 
    "            <span id=\"Time\" style=\"margin-right: 20px;\"></span> "+
    "            <button id=\"RtcConfig\" onclick=\"DisplayRtcConfig()\">RTC Config</button>" + 
    "        </div>" + 
    "        <div class=\"Events\">" + 
    "            <ul id=\"myList\"> "+
    "            </ul> "+
    "        </div>" + 
    "    </div>" + 
    "</body>" + 
    "</html>"; 

    server.send(200, "text/html", rtcPage);   
    printf("The user visited the RTC Event page\r\n"); 
}

String escapeJson(const char* input) {
  String output = "";
  while (*input) {
    char c = *input++;
    switch (c) {
      case '\"': output += "\\\""; break;
      case '\\': output += "\\\\"; break;
      case '\b': output += "\\b"; break;
      case '\f': output += "\\f"; break;
      case '\n': output += "\\n"; break;
      case '\r': output += "\\r"; break;
      case '\t': output += "\\t"; break;
      case '/':  output += "\\/"; break;
      default:
        if ((uint8_t)c <= 0x1F) {
          char buf[7];
          snprintf(buf, sizeof(buf), "\\u%04x", c);
          output += buf;
        } else {
          output += c;
        }
    }
  }
  return output;
}
void handleGetRateConfig() {
  // 构造 JSON 响应字符串
  String json = "{";
  json += "\"rs485_baud\": \"" + String(RS485_BaudRate) + "\",";
  json += "\"can_rate\": \"" + String(CAN_bitrate_kbps) + "\"";
  json += "}";

  // 发送响应
  server.send(200, "application/json", json);
}

void handleGetRS485Data() {
  if (RS485_Read_Data[0] == '\0') {
    // If empty, don't perform any operation and exit
    server.send(200, "application/json", "[]");  // Respond with an empty JSON array
    return;
  }
  // String safeString = String(RS485_Read_Data);
  String safeString = escapeJson(RS485_Read_Data);
  memset(RS485_Read_Data,0, RS485_Received_Len+1);
  RS485_Received_Len = 0;
  RS485_Read_Data[0] = '\0'; // This sets the first character to null, effectively clearing the array
  // safeString.replace("\\", "\\\\");
  // safeString.replace("\"", "\\\"");
  // safeString.replace("\n", "\\n");
  // safeString.replace("\r", "\\r");
  // safeString.replace("\t", "\\t");
  
  String json = "[\"" + safeString + "\"]";
  server.send(200, "application/json", json);
}
void handleGetCANData() {
  if (CAN_Read_Data[0] == '\0') {
    // If empty, don't perform any operation and exit
    server.send(200, "application/json", "[]");  // Respond with an empty JSON array
    return;
  }
  String safeString = String(CAN_Read_Data);
  memset(CAN_Read_Data,0, CAN_Received_Len+1);
  CAN_Received_Len = 0;
  CAN_Read_Data[0] = '\0'; // This sets the first character to null, effectively clearing the array
  safeString.replace("\\", "\\\\");
  safeString.replace("\"", "\\\"");
  safeString.replace("\n", "\\n");
  safeString.replace("\r", "\\r");
  safeString.replace("\t", "\\t");
  String json = "[\"" + safeString + "\"]";
  server.send(200, "application/json", json);
}

void handleRS485SetBaudRate() {
  char Text[1000];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);
  
  bool ret = ParseRS485BaudRateConfig(Text,&RS485_BaudRate);
  if(ret){
    RS485_UpdateBaudRate(RS485_BaudRate);
  }
  server.send(200, "text/plain", "OK");
}
void handleRS485SetConfig() {
  char Text[1000];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);
  
  ParseRS485Config(Text,&RS485_Read_Data_Type);
  server.send(200, "text/plain", "OK");
}
void handleRS485Send() {
  char Text[1000];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);
  RS485_Receive RS485Data;
  ParseRS485Data(Text, &RS485Data);
  SetData(RS485Data.Read_Data, RS485Data.DataLength);
  
  server.send(200, "text/plain", "OK");
}
void handleCANSetRate() {
  char Text[1000];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);
  
  bool ret = ParseCANRateConfig(Text,&CAN_bitrate_kbps);
  if(ret){
    CAN_UpdateRate(CAN_bitrate_kbps);
  }
  server.send(200, "text/plain", "OK");
}
void handleCANSend() {
  char Text[1000];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);
  CAN_Receive CANData;
  ParseCANData(Text, &CANData);
  
  twai_message_t Web_message;
  Web_message.identifier = CANData.CAN_ID ;
  Web_message.extd = CANData.CAN_extd;                              // Frame_type : 1：Extended frames   0：Standard frames
  Web_message.data_length_code = CANData.DataLength;                  // valueBytes[0] to valueBytes[6] are configuration parameters
  for (int i = 0; i < Web_message.data_length_code; i++) {
    Web_message.data[i] = CANData.Read_Data[i];
  }
  send_message_Bytes(Web_message);  
  server.send(200, "text/plain", "OK");
}

void handleNewEvent(void) {
  char Text[1000];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);  // Text=Date: 2024/12/20  Week: 0  Time: 0:0:0  Relay CH1: 0  Relay CH2: 2  Relay CH3: 2  Relay CH4: 2  Relay CH5: 2  Relay CH6: 2  Relay CH7: 2  Relay CH8: 2  Cycle: 0.
  datetime_t Event_Time;
  Repetition_event cycleEvent;
  Web_Receive SerialData;
  ParseRTCData(Text, &Event_Time, &SerialData, &cycleEvent);
  // Print decoded values
  // printf("Decoded datetime:\n");
  // printf("Year: %d, Month: %d, Day: %d, Week: %d\r\n", Event_Time.year, Event_Time.month, Event_Time.day, Event_Time.dotw);
  // printf("Time: %d:%d:%d\r\n", Event_Time.hour, Event_Time.minute, Event_Time.second);
  // printf("Relay States:\r\n");
  // for (int i = 0; i < 8; i++) {
  //   printf("Relay CH%d: %d\r\n", i + 1, Relay_n[i]);
  // }
  // printf("Cycle Event: %d\r\n", cycleEvent);
  if(Event_Time.month > 12 || Event_Time.day > 31 || Event_Time.dotw > 6 || Event_Time.month == 0 || Event_Time.day == 0)
    printf("Error parsing Event_Time !!!!\r\n");
  else if(Event_Time.hour > 23 || Event_Time.minute > 59 || Event_Time.second > 59 )
    printf("Error parsing Event_Time !!!!\r\n");
  else
    TimerEvent_Serial_Set(Event_Time, &SerialData, cycleEvent);
}

void handleSetRtcTime(void) {
  char Text[200];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);  // Text=Date: 2024/12/20  Week: 0  Time: 0:0:0  Relay CH1: 0  Relay CH2: 2  Relay CH3: 2  Relay CH4: 2  Relay CH5: 2  Relay CH6: 2  Relay CH7: 2  Relay CH8: 2  Cycle: 0.
  datetime_t Rtc_Time;
  ParseRtcConfig(Text, &Rtc_Time);
  // Print decoded values
  printf("Decoded datetime:\n");
  printf("Year: %d, Month: %d, Day: %d, Week: %d\r\n", Rtc_Time.year, Rtc_Time.month, Rtc_Time.day, Rtc_Time.dotw);
  printf("Time: %d:%d:%d\r\n", Rtc_Time.hour, Rtc_Time.minute, Rtc_Time.second);
  if(Rtc_Time.month > 12 || Rtc_Time.day > 31 || Rtc_Time.dotw > 6 || Rtc_Time.month == 0 || Rtc_Time.day == 0)
    printf("Error parsing Rtc_Time !!!!\r\n");
  else if(Rtc_Time.hour > 23 || Rtc_Time.minute > 59 || Rtc_Time.second > 59 )
    printf("Error parsing RTC Time !!!!\r\n");
  else
    PCF85063_Set_All(Rtc_Time);
}

void handleUpTimeAndEvent() {
  // Format the datetime string
  char datetime_str[50];
  sprintf(datetime_str, " %d/%d/%d  %s  %d:%d:%d", datetime.year, datetime.month, datetime.day, Week[datetime.dotw], datetime.hour, datetime.minute, datetime.second);

  int eventCount = Timing_events_Num;  // Get the event count (e.g., Timing_events_Num)

  // Create a JSON response
  String jsonResponse = "{";
  
  jsonResponse += "\"time\":\"" + String(datetime_str) + "\",";
  for (int i = 0; i < eventCount; i++) {
    jsonResponse += "\"eventStr" + String(i + 1) + "\":\"" + String(Event_str[i]) + "\",";
  }
  jsonResponse += "\"eventCount\":" + String(eventCount);
  jsonResponse += "}";
  // Send the datetime string as a response
  server.send(200, "text/plain", jsonResponse );
}
void handleDeleteEvent() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id > 0) {
      TimerEvent_Del_Number((uint8_t)id);
      server.send(200, "text/plain", "Event " + String(id) + " deleted.");
      printf("Event %d deleted.\r\n", id);
    } else {
      server.send(400, "text/plain", "Invalid event ID.");
    }
  } else {
    server.send(400, "text/plain", "Event ID not provided.");
  }
}


void WIFI_Init()
{
  WiFi.mode(WIFI_AP);                             
  // WiFi.setSleep(true);    
  while(!WiFi.softAP(ssid, password)) {
    printf("Soft AP creation failed.\r\n");
    printf("Try setting up the WIFI again.\r\n");
  } 
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // Set the IP address and gateway of the AP
  delay(100);  
  
  IPAddress myIP = WiFi.softAPIP();
  uint32_t ipAddress = WiFi.softAPIP();
  printf("AP IP address: ");
  sprintf(ipStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
  printf("%s\r\n", ipStr);
      
  server.on("/", handleRoot);            // Relay Control page
  server.on("/getRateConfig"     , handleGetRateConfig);
  server.on("/RS485SetBaudRate" , handleRS485SetBaudRate);
  server.on("/RS485SetConfig"   , handleRS485SetConfig);
  server.on("/RS485Send"        , handleRS485Send);
  server.on("/getRS485Data"     , handleGetRS485Data);
  server.on("/CANSetRate"       , handleCANSetRate);
  server.on("/CANSend"          , handleCANSend);
  server.on("/getCANData"       , handleGetCANData);
  
  server.on("/RTC_Event"        , handleRTCPage);      // RTC Event page
  server.on("/NewEvent"         , handleNewEvent);
  server.on("/SetRtcTime"       , handleSetRtcTime);
  server.on("/getTimeAndEvent"  , handleUpTimeAndEvent);
  server.on("/DeleteEvent"      , handleDeleteEvent);
  
  server.begin(); 
  printf("Web server started\r\n");  
  xTaskCreatePinnedToCore(
    WebTask,    
    "WebTask",   
    4096,                
    NULL,                 
    4,                   
    NULL,                 
    0                   
  );
}


void WebTask(void *parameter) {
  while(1){
    server.handleClient(); // Processing requests from clients
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  vTaskDelete(NULL);
}

int hexCharToByte(char high, char low) {
    int highValue = (high >= '0' && high <= '9') ? (high - '0') : (high >= 'A' && high <= 'F') ? (high - 'A' + 10) : (high - 'a' + 10);
    int lowValue = (low >= '0' && low <= '9') ? (low - '0') : (low >= 'A' && low <= 'F') ? (low - 'A' + 10) : (low - 'a' + 10);
    return (highValue << 4) | lowValue;  // 将高4位和低4位合并为一个字节
}
// String decoding
bool ParseRTCData(const char* Text, datetime_t* dt, Web_Receive* SerialData, Repetition_event* cycleEvent) {    
  int ret;
  // Parse Date: YYYY/MM/DD
  ret = sscanf(Text, "Date: %hd/%hhd/%hhd", &dt->year, &dt->month, &dt->day);
  if (ret != 3) {
    printf("Error parsing date\n");
    return false;
  }
  // Parse Week: W (day of the week)
  ret = sscanf(strstr(Text, "Week: "), "Week: %hhd", &dt->dotw);
  if (ret != 1) {
    printf("Error parsing week\n");
    return false;
  }
  // Parse Time: HH:MM:SS
  ret = sscanf(strstr(Text, "Time: "), "Time: %hhd:%hhd:%hhd", &dt->hour, &dt->minute, &dt->second);
  if (ret != 3) {
    printf("Error parsing time\n");
    return false;
  }
  // Parse Serial Port: RS485/CAN
  ret = sscanf(strstr(Text, "Serial Port: "), "Serial Port: %hhu", &SerialData->SerialPort);
  if (ret != 1) {
    printf("Error parsing Serial Port\n");
    return false;
  }
  
  if(SerialData->SerialPort){                      // is CAN
    ret = sscanf(strstr(Text, "CAN ID: "), "CAN ID: 0x%lx", &SerialData->CAN_ID);
    if (ret != 1) {
      printf("Error parsing CAN ID\n");
      return false;
    }
    if(SerialData->CAN_ID > 0x1FFFFFFF) {
      printf("CAN ID error:%lX\n",SerialData->CAN_ID);
      return false;
    }
    ret = sscanf(strstr(Text, "CAN Extd: "), "CAN Extd: %hhu", &SerialData->CAN_extd);
    if (ret != 1) {
      printf("Error parsing CAN Extd\n");
      return false;
    }
  }
  // Parse Serial Data Type: char / hex
  ret = sscanf(strstr(Text, "Data Type: "), "Data Type: %hhu", &SerialData->DataType);
  if (ret != 1) {
    printf("Error parsing Serial Type\n");
    return false;
  }


  // Parse Serial Data Length: 
  const char* start = strstr(Text, "Serial Data: ");
  if (!start) {
    printf("Serial Data not found\n");
    return false;
  }
  start += strlen("Serial Data: ");
  const char* end = strstr(start, "  Data Type: ");
  if (!end) {
    printf("Data Type not found\n");
    return false;
  }
  SerialData->DataLength = end - start;

  if(SerialData->DataType){
    bool missing = 0;
    char* cleanHex = (char*)malloc(SerialData->DataLength + 1);  
    strncpy(cleanHex, start, SerialData->DataLength);
    cleanHex[SerialData->DataLength] = '\0';  
    char* src = cleanHex;
    char* dst = cleanHex;
    while (*src) {
      if (*src != ' ') {
        *dst++ = *src;
      }
      src++;
    }
    *dst = '\0'; 

    SerialData->DataLength = strlen(cleanHex);
    if (SerialData->DataLength % 2) {
      SerialData->DataLength += 1;
      missing = 1;
    }
    SerialData->DataLength /= 2;
    SerialData->SerialData = (uint8_t*)malloc(SerialData->DataLength);
    if (missing) {
      for (size_t i = 0; i < SerialData->DataLength - 1; i++) {
        SerialData->SerialData[i] = hexCharToByte(cleanHex[2 * i], cleanHex[2 * i + 1]);
      }
      SerialData->SerialData[SerialData->DataLength - 1] = hexCharToByte(cleanHex[2 * (SerialData->DataLength - 1)], '0');
    } 
    else {
      for (size_t i = 0; i < SerialData->DataLength; i++) {
        SerialData->SerialData[i] = hexCharToByte(cleanHex[2 * i], cleanHex[2 * i + 1]);
      }
    }
    free(cleanHex);
  }
  else{
    // Parse actual Serial Data (e.g., char or hex values)
    SerialData->SerialData = (uint8_t *)malloc(SerialData->DataLength);
    memcpy(SerialData->SerialData, start, SerialData->DataLength);
  }
  // Parse Cycle: C
  uint8_t cycle;
  ret = sscanf(strstr(Text, "Cycle: "), "Cycle: %hhd",&cycle);
  if (ret == 1) {
    *cycleEvent = (Repetition_event)cycle;
  }
  else{
    printf("Error parsing cycle\n");
    return false;
  }
  if(*cycleEvent == Repetition_Hours || *cycleEvent == Repetition_Minutes || *cycleEvent == Repetition_Seconds || *cycleEvent == Repetition_Milliseconds){
    ret = sscanf(strstr(Text, "Cycle Duration: "), "Cycle Duration: %ld",&(SerialData->repetition_Time[0]));
    if (ret != 1) {
      printf("Error parsing Cycle Duration\n");
      return false;
    }
    switch(*cycleEvent){
      case Repetition_Hours: 
        if(4294967295 > SerialData->repetition_Time[0] * 1000 * 3600)
          SerialData->repetition_Time[0] = SerialData->repetition_Time[0] * 1000 * 3600;
        else
          SerialData->repetition_Time[0] = 0;
        break;
      case Repetition_Minutes: 
        if(4294967295 > SerialData->repetition_Time[0] * 1000 * 60)
          SerialData->repetition_Time[0] = SerialData->repetition_Time[0] * 1000 * 60;
        else
          SerialData->repetition_Time[0] = 0;
        break;
      case Repetition_Seconds: 
        if(4294967295 > SerialData->repetition_Time[0] * 1000)
          SerialData->repetition_Time[0] = SerialData->repetition_Time[0] * 1000;
        else
          SerialData->repetition_Time[0] = 0;
        break;
      case Repetition_Milliseconds: 
        if(4294967295 > SerialData->repetition_Time[0])
          SerialData->repetition_Time[0] = SerialData->repetition_Time[0];
        else
          SerialData->repetition_Time[0] = 0;
        break;
      default:
        printf("Event error!!!!\n");
        break;
    }
  }
  return true;
}
// String decoding
bool ParseRtcConfig(const char* Text, datetime_t* dt) {    
  int ret;
  // Parse Date: YYYY/MM/DD
  ret = sscanf(Text, "Date: %hd/%hhd/%hhd", &dt->year, &dt->month, &dt->day);
  if (ret != 3) {
    printf("Error parsing date\n");
    return false;
  }
  // Parse Week: W (day of the week)
  ret = sscanf(strstr(Text, "Week: "), "Week: %hhd", &dt->dotw);
  if (ret != 1) {
    printf("Error parsing week\n");
    return false;
  }
  // Parse Time: HH:MM:SS
  ret = sscanf(strstr(Text, "Time: "), "Time: %hhd:%hhd:%hhd", &dt->hour, &dt->minute, &dt->second);
  if (ret != 3) {
    printf("Error parsing time\n");
    return false;
  }
  return true;
}


// String decoding
bool ParseRS485BaudRateConfig(const char* Text,  unsigned long * RS485_BaudRate) {    
  int ret;
  // Parse Serial Data Type: char / hex
  ret = sscanf(strstr(Text, "RS485 BaudRate: "), "RS485 BaudRate: %lu", RS485_BaudRate);
  if (ret != 1) {
    printf("Error parsing RS485 Read Type\n");
    return false;
  }
  return true;
}
// String decoding
bool ParseRS485Config(const char* Text,uint8_t* RS485_Read_Data_Type) {    
  int ret;
  // Parse Serial Data Type: char / hex
  ret = sscanf(strstr(Text, "Data Type: "), "Data Type: %hhu", RS485_Read_Data_Type);
  if (ret != 1) {
    printf("Error parsing RS485 Read Type\n");
    return false;
  }
  return true;
}
// String decoding
bool ParseRS485Data(const char* Text, RS485_Receive* RS485Data) {    
  int ret;
  // Parse Serial Data Type: char / hex
  ret = sscanf(strstr(Text, "Data Type: "), "Data Type: %hhu", &RS485Data->DataType);
  if (ret != 1) {
    printf("Error parsing Serial Type\n");
    return false;
  }
  
  // Parse Serial Data Length: 
  const char* start = strstr(Text, "RS485 Data: ");
  if (!start) {
    printf("RS485 Data not found\n");
    return false;
  }
  start += strlen("RS485 Data: ");
  const char* end = strstr(start, "  Web End");
  if (!end) {
    printf("Data Type not found\n");
    return false;
  }
  RS485Data->DataLength = end - start;

  if(RS485Data->DataType){
    size_t rawLen = end - start;
    char* cleanHex = (char*)malloc(rawLen + 1); 
    strncpy(cleanHex, start, rawLen);
    cleanHex[rawLen] = '\0';

    char* src = cleanHex;
    char* dst = cleanHex;
    while (*src) {
      if (*src != ' ') {
          *dst++ = *src;
      }
      src++;
    }
    *dst = '\0'; 
    size_t hexLen = strlen(cleanHex);
    bool missing = 0;
    if (hexLen % 2) {
      hexLen += 1;
      missing = 1;
    }
    RS485Data->DataLength = hexLen / 2;
    RS485Data->Read_Data = (uint8_t*)malloc(RS485Data->DataLength);
    if (missing) {
      for (size_t i = 0; i < RS485Data->DataLength - 1; i++) {
        RS485Data->Read_Data[i] = hexCharToByte(cleanHex[2 * i], cleanHex[2 * i + 1]);
      }
      RS485Data->Read_Data[RS485Data->DataLength - 1] = hexCharToByte(cleanHex[2 * (RS485Data->DataLength - 1)], '0');
    } 
    else {
      for (size_t i = 0; i < RS485Data->DataLength; i++) {
        RS485Data->Read_Data[i] = hexCharToByte(cleanHex[2 * i], cleanHex[2 * i + 1]);
      }
    }
    free(cleanHex);  
  }
  else{
    // Parse actual Serial Data (e.g., char or hex values)
    RS485Data->Read_Data = (uint8_t *)malloc(RS485Data->DataLength);
    memcpy(RS485Data->Read_Data, start, RS485Data->DataLength);
  }
  return true;
}
// String decoding
bool ParseCANRateConfig(const char* Text,  uint32_t * CAN_bitrate_kbps) {    
  int ret;
  // Parse Serial Data Type: char / hex
  ret = sscanf(strstr(Text, "CAN Rate: "), "CAN Rate: %lu", CAN_bitrate_kbps);
  if (ret != 1) {
    printf("Error parsing RS485 Read Type\n");
    return false;
  }
  return true;
}
// String decoding
bool ParseCANData(const char* Text, CAN_Receive* CANData) {    
  int ret;

  ret = sscanf(strstr(Text, "CAN ID: "), "CAN ID: 0x%lx", &CANData->CAN_ID);
  if (ret != 1) {
    printf("Error parsing CAN ID\n");
    return false;
  }
  if(CANData->CAN_ID > 0x1FFFFFFF) {
    printf("CAN ID error:%lX\n",CANData->CAN_ID);
    return false;
  }
  ret = sscanf(strstr(Text, "CAN Extd: "), "CAN Extd: %hhu", &CANData->CAN_extd);
  if (ret != 1) {
    printf("Error parsing CAN Extd\n");
    return false;
  }
  
  // Parse Serial Data Length: 
  const char* start = strstr(Text, "CAN Data: ");
  if (!start) {
    printf("CAN Data not found\n");
    return false;
  }
  start += strlen("CAN Data: ");
  const char* end = strstr(start, "  Web End");
  if (!end) {
    printf("Data Type not found\n");
    return false;
  }
  
  size_t dataLength = end - start;
  bool missing = 0;
  char* cleanHex = (char*)malloc(dataLength + 1);  
  strncpy(cleanHex, start, dataLength);
  cleanHex[dataLength] = '\0';  

  char* src = cleanHex;
  char* dst = cleanHex;
  while (*src) {
    if (*src != ' ') {
      *dst++ = *src;
    }
    src++;
  }
  *dst = '\0';  

  CANData->DataLength = strlen(cleanHex);
  if (CANData->DataLength % 2) {
    CANData->DataLength += 1;
    missing = 1;
  }
  CANData->DataLength /= 2;
  CANData->Read_Data = (uint8_t*)malloc(CANData->DataLength);
  if (missing) {
    for (size_t i = 0; i < CANData->DataLength - 1; i++) {
        CANData->Read_Data[i] = hexCharToByte(cleanHex[2 * i], cleanHex[2 * i + 1]);
    }
    CANData->Read_Data[CANData->DataLength - 1] = hexCharToByte(cleanHex[2 * (CANData->DataLength - 1)], '0');
  } else {
    for (size_t i = 0; i < CANData->DataLength; i++) {
        CANData->Read_Data[i] = hexCharToByte(cleanHex[2 * i], cleanHex[2 * i + 1]);
    }
  }
  free(cleanHex);
  return true;
}

