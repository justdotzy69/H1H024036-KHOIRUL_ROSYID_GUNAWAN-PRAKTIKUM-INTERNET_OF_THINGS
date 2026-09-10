void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
    digitalWrite(ledPin, HIGH); 
  } else {
    Serial.println("Status: Terputus. Mencoba menghubungkan kembali...");
    digitalWrite(ledPin, LOW);
    
    // Perintah untuk memutus total koneksi lama lalu menyambung ulang
    WiFi.disconnect();
    WiFi.reconnect();
  }
  delay(5000);
}