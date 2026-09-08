**Pertanyaan Praktikum 1.5.4**


Nama : Khoirul Rosyid Gunawan

NIM : H1H024036


## 1. Gambarkan diagram alur (flowchart) proses akuisisi data sensor DHT22 pada program!

Jawaban : 

<img width="849" height="1121" alt="Untitled Diagram drawio" src="https://github.com/user-attachments/assets/49cabc33-15d8-456b-82a0-f42d34f47e92" />

---

## 2. Apa fungsi dari perintah `isnan()` pada program tersebut?

Jawaban:

Perintah isnan() bertugas mendeteksi apakah output dari sensor berupa angka valid atau berstatus Not a Number (NaN). Keluaran NaN biasanya didapatkan apabila proses akuisisi data gagal akibat koneksi kabel yang tidak sempurna, sensor yang belum responsif, atau adanya masalah sinkronisasi komunikasi data. Dengan memanfaatkan fungsi ini, program dapat menyaring data yang rusak dan menampilkan indikator error alih-alih mencetak nilai pengukuran yang salah.


---

## 3. Jelaskan mengapa diperlukan jeda (*delay*) minimal sekitar 2 detik antar pembacaan sensor DHT22!

Jawaban:

Karakteristik perangkat keras DHT22 secara spesifik mensyaratkan interval sampling data di kisaran 2 detik. Jeda waktu ini sangat dibutuhkan agar elemen internal sensor memiliki cukup kesempatan untuk menstabilkan diri dan menyelesaikan transmisi informasi melalui protokol komunikasi kabel tunggal (single-wire). Jika sensor dipaksa melakukan pembacaan dengan frekuensi yang lebih cepat dari spesifikasinya, pengiriman data akan terganggu sehingga berpotensi menghasilkan pengukuran yang tidak presisi atau sekadar mengembalikan nilai NaN.

---

## 4. Modifikasi program agar data suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan sebelum ditampilkan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md!

Jawaban : 

<a href="https://github.com/justdotzy69/H1H024036-KHOIRUL_ROSYID_GUNAWAN-PRAKTIKUM-INTERNET_OF_THINGS/tree/main/Pertemuan%201/Percobaan%201A/jawaban1">Jawaban 1 </a>
