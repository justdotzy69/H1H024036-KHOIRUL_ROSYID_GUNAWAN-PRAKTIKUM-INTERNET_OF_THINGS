#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "MosoZydraGyrottaZao";
const char* password = "stone park";
const char* serverUrl = "http://httpbin.org/post";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi berhasil terhubung!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverUrl); // Memulai koneksi HTTP ke server
    http.addHeader("Content-Type", "application/json"); // Deklarasi tipe konten JSON
    
    // Membuat objek JSON berisi suhu, kelembaban, dan waktu (millis)
    JsonDocument doc;
    doc["suhu"] = 28.5;       
    doc["kelembaban"] = 65.0; 
    doc["waktu_ms"] = millis(); // MODIFIKASI: Menambahkan waktu aktif alat
    
    String requestBody;
    serializeJson(doc, requestBody); // Konversi objek JSON menjadi teks
    
    Serial.print("Mengirim data: ");
    Serial.println(requestBody);
    
    int httpResponseCode = http.POST(requestBody); // Mengirim data HTTP POST
    
    if (httpResponseCode > 0) {
      Serial.print("Kode Response HTTP: ");
      Serial.println(httpResponseCode);
      Serial.println("Isi Response:\n" + http.getString());
    } else {
      Serial.println("Pengiriman gagal, kode error: " + String(httpResponseCode));
    }
    http.end(); // Menutup koneksi
  }
  delay(10000); 
}