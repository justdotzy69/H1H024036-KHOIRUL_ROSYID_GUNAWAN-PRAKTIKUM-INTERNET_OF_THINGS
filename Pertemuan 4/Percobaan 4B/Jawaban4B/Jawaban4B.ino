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