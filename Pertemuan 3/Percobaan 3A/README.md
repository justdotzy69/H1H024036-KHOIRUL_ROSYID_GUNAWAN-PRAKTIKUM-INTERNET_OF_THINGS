# Pertanyaan Praktikum 3.5.4

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036


---

**1. Gambarkan diagram alur (flowchart) proses pengiriman data melalui HTTP POST pada program di atas!**

Jawaban:

<img width="523" height="1575" alt="MODUL 3 drawio" src="https://github.com/user-attachments/assets/c639f0e6-4e0e-48d0-a9ff-bbf7eef82f4b" />



**2.Apa fungsi dari perintah http.addHeader("Content-Type", "application/json") pada program tersebut?**

Jawaban:

Perintah ini berfungsi untuk menyisipkan informasi tambahan (header) ke dalam paket HTTP yang dikirimkan ke server. Tujuannya adalah memberi tahu server pengolah pesan bahwa format atau tipe konten data yang dikirim (body) terstruktur dalam format JSON, sehingga server tidak salah membaca data. 

---

**3. Jelaskan arti dari kode response HTTP 200 dan sebutkan salah satu contoh kode response HTTP lain beserta artinya!**

Jawaban:

Kode HTTP 200 OK menandakan bahwa request atau permintaan yang dikirimkan oleh klien (ESP8266) telah berhasil diterima, dipahami, dan diproses tanpa masalah oleh server. Contoh kode lainnya adalah 404 Not Found (halaman/alamat endpoint tidak ditemukan di server) atau 500 Internal Server Error (terjadi kesalahan sistem di sisi server).

---

**4.Modifikasi program agar ESP32 dapat mengirimkan data tambahan berupa waktu(dalam milidetik sejak dinyalakan menggunakan millis()) ke dalam JSON yang dikirim,dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentukREADME.md**

Jawaban:
```cpp
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
