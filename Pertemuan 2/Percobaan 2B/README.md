# Pertanyaan Praktikum 2.6.4

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036


---

**1. Mengapa alamat IP default Access Point pada ESP32 umumnya bernilai 192.168.4.1?**

Jawaban:

Alamat IP 192.168.4.1 merupakan alamat default yang sudah ditetapkan (bawaan pabrik) dari library ESP32 untuk mode Access Point. Alamat IP ini digunakan karena termasuk ke dalam blok rentang IP privat kelas C yang memang dialokasikan khusus untuk jaringan lokal (Local Area Network) dan tidak bisa dialihkan ke internet publik, sehingga aman untuk membuat jaringan hotspot mandiri agar tidak bentrok dengan IP publik.

---

**2. Apa perbedaan mendasar antara mode Station dan mode Access Point pada ESP32?**

Jawaban:

Pada mode Station (STA), ESP32 berfungsi sebagai klien (penerima) yang harus terhubung dan menumpang pada jaringan WiFi eksternal yang sudah ada sebelumnya. Sebaliknya, pada mode Access Point (AP), ESP32 justru berfungsi sebagai penyedia jaringan (pemancar/hotspot) mandiri, di mana perangkat lain seperti smartphone yang akan datang dan meminta koneksi ke ESP32 tersebut tanpa memerlukan bantuan router tambahan. 

---

**3. Jelaskan risiko keamanan apabila password Access Point tidak diberikan atau terlalu sederhana!**

Jawaban:

Jika kata sandi tidak ada atau terlalu sederhana (seperti "12345678" atau "password"), siapa pun yang berada di radius pancaran sinyal ESP32 dapat dengan mudah terhubung ke jaringan tersebut. Risiko keamanannya meliputi penyadapan data sensor yang dikirim, eksploitasi perintah untuk mengontrol aktuator (misal: mematikan/menyalakan perangkat IoT secara ilegal), membebani kerja prosesor ESP32 dengan banyaknya permintaan klien, hingga mengubah kredensial sistem secara keseluruhan. 

---

**4.Modifikasi program agar ESP32 berjalan pada mode AP+STA (terhubung ke WiFi rumah sekaligus menyediakan Access Point), dan berikan penjelasan di setiap baris kode nya dalam bentuk README.md!**

Jawaban:
```cpp
#include <WiFi.h>

const char* ssid_rumah = "WIFI_RUMAH";
const char* password_rumah = "PASS_RUMAH";

const char* ap_ssid = "ESP32_AP_STA";
const char* ap_password = "passwordkuat";

void setup() {
  Serial.begin(115200);

  // Set mode menjadi gabungan Access Point dan Station
  WiFi.mode(WIFI_AP_STA);

  // 1. Konfigurasi Station (Klien)
  WiFi.begin(ssid_rumah, password_rumah);
  Serial.print("Menghubungkan ke WiFi Rumah");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung! IP STA: " + WiFi.localIP().toString());

  // 2. Konfigurasi Access Point (Hotspot)
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access Point Aktif! IP AP: " + WiFi.softAPIP().toString());
}

void loop() {
  // Kosong
}
