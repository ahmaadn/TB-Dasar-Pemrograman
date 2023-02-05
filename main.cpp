#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#define DB_BUKU_NAME "buku.csv"
#define DB_USER_NAME "user.csv"

using namespace std;

struct Buku {
    string isbn;
    string judul;
    string penulis;
    string tahun;
    string halaman;
    string penerbit;
};

void clear();
string garis(int width = 50);
bool login();
void buat_akun();
vector<Buku> list_buku();
void print_buku(Buku &buku);
void input_buku(Buku &buku);
void simpan_buku(vector<Buku> &semua_buku);
void sort_buku(vector<Buku> &semua_buku);

char username[100], password[100];

int get_index(int prompt){
    int index;
    cout << "Pilih [1-" << prompt << "]? : ";
    if (!(cin >> index)) {
        cin.clear();
        cin.ignore();
        cout << endl << "Input tidak benar" << endl;
        return get_index(prompt);
    }
    return index;
}

int main(){
    bool auth = false;
    char pilih;

    clear();
    while (!auth) {
        cout << endl << "Selamat Datang Di Perpustakaan" << endl
             << garis() << endl
             << "Silahkan login terlebih dahulu untuk masuk " << endl 
             << "ke dalam perpustakaan atau daftar bila tidak" << endl 
             << "mempunyai akun" << endl;
        cout << "1) Login "<< endl 
             << "2) Buat akun "<< endl 
             << "3) Keluar "<< endl
             << "pilih [1-3]? : ";

        cin >> pilih;
        switch (pilih){
            case '1':
                clear();
                if (login()){
                    auth = true;
                    cout << "Login Berhasill" << endl << endl;
                }
                break;
            case '2':
                clear();
                buat_akun();
                break;
            case '3':
                cout << endl << "program keluar terima kasih telah berkunjung" << endl;
                return 0;
            default:
                break;
        }
    }

    while (auth) {
        vector<Buku> semua_buku = list_buku();      // Mendapatkan semua buku
        int size = semua_buku.size();               // Jumlah buku
        int index;

        cout << endl << "Daftar Buku" << endl << garis() << endl;
        if (semua_buku.empty()){
            cout << "Buku tidak tesedia" << endl 
                 << "Silahkan tambahkan buku terlebih dahulu" << endl;
        } else {
            cout << "NO  | JUDUL" << endl;
            for (int i=1; i<=size; i++){
                cout << i << "    " << semua_buku[i-1].judul << endl;
            }

            cout << endl << "Pilih 1 - " << size 
                 << " untuk melihat detail buku" << endl;
        }

        cout << "   Tambah buku  : " << size + 1 << endl
             << "   Update buku  : " << size + 2 << endl
             << "   Delete buku  : " << size + 3 << endl
             << "   Sort buku    : " << size + 4 << endl 
             << "   Keluar       : " << size + 5 << endl << endl;

        index = get_index(size + 4);

        // Detail buku
        if (index >= 1 && index <= size){
            clear();
            Buku &buku = semua_buku[index-1];
            cout << endl << "BUKU " << buku.judul 
                 << endl << garis() << endl;
            print_buku(buku);
            cout << endl << "Input apa saja untuk melanjutkan : ";
            cin >> pilih;

        // Tambah buku
        } else if (index == size + 1) {
            char ulang;
            do {
                Buku buku_baru;
                cin.ignore(); 
                cout << "Tambah buku" << endl << garis() << endl
                     << "Inputkan Data Buku Dibawah" << endl;
                
                input_buku(buku_baru);
                semua_buku.push_back(buku_baru);

                cout << "Apakah Anda Akan Meng-Input Data Lagi (y/n)? ";
                cin >> ulang;
                clear();
            } while (ulang == 'y' || ulang == 'Y');
            simpan_buku(semua_buku);
        
        // Keluar program
        } else if (index == size + 5) {
            cout << endl << "program keluar terima kasih telah berkunjung" << endl;
            auth = false;
            break;

        // Jika buku kosong execkusi statement
        } else if (semua_buku.empty()) {
            clear();
            cout << "Harap masukan buku terlebih dahulu" << endl;
            continue;

        // Update dan Delete Buku
        } else if (index >= size + 2 && index <= size + 3) {
            
            cout << endl << "Pilih buku 1 - " << size << endl;
            int pilih = get_index(size);

            if (pilih >= 1 && pilih <= size) {
                // mendapatkan index buku
                Buku &buku = semua_buku[pilih-1];
    
                clear();
                cout << endl << "Buku " << buku.judul 
                     << endl << garis() << endl;
                print_buku(buku);

                // Update buku
                if (index == size + 2){
                    cout << endl << "Buku yang dipilih" 
                         << endl << "Inputkan data untuk update buku" << endl;

                    cin.ignore();
                    input_buku(buku);
                    
                    clear();
                    cout << "Buku Berhasil di update" << endl << endl;

                // Delete Buku
                } else if (index == size + 3){
                    char yakin;
                    cout << "Apakah yakin ingin menghapus buku [y/n]: "; cin >> yakin;
                    clear();
                    if (yakin == 'y' || yakin == 'Y') {
                        semua_buku.erase(semua_buku.begin() + (pilih - 1));
                        cout << "Buku berhasil di hapus" << endl;
                    }
                }
                // Simpan ke databse ketika sudah di update / delete
                simpan_buku(semua_buku);
                continue;

            }
        // Sort buku
        } else if (index == size + 4) {
            clear();
            sort_buku(semua_buku);
            cout << "Sorting berhasil bedasarkan Judul berhasil" << endl << endl;
            simpan_buku(semua_buku);
        }
    }
}

