#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#define DB_USER_NAME "data/user.csv"
#define DB_BOOK_NAME "data/books.csv"

using namespace std;

struct Users {
    string username;
    string password;
};

struct Books {
    string isbn;
    string judul;
    string penulis;
    string tahun;
    string halaman;
    string penerbit;
};

void clear();
bool login();
void buat_akun();
vector<Books> list_buku();
void print_list_buku(vector<Books> &semua_buku);
int lihat_buku();
void update_buku();
void tambah_buku();
void delete_buku();


int main(){
    bool auth = false, run=true;
    char pilih;
    clear();
    while (run) {
        cout << endl;
        cout << "       Selamat Datang Di Perpustakaan      " << endl;
        cout << "-------------------------------------------" << endl; 
        cout << "Silahkan login terlebih dahulu untuk masuk \nke dalam perpustakaan" << endl;
        cout << "atau daftar bila tidak mempunyai akun" << endl;
        cout << "1) Login \n2) Daftar \n3) Keluar \n>>> ";

        cin >> pilih;
        clear();

        switch (pilih)
        {
        case '1':
            if (login()) auth = true;
            break;
        case '2':
            buat_akun();
            break;
        case '3':
            cout << "Program Berhasil Keluar" << endl;
            return 0;
        default:
            break;
        }

        if (auth) {
            cout << "Login telah berhasil" << endl << endl;
            break;
        }
    }


    while (auth){
        cout << "       Selamat Datang Di Perpustakaan      " << endl;
        cout << "-------------------------------------------" << endl;
        cout << "1) Daftar Buku \n2) Tambah buku \n3) Update buku \n4) Delete buku " << endl;
        cout << "pilih menu: ";

        cin >> pilih;
        clear();
        switch (pilih)
        {
        case '1':
            lihat_buku();
            break;
        case '2':
            tambah_buku();
            break;
        default:
            break;
        }
    }
}

void clear(){
    // Untuk menghapus interface pada terminal
    cout << "\e[1;1H\e[2J";
}

bool login(){
    fstream DB_USER(DB_USER_NAME);
    bool terdaftar = false;
    int i = 0;

    // Kesempatan login sebanyak 3x
    while(i < 3) {
        i++;
        cout << "                   Login                   " << endl
             << "-------------------------------------------" << endl;
        // Input Username dan Password
        Users auth_user;
        string line;
        cout << "Username: ";  cin >> auth_user.username;
        cout << "Password: ";  cin >> auth_user.password;
        clear();

        // lewati baris pertama
        getline(DB_USER, line);
        
        // Dapatkan username dan password setiap baris
        while (getline(DB_USER, line)) {
            // Mencari password dan username
            int pos = line.find(";");
            string usr = line.substr(0, pos);
            string pwd = line.substr(pos + 1);

            if (auth_user.username == usr && auth_user.password == pwd) {
                terdaftar = true;
                break;
            }
        }

        // Untuk Memindahkan pointer file ke posisi awal
        DB_USER.clear();
        DB_USER.seekg(0, ios::beg);

        if (terdaftar) {
            break;
        }
        cout << "Password dan username salah silahkan coba lagi" << endl;
        cout << "sisa percobaan: " << 3 - i << "x" << endl;
        
    }
    DB_USER.close();

    if (terdaftar) {
        return true;
    }
    clear();
    return false;
}

void buat_akun(){
    Users new_user;
    string line;
    fstream DB_USER;
    DB_USER.open(DB_USER_NAME);
    int i = 0;

    do {
        bool terdaftar = false;
        i++;
        cout << "           Membuat akun Baru               " << endl
             << "-------------------------------------------" << endl;
        cout << "Username (new): "; cin >> new_user.username;
        cout << "Password (new): "; cin >> new_user.password;

        getline(DB_USER, line);
        while (getline(DB_USER, line)) {
            int pos = line.find(";");
            string usr = line.substr(0, pos);
            if (usr == new_user.username) {
                terdaftar = true;
                break;
            }
        }

        DB_USER.clear();
        DB_USER.seekg(0, ios::beg);
        clear();

        if (!terdaftar) {
            clear();
            cout << "Pembuatan akun berhasil" << endl;
            cout << "Silahkan login untuk melanjutkan" << endl;
            DB_USER.close();
            DB_USER.open(DB_USER_NAME, ios::app);
            DB_USER << new_user.username << ";" << new_user.password << endl;
            break;
        } else {
            cout << "Pembuatan Akun Gagal" << endl;
            cout << "Akun telah terdaftar silahkan coba username yang lain" << endl << endl;
        }

    } while (i < 3) ;
}


