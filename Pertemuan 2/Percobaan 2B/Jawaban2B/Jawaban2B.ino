#include <WiFi.h>

const char* ssid_rumah = "WIFI_RUMAH_ANDA";
const char* password_rumah = "PASSWORD_WIFI_RUMAH";

const char* ap_ssid = "ESP32_AP_STA";
const char* ap_password = "passwordkuat123";

void setup() {
  Serial.begin(115200);

  // Set mode menjadi gabungan Access Point dan Station
  WiFi.mode(WIFI_AP_STA);

  // 1. Konfigurasi Station (Terhubung ke WiFi Rumah)
  WiFi.begin(ssid_rumah, password_rumah);
  Serial.print("Menghubungkan ke WiFi Rumah");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi Rumah! IP: " + WiFi.localIP().toString());

  // 2. Konfigurasi Access Point (Menyediakan Jaringan Sendiri)
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access Point Aktif! IP: " + WiFi.softAPIP().toString());
}

void loop() {
  // Biarkan kosong atau tambahkan kode lain sesuai kebutuhan
}