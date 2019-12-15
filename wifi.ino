
void wifi_setup() {
  int cnt = 10;

  WiFi.setSleepMode(WIFI_NONE_SLEEP); // or WIFI_MODEM_SLEEP also does not drop out (?)
  WiFi.mode(WIFI_STA);
  WiFi.setOutputPower(17);        // 10dBm == 10mW, 14dBm = 25mW, 17dBm = 50mW, 20dBm = 100mW
  WiFi.setAutoReconnect(true);

  WiFi.begin(ssid, password);
  delay(2000);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("wifi failed\n");
    //WiFi.disconnect(false);
    //    delay(1000);
    //    WiFi.connect(true);
    //WiFi.begin(ssid, password);
    delay(2000);
    if (--cnt == 0) ESP.restart();
  }


  MDNS.begin(hostName);

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  WiFi.printDiag(Serial);

}