vector<Books> list_buku(){
    fstream DB_BOOK(DB_BOOK_NAME);
    string line;
    vector<Books> semua_buku;

    // Melewati 1 barus
    getline(DB_BOOK, line);

    while (getline(DB_BOOK, line)) {
        stringstream ss(line);
        Books buku;

        // Dapatkan semua informasi buku dan simpan ke variable buku
        getline(ss, buku.isbn, ';');
        getline(ss, buku.judul, ';');
        getline(ss, buku.penulis, ';');
        getline(ss, buku.tahun, ';');
        getline(ss, buku.halaman, ';');
        getline(ss, buku.penerbit);
        // simpan buku di all books
        semua_buku.push_back(buku);
    }
    DB_BOOK.close();
    return semua_buku;
}

void print_list_buku(vector<Books> &semua_buku) {
    cout << "No | Judul" << endl;
    if (semua_buku.empty()) {
        string line;
        cout << "           --- TIDAK ADA BUKU ---          " << endl;
        cout << "Silahkan menambahkan buku terlebih dahulu untuk \nmelihat buku" << endl;
        cout << endl <<"Input apa saja untuk melanjutkan\n>>> ";
        cin >> line;
        clear();
    } else {
        for (int i = 0;i<semua_buku.size();i++) {
            cout << i+1 << "  | " << semua_buku[i].judul << endl; 
        }
    }
}

int lihat_buku(){
    // Header
    while (true) {
        clear();
        cout << "                Daftar Buku                " << endl
             << "-------------------------------------------" << endl;
        
        // deklarasi variable
        vector<Books> semua_buku = list_buku();
        // Menampilkan semua buku
        print_list_buku(semua_buku);
        // Kembali ke menu awal jika daftar buku kosong
        if (semua_buku.empty()) {
            return 0;
        }

        cout << "Pilih index 1-" << semua_buku.size() << " untuk melihat detail buku" << endl;
        cout << "Untuk Keluar: b \nUntuk mengurutkan buku: s \n>>> ";

        // mendapatkan index dari buku
        string pilih;
        cin >> pilih;
        clear();

        if (pilih == "b") {
            return 0;
        } else if (pilih == "s") {
            continue;
        }

        try {
            // Convert string menjadi int
            int pilihan = stoi(pilih);
            // plihan 1 dan tidak lebih dari jumlah buku
            if (pilihan >= 1 && pilihan <= semua_buku.size()){
                // Mendapatkan buku by index
                Books curr_buku = semua_buku[pilihan-1];
                // Print Informasi buku
                cout << "                   Buku                    " << endl
                     << "-------------------------------------------" << endl;
                cout << "Judul: " << curr_buku.judul << endl
                     << "ISBN: " << curr_buku.isbn << endl
                     << "Penulis:" << curr_buku.penulis << endl
                     << "Tahun: " << curr_buku.tahun << endl
                     << "Halaman: " << curr_buku.halaman << endl
                     << "Penerbit: " << curr_buku.penerbit << endl;

                // tekan apa saja
                cout << endl << "Input apa saja untuk melanjutkan \n>>> ";
                cin >> pilih;
            }
            continue;
        } catch (invalid_argument& e){
            continue;
        }
    }
    return 0;
}

void tambah_buku() {
    char ulang;
    do{
        Books buku_baru;
        fstream DB_BOOK(DB_BOOK_NAME, ios::app);
        cin.ignore();
        cout << "               Buat buku baru              " << endl 
            << "-------------------------------------------" << endl;
        cout << "Inputkan Data Buku Dibawah" << endl;
        cout << "ISBN Buku : ";             getline(cin, buku_baru.isbn);
        cout << "Judul buku : ";            getline(cin, buku_baru.judul);
        cout << "Penulis buku : ";          getline(cin, buku_baru.penulis);
        cout << "Terbitan tahun : ";        getline(cin, buku_baru.tahun);
        cout << "Jumlah halaman buku : ";   getline(cin, buku_baru.halaman);
        cout << "Dari penerbit : ";         getline(cin, buku_baru.penerbit);

        DB_BOOK << buku_baru.isbn << ";"
                << buku_baru.judul << ";"
                << buku_baru.penulis << ";"
                << buku_baru.tahun << ";"
                << buku_baru.halaman << ";"
                << buku_baru.penerbit << ";"
                << endl;
        
        cout << "Apakah Anda Akan Meng-Input Data Lagi (y/n)? ";
        cin >> ulang;
        clear();
    } while(ulang == 'y' || ulang == 'Y');
}
