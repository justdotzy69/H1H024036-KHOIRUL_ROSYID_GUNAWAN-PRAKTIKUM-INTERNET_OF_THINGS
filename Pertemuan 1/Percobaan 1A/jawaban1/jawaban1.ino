#include <DHT.h>
#define DHTPIN 4 // pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22 // tipe sensor yang digunakan
DHT dht(DHTPIN, DHTTYPE);

const int JUMLAH_SAMPEL = 5; // jumlah pembacaan yang akan dirata-ratakan

void setup() {
  Serial.begin(115200);
  dht.begin(); // inisialisasi sensor DHT22
  Serial.println("Memulai akuisisi data sensor DHT22...");
}

void loop() {
  float totalKelembaban = 0; // penampung jumlah kelembaban dari beberapa sampel
  float totalSuhu = 0;       // penampung jumlah suhu dari beberapa sampel
  int jumlahValid = 0;       // penghitung sampel yang berhasil dibaca (bukan NaN)

  for (int i = 0; i < JUMLAH_SAMPEL; i++) { // ulangi pembacaan sebanyak JUMLAH_SAMPEL kali
    // Membaca data kelembaban dan suhu
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();

    // Periksa apakah pembacaan berhasil
    if (isnan(kelembaban) || isnan(suhu)) {
      Serial.println("Gagal membaca data dari sensor DHT22!");
    } else {
      totalKelembaban += kelembaban; // tambahkan kelembaban yang valid ke total
      totalSuhu += suhu;             // tambahkan suhu yang valid ke total
      jumlahValid++;                 // tambah jumlah sampel yang valid
    }

    delay(2000); // jeda pembacaan setiap 2 detik (sekarang berlaku di tiap sampel)
  }

  if (jumlahValid > 0) { // 
    float rataSuhu = totalSuhu / jumlahValid;             // hitung rata-rata suhu
    float rataKelembaban = totalKelembaban / jumlahValid; // hitung rata-rata kelembaban

    Serial.print("Suhu: ");
    Serial.print(rataSuhu);
    Serial.print(" °C, Kelembaban: ");
    Serial.print(rataKelembaban);
    Serial.println(" %");
  } else {
    Serial.println("Semua sampel gagal dibaca dari sensor DHT22!"); 
}