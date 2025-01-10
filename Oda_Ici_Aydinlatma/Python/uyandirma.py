from flask import Flask
import os

app = Flask(__name__)

# Müziği başlatacak fonksiyon
def play_music():
    music_file = "muzik.mp3"  # Müziğin tam dosya yolu
    if os.path.exists(music_file):
        os.startfile(music_file)  # Windows Media Player ile çalma
    else:
        print("Müzik dosyası bulunamadı!")

@app.route('/wake')
def wake():
    # Uyku modundan çıkma ve monitörü açma
    os.system("powercfg -h off")  # Hibernate devre dışı bırak
    os.system("nircmd.exe monitor on")  # NirCmd ile monitörü aç

    # Müziği başlat
    play_music()

    return "Bilgisayar uyandırıldı ve müzik çalınıyor"

if __name__ == '__main__':
    app.run(host='', port=) # kodu her çalıştırmadan önce netstat -ano | findstr :8000 koduyla boş mu diye kontrol et ( cmd)
