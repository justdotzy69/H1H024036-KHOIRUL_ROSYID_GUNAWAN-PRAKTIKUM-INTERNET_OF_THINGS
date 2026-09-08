**Pertanyaan Praktikum 1.6.4**

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036



## 1. Mengapa diperlukan nilai ambang batas (*threshold*) dalam sistem kendali aktuator berbasis sensor?

Jawaban:
Nilai ambang batas (threshold) berperan sebagai patokan untuk mengubah sinyal sensor yang
kontinu (analog) menjadi aksi kendali yang diskret (ON/OFF). Tanpa adanya nilai batas ini,
mikrokontroler tidak memiliki acuan yang jelas kapan harus menghidupkan atau mematikan
aktuator, sehingga sistem gagal merespons perubahan kondisi lingkungan secara otomatis dan
konsisten.

---

## 2. Jelaskan apa yang akan terjadi apabila nilai `suhuThreshold` diturunkan menjadi sangat rendah, misalnya 20.0°C!

Jawaban:
Karena suhu normal ruangan biasanya melebihi 20°C, aktuator akan terus-menerus menyala (ON)
tanpa henti meskipun lingkungan tidak membutuhkannya. Akibatnya, sistem kehilangan fungsi
utamanya sebagai kendali otomatis. Pada implementasi perangkat keras nyata (seperti pada relay),
kondisi ini akan memicu pemborosan energi listrik dan mempercepat kerusakan komponen.

---

## 3. Apa perbedaan antara kendali aktuator kondisi tunggal dengan kendali menggunakan histerisis?

Jawaban : Kendali kondisi tunggal hanya memakai satu titik acuan, sehingga rentan mengalami siklus
nyala-mati dengan cepat (flickering) ketika nilai suhu berfluktuasi tepat di sekitar angka threshold,
yang bisa merusak relay. Sebaliknya, sistem histeresis menerapkan dua batas (atas dan bawah) dan
mengingat status aktuator sebelumnya. Sistem ini hanya mengubah status jika suhu secara pasti
melewati salah satu batas, sehingga jauh lebih stabil selama suhu berada di zona transisi.

---

## 4. Modifikasi program agar menggunakan dua ambang batas (histerisis), misalnya aktuator menyala pada suhu di atas 30°C dan baru mati pada suhu di bawah **28°C**, serta berikan penjelasannya!
Jawaban:

<a href="https://github.com/justdotzy69/H1H024036-KHOIRUL_ROSYID_GUNAWAN-PRAKTIKUM-INTERNET_OF_THINGS/blob/main/Pertemuan%201/Percobaan%202A/Percobaan2/Jawaban2.ino">Jawabannya</a>
