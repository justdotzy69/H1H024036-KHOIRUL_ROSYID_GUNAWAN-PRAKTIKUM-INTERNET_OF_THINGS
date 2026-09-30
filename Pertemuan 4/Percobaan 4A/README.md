# Pertanyaan Praktikum 4.5.4

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036


---

**1. Mengapa penggunaan delay() yang lama sebaiknya dihindari pada program yang menggabungkan proses publish dan subscribe secara bersamaan!**

Jawaban:


<img width="1038" height="1728" alt="FLowchart 4A drawio" src="https://github.com/user-attachments/assets/300423b6-dc2f-4a50-b2ef-d71592d48bab" />




**2.Jelaskan cara kerja mekanisme non-blocking menggunakan fungsi millis() pada program di atas**

Jawaban:


millis() mengembalikan waktu dalam milidetik sejak board menyala. Program menyimpan waktu publish terakhir pada waktuTerakhirPublish. Pada setiap iterasi loop(), selisih millis() - waktuTerakhirPublish dibandingkan dengan intervalPublish (5000 ms). Jika selisih sudah melebihi interval, program membaca sensor, mem-publish data, dan memperbarui waktuTerakhirPublish. Jika belum, program langsung melanjutkan iterasi tanpa menunggu, sehingga client.loop() terus dipanggil dan pesan masuk tetap diproses.

---

**3. Apa yang akan terjadi apabila fungsi client.loop() jarang dipanggil (misalnya hanya sekali setiap 10 detik)?**

Jawaban:

Pesan masuk dari broker baru diproses setiap 10 detik sehingga respons LED terlambat. Selain itu, keep-alive ke broker tidak terkirim tepat waktu; karena nilai keep-alive bawaan PubSubClient adalah 15 detik, koneksi rawan diputus broker dan ESP32 harus reconnect. Pada saat terputus, perintah yang dikirim dapat hilang karena tidak ada pengiriman ulang pada QoS 0.

---

**4. Modifikasi program agar menambahkan satu topic perintah baru untuk mengendalikan aktuator kedua (misalnya buzzer), dengan fungsi callback yang dapat membedakan topic mana yang menerima pesan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md!**

Jawaban:
```cpp
#include <ESP8266WiFi.h>           // Library untuk koneksi WiFi
#include <PubSubClient.h>   // Library untuk komunikasi MQTT
#include <ArduinoJson.h>    // Library untuk parsing JSON

// ==================== KONFIGURASI WIFI ====================
const char* ssid = "MosoZydraGyrottaZao";          // Ganti dengan SSID WiFi Anda
const char* password = "stone park";  // Ganti dengan password WiFi Anda

// ==================== KONFIGURASI MQTT ====================
const char* mqttServer = "broker.hivemq.com";  // Broker MQTT publik
const int mqttPort = 1883;                      // Port MQTT standar
const char* topicPerintah = "unsoed/tk245004/kelompok1/perintah";  // Topic untuk menerima perintah

// ==================== KONFIGURASI PIN ====================
const int ledPin = 4;  // GPIO 4 untuk LED indikator

// ==================== MODIFIKASI: KONFIGURASI PWM ====================
// ESP8266 hanya punya 1 channel PWM (0), dengan range 0-1023 (10-bit)
const int pwmChannel = 0;         // Channel PWM ESP8266 (hanya 0)
const int pwmFrequency = 5000;    // Frekuensi PWM 5 kHz
const int pwmRange = 1023;        // Range PWM 10-bit (0-1023)

// ==================== OBJEK GLOBAL ====================
WiFiClient espClient;           // Objek client WiFi
PubSubClient client(espClient); // Objek client MQTT

// ==================== FUNGSI CALLBACK ====================
// Fungsi ini dipanggil secara otomatis setiap ada pesan baru masuk
// pada topic yang di-subscribe
void callback(char* topic, byte* payload, unsigned int length) {
    String pesan;
    
    // Mengubah payload (byte array) menjadi String
    for (unsigned int i = 0; i < length; i++) {
        pesan += (char)payload[i];
    }
    
    // Menampilkan pesan mentah yang diterima
    Serial.print("Pesan diterima [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(pesan);
    
    // ==================== DESERIALISASI JSON ====================
    JsonDocument doc;  // Membuat objek JSON document
    DeserializationError error = deserializeJson(doc, pesan);  // Parsing JSON
    
    // Cek apakah parsing berhasil
    if (error) {
        Serial.print("Gagal parsing JSON: ");
        Serial.println(error.c_str());
        return;  // Keluar dari fungsi jika parsing gagal
    }
    
    // Mengambil nilai "perintah" dari JSON
    const char* perintah = doc["perintah"];
    
    // ==================== MODIFIKASI: KONTROL AKTUATOR DENGAN PWM ====================
    if (String(perintah) == "ON") {
        // Ambil nilai intensitas, default 1023 jika tidak ada
        // Operator | adalah default value operator dari ArduinoJson
        int intensitas = doc["intensitas"] | 1023;
        
        // Batasi nilai 0-1023 (range PWM ESP8266)
        intensitas = constrain(intensitas, 0, 1023);
        
        // Set duty cycle PWM
        analogWrite(ledPin, intensitas);
        // analogWrite() di ESP8266 otomatis menggunakan PWM
        // 0 = mati, 1023 = paling terang
        
        Serial.print("Aktuator: ON, Intensitas: ");
        Serial.println(intensitas);
        
    } else if (String(perintah) == "OFF") {
        analogWrite(ledPin, 0);   // Matikan LED dengan duty cycle 0
        Serial.println("Aktuator: OFF");
    }
}

// ==================== FUNGSI KONEKSI WIFI ====================
void hubungkanWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Menghubungkan ke WiFi");
    
    // Tunggu sampai terhubung
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi berhasil terhubung!");
}

// ==================== FUNGSI KONEKSI MQTT ====================
void hubungkanMQTT() {
    // Loop sampai berhasil terhubung ke broker
    while (!client.connected()) {
        Serial.print("Menghubungkan ke broker MQTT...");
        
        // Membuat client ID unik menggunakan random
        String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            Serial.println("berhasil terhubung!");
            
            // Subscribe ke topic perintah setelah berhasil terhubung
            client.subscribe(topicPerintah);
            Serial.print("Subscribe ke topic: ");
            Serial.println(topicPerintah);
        } else {
            Serial.print("gagal, rc=");
            Serial.print(client.state());
            Serial.println(" coba lagi dalam 2 detik");
            delay(2000);
        }
    }
}

// ==================== SETUP ====================
void setup() {
    Serial.begin(115200);           // Inisialisasi Serial Monitor
    pinMode(ledPin, OUTPUT);        // Set GPIO 4 sebagai output
    
    // ==================== MODIFIKASI: INISIALISASI PWM ====================
    analogWriteRange(pwmRange);       // Set range PWM 0-1023 (10-bit)
    analogWriteFreq(pwmFrequency);    // Set frekuensi PWM 5 kHz
    analogWrite(ledPin, 0);           // Mulai dengan LED mati
    
    hubungkanWiFi();                // Hubungkan ke WiFi
    
    client.setServer(mqttServer, mqttPort);  // Set alamat broker MQTT
    client.setCallback(callback);            // Daftarkan fungsi callback
}

// ==================== LOOP ====================
void loop() {
    // Cek koneksi MQTT, jika terputus maka sambungkan ulang
    if (!client.connected()) {
        hubungkanMQTT();
    }
    
    // Wajib dipanggil terus-menerus agar pesan dapat diterima
    // dan koneksi ke broker tetap aktif
    client.loop();
}
