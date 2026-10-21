import json
import time
import paho.mqtt.client as mqtt
import pandas as pd
import matplotlib.pyplot as plt

# ... (Bagian Konfigurasi dan Callback MQTT di atas tetap sama) ...

# --- TAHAP 2: SIMPAN KE CSV ---
df = pd.DataFrame(data)
df.to_csv("data_suhu.csv", index=False)
print("Data tersimpan di data_suhu.csv")

# --- TAHAP 3: ANALISIS STATISTIK & Z-SCORE (DATA ASLI) ---
rata2 = df["suhu"].mean()
std   = df["suhu"].std()

if std == 0:
    df["zscore"] = 0.0
else:
    df["zscore"] = (df["suhu"] - rata2) / std

df["anomali"] = df["zscore"].abs() > 2

print("\n=== Statistik Data Asli ===")
print(f"Jumlah anomali: {df['anomali'].sum()}")

# =========================================================================
# [TAMBAHAN 5B] MOVING AVERAGE 5 DATA SEBELUM Z-SCORE
# =========================================================================

# 1. Menghitung rata-rata bergerak (Moving Average) dengan jendela 5 data.
#    min_periods=1 memastikan data ke-1 sampai ke-4 tetap dihitung (tidak NaN).
df["suhu_ma5"] = df["suhu"].rolling(window=5, min_periods=1).mean()

# 2. Menghitung ulang rata-rata (mean) dan simpangan baku (std) 
#    berdasarkan data yang SUDAH dihaluskan (suhu_ma5).
rata2_ma = df["suhu_ma5"].mean()
std_ma   = df["suhu_ma5"].std()

# 3. Menghitung Z-score dari data MA5 (mencegah error jika std_ma = 0)
if std_ma == 0:
    df["zscore_ma"] = 0.0
else:
    df["zscore_ma"] = (df["suhu_ma5"] - rata2_ma) / std_ma

# 4. Menentukan status anomali baru berdasarkan Z-score data MA5 (|z| > 2)
df["anomali_ma"] = df["zscore_ma"].abs() > 2

# 5. Menampilkan perbandingan jumlah anomali ke terminal
print("\n=== Statistik Setelah Moving Average 5 ===")
print(f"Jumlah anomali MA5: {df['anomali_ma'].sum()}")
print(f"Perbandingan      : asli = {df['anomali'].sum()} | MA5 = {df['anomali_ma'].sum()}")

# =========================================================================

# --- TAHAP 4: VISUALISASI DATA ASLI ---
plt.figure(figsize=(10, 4))
plt.plot(df.index, df["suhu"], marker="o", label="Suhu (asli)")
anom = df[df["anomali"]]
plt.scatter(anom.index, anom["suhu"], color="red", s=100, label="Anomali", zorder=3)
plt.axhline(rata2, color="gray", linestyle="--", label="Rata-rata")
plt.title("Data Suhu dan Deteksi Anomali (Data Asli)")
plt.legend()
plt.savefig("grafik_suhu.png")
plt.show()

# --- TAHAP 5: VISUALISASI SETELAH MA5 ---
plt.figure(figsize=(10, 4))
plt.plot(df.index, df["suhu"],     marker="o", color="lightblue", alpha=0.6, label="Suhu asli")
# [TAMBAHAN] Menggambar garis data MA5
plt.plot(df.index, df["suhu_ma5"], marker="o", color="blue",      label="Suhu MA5")
# [TAMBAHAN] Menandai titik anomali pada data MA5 (jika ada)
anom_ma = df[df["anomali_ma"]]
plt.scatter(anom_ma.index, anom_ma["suhu_ma5"], color="orange", s=100, label="Anomali MA5", zorder=3)
plt.axhline(rata2_ma, color="gray", linestyle="--", label="Rata-rata MA5")
plt.title("Data Suhu Setelah Moving Average 5")
plt.legend()
plt.savefig("grafik_suhu_ma5.png")
plt.show()

# --- TAHAP 6: SIMPAN CSV LENGKAP ---
df.to_csv("data_suhu.csv", index=False)
print("Selesai. CSV dan grafik telah diperbarui.")