void clear(){
    system("cls");
}

string garis(int width){
    string line = "";
    for (int i = 0;i < width; i++){
        line += "-";
    }
    return line;
}

bool login(){
    fstream DB_USER(DB_USER_NAME);
    bool terdaftar = false;
    int i = 0;

    cin.ignore();
    while (i < 3){
        string line;
        i++;
        cout << endl << "LOGIN" << endl << garis(50)<< endl;
        cout << "Username: "; cin.getline(username, 100);
        cout << "Password: "; cin.getline(password, 100);

        clear();
        getline(DB_USER, line);
        while (getline(DB_USER, line)) {
            int pos = line.find(";");
            string usr = line.substr(0, pos);
            string pwd = line.substr(pos + 1);

            if (username == usr && password == pwd){
                terdaftar = true;
                break;
            }
        }

        DB_USER.clear();
        DB_USER.seekg(0, ios::beg);

        if (terdaftar) {
            DB_USER.close();
            return true;
        }
        cout << "Password dan username salah silahkan coba lagi" << endl
             << "sisa percobaan: " << 3 - i << "x" << endl;
    }
    DB_USER.close();
    clear();
    return false;
}

void buat_akun(){
    fstream DB_USER(DB_USER_NAME);
    string line;
    int i = 0;

    cin.ignore();
    do {
        bool terdaftar = false;
        i++;

        cout << "BUAT AKUN" << endl << garis() << endl;
        cout << "Username (new): "; cin.getline(username, 100);
        cout << "Password (new): "; cin.getline(password, 100);

        getline(DB_USER, line);
        while (getline(DB_USER, line)){
            int pos = line.find(";");
            string usr = line.substr(0, pos);
            if (usr == username){
                terdaftar = true;
                break;
            }
        }

        DB_USER.clear();
        DB_USER.seekg(0, ios::beg);
        clear();

        if (!terdaftar){
            clear();
            cout << "Pembuatan akun berhasil" << endl
                 << "Silahkan login untuk melanjutkan" << endl;
            DB_USER.close();
            DB_USER.open(DB_USER_NAME, ios::app);
            DB_USER << username << ";" << password << endl;
            DB_USER.close();
            break;
        } else {
            cout << "Pembuatan Akun Gagal" << endl
                 << "Akun telah terdaftar silahkan coba username yang lain" << endl
                 << "sisa percobaan: " << 3 - i << "x" << endl << endl;
        }

    } while (i < 3);
}

vector<Buku> list_buku() {
    fstream DB_BOOK(DB_BUKU_NAME);
    vector<Buku> semua_buku;
    string line;

    getline(DB_BOOK, line);
    while (getline(DB_BOOK, line)){
        stringstream ss(line);
        Buku buku;

        getline(ss, buku.isbn, ';');
        getline(ss, buku.judul, ';');
        getline(ss, buku.penulis, ';');
        getline(ss, buku.tahun, ';');
        getline(ss, buku.halaman, ';');
        getline(ss, buku.penerbit);
        semua_buku.push_back(buku);
    }
    DB_BOOK.close();
    return semua_buku;
}

void print_buku(Buku &buku) {
    cout << "Judul      : " << buku.judul << endl
         << "ISBN       : " << buku.isbn << endl
         << "Penulis    : " << buku.penulis << endl
         << "Tahun      : " << buku.tahun << endl
         << "Halaman    : " << buku.halaman << endl
         << "Penerbit   : " << buku.penerbit << endl;
}

void input_buku(Buku &buku) {
    cout << "ISBN Buku : ";             getline(cin, buku.isbn);
    cout << "Judul buku : ";            getline(cin, buku.judul);
    cout << "Penulis buku : ";          getline(cin, buku.penulis);
    cout << "Terbitan tahun : ";        getline(cin, buku.tahun);
    cout << "Jumlah halaman buku : ";   getline(cin, buku.halaman);
    cout << "Dari penerbit : ";         getline(cin, buku.penerbit);
}

void simpan_buku(vector<Buku> &semua_buku){
    ofstream DB_BUKU(DB_BUKU_NAME, ios::trunc);
    DB_BUKU << "isbn;judul;penulis;tahun;halaman;penerbit" << endl;

    for (int i = 0; i < semua_buku.size(); i++){
        Buku &buku = semua_buku[i];
        DB_BUKU << buku.isbn << ";"
                << buku.judul << ";"
                << buku.penulis << ";"
                << buku.tahun << ";"
                << buku.halaman << ";"
                << buku.penerbit << endl;
    }
    DB_BUKU.close();
}

void sort_buku(vector<Buku> &semua_buku){
    int n = semua_buku.size();
    for (int i = 1; i < n; i++) {
        Buku &key = semua_buku[i];
        int j = i - 1;

        while (j >= 0 && semua_buku[j].judul > key.judul) {
            semua_buku[j + 1] = semua_buku[j];
            j--;
        }
        semua_buku[j + 1] = key;
    }
}
