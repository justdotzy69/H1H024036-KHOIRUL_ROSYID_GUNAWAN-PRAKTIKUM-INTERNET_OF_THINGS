# Pertanyaan Praktikum 4.6.4

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036

---

## 1. Mengapa penggunaan delay() yang lama sebaiknya dihindari pada program yang menggabungkan proses publish dan subscribe secara bersamaan?

**Jawaban:**

`delay()` bersifat **blocking**, yaitu menghentikan seluruh eksekusi program selama durasi yang ditentukan. Pada program yang menggabungkan publish dan subscribe, hal ini menyebabkan:

1. **`client.loop()` tidak dipanggil** — pesan MQTT yang masuk tidak diproses sampai delay selesai, sehingga perintah kendali aktuator menjadi lambat direspons.

2. **Keep-alive ke broker tidak terkirim** — broker MQTT akan menganggap client mati dan memutus koneksi (default keep-alive PubSubClient = 15 detik).

3. **Proses publish tertunda** — jadwal publish data sensor menjadi tidak presisi karena waktu eksekusi kode lain ikut terhitung dalam delay.

4. **Sistem tidak responsif** — aktuator tidak dapat merespons perintah secara real-time, yang bertentangan dengan prinsip dasar IoT.

Oleh karena itu, pendekatan **non-blocking dengan `millis()`** lebih tepat digunakan agar publish dan subscribe dapat berjalan bersamaan tanpa saling mengganggu.

---

## 2. Jelaskan cara kerja mekanisme non-blocking menggunakan fungsi millis() pada program di atas!

**Jawaban:**

`millis()` mengembalikan waktu dalam milidetik sejak board menyala. Program menyimpan waktu publish terakhir pada `waktuTerakhirPublish`. Pada setiap iterasi `loop()`, selisih `millis() - waktuTerakhirPublish` dibandingkan dengan `intervalPublish` (5000 ms).

- **Jika selisih sudah melebihi interval** → program membaca sensor DHT, mem-publish data ke topic data, dan memperbarui `waktuTerakhirPublish`.
- **Jika belum** → program langsung melanjutkan iterasi tanpa menunggu, sehingga `client.loop()` terus dipanggil dan pesan masuk tetap diproses.

Dengan cara ini, publish data sensor dan subscribe perintah kendali dapat berjalan bersamaan (**full duplex**) tanpa saling memblokir.

---

## 3. Apa yang akan terjadi apabila fungsi client.loop() jarang dipanggil (misalnya hanya sekali setiap 10 detik)?

**Jawaban:**

Pesan masuk dari broker baru diproses setiap 10 detik sehingga respons LED/Buzzer terlambat. Selain itu, keep-alive ke broker tidak terkirim tepat waktu; karena nilai keep-alive bawaan PubSubClient adalah 15 detik, koneksi rawan diputus broker dan ESP8266 harus reconnect.

Pada saat terputus, perintah yang dikirim dapat hilang karena tidak ada pengiriman ulang pada QoS 0. Jika banyak pesan masuk sementara `client.loop()` tidak dipanggil, buffer jaringan bisa penuh dan menyebabkan kehilangan data.

---

## 4. Modifikasi program agar menambahkan satu topic perintah baru untuk mengendalikan aktuator kedua (misalnya buzzer), dengan fungsi callback yang dapat membedakan topic mana yang menerima pesan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md!

**Jawaban:**

