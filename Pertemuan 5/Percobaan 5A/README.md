# Pertanyaan Praktikum 5.5.4

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036


---

**1. Gambarkan diagram alur (flowchart) proses perhitunganmoving average pada fungsi hitungMovingAverage()!**

Jawaban:


<img width="422" height="1513" alt="modul 5  drawio" src="https://github.com/user-attachments/assets/3659decb-6e42-4a24-a6bb-c865366e7a5e" />





**2.Jelaskan fungsi dari operasi (indeks + 1) % N pada program tersebut!**

Jawaban:


Operasi tersebut digunakan untuk membuat circular buffer (buffer melingkar). Saat indeks mencapai nilai N (ukuran maksimal buffer), operasi modulo (% N) akan mengembalikan nilai indeks menjadi 0. Hal ini mencegah terjadinya out of bounds pada array dan memastikan bahwa data baru akan menimpa data paling lama secara berurutan.
---

**3. Apa dampaknya terhadap data hasil filter apabila nilai N diperbesar menjadi 20 atau diperkecil menjadi 2**

Jawaban:

Jika N = 20: Data hasil filter akan menjadi jauh lebih halus (noise berkurang drastis), namun sistem akan merespons perubahan suhu secara lambat (lagging). Jika suhu naik mendadak, nilai moving average butuh waktu lama untuk menyesuaikan.

Jika N = 2: Data hasil filter kurang halus dan masih berfluktuasi mendekati data mentah, namun respons terhadap perubahan sangat cepat. Cocok untuk sistem yang butuh reaksi real-time.



---

**4.Modifikasi program agar ESP8266 juga menghitung simpangan baku dari data pada buffer, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md.**

Jawaban:
```cpp
#include <DHT.h>

#define DHTPIN  4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int N = 5;
float buffer[N];
int indeks = 0;
int jumlah = 0;

float suhuMin = 1000.0;
float suhuMax = -1000.0;

float hitungMovingAverage(float nilaiBaru) {
  buffer[indeks] = nilaiBaru;
  indeks = (indeks + 1) % N;
  if (jumlah < N) jumlah++;

  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }
  return total / jumlah;
}

// =========================================================================
// [TAMBAHAN 5A] FUNGSI MENGHITUNG SIMPANGAN BAKU (STANDAR DEVIASI)
// =========================================================================
float hitungSimpanganBaku(float rataRata) {
  // 1. Membuat variabel untuk menampung total dari (data - rata-rata) kuadrat
  float jumlahKuadrat = 0.0; 
  
  // 2. Looping sebanyak data yang saat ini ada di dalam buffer
  for (int i = 0; i < jumlah; i++) {
    // 3. Menghitung selisih antara data ke-i dengan nilai rata-rata
    float selisih = buffer[i] - rataRata; 
    
    // 4. Mengkuadratkan selisih (agar tidak negatif) dan menjumlahkannya
    jumlahKuadrat += selisih * selisih;   
  }
  
  // 5. Jika data masih 1 atau 0, kembalikan 0 untuk mencegah error pembagian nol
  if (jumlah <= 1) return 0.0; 
  
  // 6. Akarkan hasil (Total Kuadrat / Jumlah Data) menggunakan fungsi sqrt()
  return sqrt(jumlahKuadrat / jumlah); 
}
// =========================================================================

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Percobaan 5A - ESP8266 + DHT11 + Simpangan Baku");
}

void loop() {
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Pembacaan gagal, data diabaikan");
  } else {
    float suhuFilter = hitungMovingAverage(suhu);
    
    // [TAMBAHAN] Memanggil fungsi hitungSimpanganBaku dengan parameter hasil filter
    float simpangan = hitungSimpanganBaku(suhuFilter); 

    if (suhu < suhuMin) suhuMin = suhu;
    if (suhu > suhuMax) suhuMax = suhu;

    Serial.print("Mentah: ");
    Serial.print(suhu, 1);
    Serial.print(" | Filter: ");
    Serial.print(suhuFilter, 2);
    
    // [TAMBAHAN] Menampilkan nilai simpangan baku ke Serial Monitor
    Serial.print(" | Std: ");
    Serial.print(simpangan, 2); 
    
    Serial.print(" | Min: ");
    Serial.print(suhuMin, 1);
    Serial.print(" | Max: ");
    Serial.println(suhuMax, 1);
  }

  delay(2000);
}
