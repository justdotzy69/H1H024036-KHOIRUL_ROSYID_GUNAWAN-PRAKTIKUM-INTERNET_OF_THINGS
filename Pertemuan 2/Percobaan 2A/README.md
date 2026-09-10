# Pertanyaan Praktikum 2.5.4

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036


---

**1. Gambarkan diagram alur (flowchart) proses koneksi ESP32 ke jaringan WiFi pada program di atas!**

Jawaban:

<img width="679" height="1218" alt="FLowchartModul2 drawio" src="https://github.com/user-attachments/assets/6a7e8283-b993-480e-98cc-474de347cbf3" />


**2. Apa fungsi dari perintah `WiFi.mode(WIFI_STA)` pada program tersebut?**

Jawaban:

Perintah ini berfungsi untuk mengatur mode operasi WiFi pada mikrokontroler agar berperan sebagai klien (Station). Dengan mode ini, ESP32 dikonfigurasi untuk terhubung ke sebuah jaringan WiFi yang sudah ada (seperti router atau hotspot dari smartphone), sama halnya seperti laptop yang tersambung ke jaringan WiFi. 

---

**3. Jelaskan apa yang terjadi apabila SSID atau password yang dimasukkan salah!**

Jawaban:

Program akan terjebak di dalam perulangan while (WiFi.status() != WL_CONNECTED). ESP32 akan terus-menerus mencoba melakukan koneksi yang gagal dan Serial Monitor hanya akan mencetak karakter titik (".") secara terus-menerus tanpa henti. Program di dalam fungsi setup() tidak akan pernah selesai, sehingga IP address tidak akan muncul dan LED indikator tidak akan pernah menyala.

---

**4. Modifikasi program agar ESP32 mencoba menghubungkan ulang (reconnect) secara otomatis apabila koneksi WiFi terputus, dan berikan penjelasan di setiap baris kode yang ditambahkan!**

Jawaban:
```cpp
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
    digitalWrite(ledPin, HIGH); 
  } else {
    Serial.println("Status: Terputus. Mencoba menghubungkan kembali...");
    digitalWrite(ledPin, LOW);
    
    // Memutus koneksi lama dan menyambung ulang
    WiFi.disconnect();
    WiFi.reconnect();
  }
  delay(5000);
}
