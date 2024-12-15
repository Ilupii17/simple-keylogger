import socket

# Konfigurasi server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("0.0.0.0", 8080))  # Dengarkan di semua alamat IP
server_socket.listen(5)
print("Menunggu koneksi...")

client_socket, addr = server_socket.accept()
print(f"Koneksi diterima dari {addr}")

while True:
    data = client_socket.recv(1024)  # Terima data sebagai bytes
    if not data:
        break
    try:
        # Coba decode data menjadi string (hanya jika data valid)
        decoded_data = data.decode('utf-8')
        print(f"Data diterima: {decoded_data}")
    except UnicodeDecodeError:
        # Jika gagal, tampilkan data dalam format byte
        print(f"Data biner diterima: {data}")
        
client_socket.close()
server_socket.close()

