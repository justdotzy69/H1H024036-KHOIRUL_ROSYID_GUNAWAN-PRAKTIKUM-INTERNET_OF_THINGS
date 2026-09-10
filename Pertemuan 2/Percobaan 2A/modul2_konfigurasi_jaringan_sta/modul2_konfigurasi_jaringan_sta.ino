  #include <WiFi.h> 

// Mendeklarasikan variabel konstanta untuk menyimpan nama (SSID) dan kata sandi WiFi yang akan dituju
const char* ssid = "NAMA_WIFI_ANDA"; 
const char* password = "PASSWORD_WIFI_ANDA";
const int ledPin = 2; 

void setup() {
  // Memulai komunikasi serial dengan baud rate 115200 agar ESP32 bisa mengirim data ke Serial Monitor komputer
  Serial.begin(115200); 

  // Mengatur pin LED (GPIO 2) sebagai OUTPUT keras (perangkat keras pengeluaran)
  pinMode(ledPin, OUTPUT); 
  
  // Memastikan LED dalam keadaan mati (LOW) pada saat program baru pertama kali berjalan
  digitalWrite(ledPin, LOW); 

  // Mengatur mode WiFi ESP32 menjadi mode Station (STA) agar berperan sebagai klien jaringan
  WiFi.mode(WIFI_STA); 
  
  // Memulai proses koneksi ke jaringan WiFi menggunakan SSID dan password yang sudah dideklarasikan
  WiFi.begin(ssid, password); 

  Serial.print("Menghubungkan ke WiFi"); 

  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); // 
    Serial.print("."); // Mencetak titik-titik di Serial Monitor sebagai indikasi proses loading/mencari jaringan
  }

  // Baris-baris di bawah ini hanya akan dieksekusi JIKA perulangan while di atas selesai (WiFi berhasil terhubung)
  Serial.println(); 
  Serial.println("WiFi berhasil terhubung!"); 
  
  Serial.print("IP Address : "); 
  // Menampilkan alamat IP lokal yang berhasil diperoleh ESP32 dari router WiFi
  Serial.println(WiFi.localIP()); 
  
  Serial.print("MAC Address: "); 
  // Menampilkan alamat MAC (identitas fisik) dari perangkat ESP32
  Serial.println(WiFi.macAddress()); 
  
  Serial.print("RSSI (dBm) : "); 
  // Menampilkan nilai RSSI yang menunjukkan seberapa kuat sinyal WiFi yang diterima oleh ESP32
  Serial.println(WiFi.RSSI()); 

  // Menyalakan LED (memberikan sinyal HIGH) sebagai indikator visual bahwa perangkat sukses terhubung ke jaringan
  digitalWrite(ledPin, HIGH); 
}

void loop() {
  // 

  // Mengecek apakah status WiFi saat ini masih dalam keadaan terhubung 
  if (WiFi.status() == WL_CONNECTED) { 
    Serial.println("Status: Terhubung"); // Jika masih terhubung, cetak teks ini
  } else { 
    // Jika koneksi terputus dari router, masuk ke blok ini
    Serial.println("Status: Terputus"); 
    
    // Mematikan LED sebagai indikator visual bahwa koneksi jaringan sedang hilang/terputus
    digitalWrite(ledPin, LOW); 
  }
  
  // Memberikan jeda waktu 5000 milidetik (5 detik) s
  delay(5000); 
}