```cpp
#include <ESP8266WiFi.h>    // Library koneksi WiFi ESP8266
#include <PubSubClient.h>   // Library untuk komunikasi MQTT
#include <ArduinoJson.h>    // Library untuk JSON
#include <DHT.h>            // Library untuk sensor DHT

// ==================== KONFIGURASI WIFI ====================
const char* ssid = "MosoZydraGyrottaZao";          // Ganti dengan SSID WiFi Anda
const char* password = "stone park";                // Ganti dengan password WiFi Anda

// ==================== KONFIGURASI MQTT ====================
const char* mqttServer = "broker.hivemq.com";  // Broker MQTT publik
const int mqttPort = 1883;                      // Port MQTT standar
const char* topicData = "unsoed/tk245004/kelompok1/data";  // Topic publish data sensor

// ==================== MODIFIKASI: DUA TOPIC PERINTAH ====================
const char* topicPerintahLED = "unsoed/tk245004/kelompok1/perintah/led";       // Topic perintah LED
const char* topicPerintahBuzzer = "unsoed/tk245004/kelompok1/perintah/buzzer"; // Topic perintah Buzzer

// ==================== KONFIGURASI SENSOR DHT ====================
#define DHTPIN 2        // GPIO 2 untuk sensor DHT (D4 pada NodeMCU)
#define DHTTYPE DHT11   // Tipe sensor DHT11

// ==================== KONFIGURASI PIN ====================
const int ledPin = 4;       // GPIO 4 untuk LED

// ==================== MODIFIKASI: PIN BUZZER ====================
const int buzzerPin = 5;    // GPIO 5 untuk buzzer

// ==================== OBJEK GLOBAL ====================
DHT dht(DHTPIN, DHTTYPE);       // Objek sensor DHT
WiFiClient espClient;           // Objek client WiFi
PubSubClient client(espClient); // Objek client MQTT

// ==================== VARIABEL NON-BLOCKING ====================
unsigned long waktuTerakhirPublish = 0;     // Menyimpan waktu terakhir publish
const long intervalPublish = 5000;          // Interval publish 5 detik

// ==================== FUNGSI CALLBACK ====================
// Dipanggil otomatis saat ada pesan masuk pada topic yang di-subscribe
void callback(char* topic, byte* payload, unsigned int length) {
    String pesan;
    
    // Konversi payload byte array ke String
    for (unsigned int i = 0; i < length; i++) {
        pesan += (char)payload[i];
    }
    
    // Tampilkan pesan mentah yang diterima
    Serial.print("Pesan diterima [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(pesan);
    
    // Parsing JSON
    JsonDocument doc;
    if (deserializeJson(doc, pesan)) return;  // Abaikan jika parsing gagal
    
    // Ambil nilai perintah
    const char* perintah = doc["perintah"];
    
    // ==================== MODIFIKASI: BEDAKAN TOPIC ====================
    String topicStr = String(topic);  // Konversi topic ke String untuk perbandingan
    
    if (topicStr == topicPerintahLED) {
        // ---------- Kontrol LED ----------
        digitalWrite(ledPin, String(perintah) == "ON" ? HIGH : LOW);
        Serial.print("LED: ");
        Serial.println(perintah);
        
    } else if (topicStr == topicPerintahBuzzer) {
        // ---------- Kontrol Buzzer ----------
        digitalWrite(buzzerPin, String(perintah) == "ON" ? HIGH : LOW);
        Serial.print("Buzzer: ");
        Serial.println(perintah);
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
            
            // ==================== MODIFIKASI: SUBSCRIBE DUA TOPIC ====================
            client.subscribe(topicPerintahLED);
            client.subscribe(topicPerintahBuzzer);
            
            Serial.println("Subscribe ke topic LED dan Buzzer");
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
    digitalWrite(ledPin, LOW);      // Pastikan LED mati saat mulai
    
    // ==================== MODIFIKASI: INISIALISASI BUZZER ====================
    pinMode(buzzerPin, OUTPUT);     // Set GPIO 5 sebagai output
    digitalWrite(buzzerPin, LOW);   // Pastikan buzzer mati saat mulai
    
    dht.begin();                    // Inisialisasi sensor DHT
    
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
    
    // ==================== PUBLISH DATA SENSOR (NON-BLOCKING) ====================
    // Cek apakah sudah waktunya publish berdasarkan millis()
    if (millis() - waktuTerakhirPublish > intervalPublish) {
        waktuTerakhirPublish = millis();  // Update waktu terakhir publish
        
        // Baca suhu dari sensor DHT
        float suhu = dht.readTemperature();
        
        // Cek apakah pembacaan valid (bukan NaN)
        if (!isnan(suhu)) {
            // Buat JSON document
            JsonDocument doc;
            doc["suhu"] = suhu;
            
            // Serialisasi JSON ke buffer
            char buffer[128];
            serializeJson(doc, buffer);
            
            // Publish ke topic data
            client.publish(topicData, buffer);
            
            Serial.print("Data terkirim: ");
            Serial.println(buffer);
        }
    }
}
