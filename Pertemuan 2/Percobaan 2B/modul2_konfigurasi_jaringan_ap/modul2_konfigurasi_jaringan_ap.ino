
#include <WiFi.h>

// Menyiapkan nama jaringan (SSID) yang akan dipancarkan/dibuat oleh ESP32
const char* ap_ssid = "ESP32_Access Point";

const char* ap_password = "12345678"; 

void setup() {
  // Membuka jalur komunikasi serial ke Serial Monitor dengan kecepatan 115200 
  Serial.begin(115200);

  // Mengubah mode operasi WiFi ESP32 menjadi mode Access Point (AP) agar berfungsi sebagai penyedia jaringan 
  WiFi.mode(WIFI_AP);

  // Mengaktifkan pemancar jaringan pada ESP32 menggunakan nama SSID dan password yang telah kita tentukan di atas
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress apIP = WiFi.softAPIP();

  // Mencetak informasi ke Serial Monitor bahwa mode Access Point telah sukses dijalankan
  Serial.println("Access Point aktif!");
  Serial.print("SSID : ");
  Serial.println(ap_ssid); // Menampilkan nama WiFi ESP32
  
  Serial.print("IP Address: ");
  Serial.println(apIP); // Menampilkan IP Address dari perangkat pemancar (ESP32)
}

void loop() {

  // Menggunakan fungsi bawaan untuk menghitung jumlah total perangkat klien (misal smartphone/laptop) yang sedang terhubung ke ESP32
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  // Mencetak angka jumlah klien tersebut ke Serial Monitor
  Serial.println(jumlahClient);

  // Memberikan jeda selama 5000 milidetik (5 detik) 
  delay(5000);
}