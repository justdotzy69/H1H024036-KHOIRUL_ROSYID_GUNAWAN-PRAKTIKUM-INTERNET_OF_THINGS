# Pertanyaan Praktikum 3.6.4

Nama : Khoirul Rosyid Gunawan

NIM : H1H024036


---

**1.Apa fungsi dari topic pada protokol MQTT, dan mengapa topic yang digunakan perlu dibuat unik**

Jawaban:

Fungsi topic adalah sebagai rute, alamat, atau saluran pengelompokan pesan di dalam jaringan broker. Topic bertugas mengarahkan pesan secara spesifik dari publisher menuju subscriber yang tepat. Topic wajib dibuat unik (misal menyisipkan identitas/NIM) untuk mencegah terjadinya tumpang tindih data. Jika topic sama persis dengan kelompok lain, klien kita akan ikut menerima data kelompok tersebut (gangguan data). 

---

**2.  Jelaskan fungsi dari perintah client.loop() yang dipanggil pada setiap iterasi loop()!**

Jawaban:

Fungsi ini merupakan denyut nadi operasional sistem MQTT. Tujuannya adalah mengeksekusi manajemen jaringan secara terus menerus, seperti mengirim sinyal keep-alive (ping) ke broker untuk membuktikan ESP8266 masih aktif terhubung, memproses pelepasan paket yang tertahan, serta mengecek apakah ada pesan masuk dari broker.

---

**3. Apa yang akan terjadi apabila koneksi ke broker MQTT terputus di tengah program berjalan?**

Jawaban:

Jika koneksi terputus, program akan segera mendeteksinya melalui blok kondisi if (!client.connected()). Ketika dievaluasi dan bernilai benar (putus), sistem secara otomatis akan memanggil ulang fungsi hubungkanMQTT(). Program kemudian akan terjebak di perulangan fungsi reconnect tersebut sambil terus mencoba memulihkan koneksi kembali ke broker sebelum bisa melanjutkan eksekusi program utamanya.

---

