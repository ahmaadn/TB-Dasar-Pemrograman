#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define DB_BUKU_NAME "data/buku.csv"
#define DB_USER_NAME "data/user.csv"

using namespace std;

struct User {
    string username;
    string password;
};

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
void input_buku(Buku &buku);
void daftar_buku(string mode = "lihat");
void tambah_buku();
void simpan_buku(vector<Buku> &semua_buku);


int main(){
    bool auth = false, run = true;
    char pilih;
    int index;

    clear();
    while (run) {
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
        clear();

        switch (pilih){
            case '1':
                if (login()){
                    auth = true;
                }
                break;
            case '2':
                buat_akun();
                break;
            case '3':
                return 0;
            default:
                break;
        }
        if (auth) {
            cout << "Login Berhasill" << endl << endl;
            break;
        }
    }
    
    while (auth) {
        cout << "Selamat Datang Di Perpustakaan" << endl 
             << garis() << endl;
        cout << "1) Daftar buku " << endl 
             << "2) Tambah buku " << endl 
             << "3) Update buku " << endl 
             << "4) Delete buku " << endl
             << "5) Keluar " << endl
             << "pilih [1-4]? : ";

        cin >> pilih;
        clear();

        switch (pilih) {
            case '1':
                daftar_buku();
                break;
            case '2':
                tambah_buku();
                break;
            case '3':
                daftar_buku("update");
            case '4':
                daftar_buku("delete");
                break;
            case '5':
                auth = false;
                break;
            default:
                break;
        }
    }
    
}

void clear(){
    cout << "\e[1;1H\e[2J";
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
        User user;
        string line;
        i++;
        cout << endl << "LOGIN" << endl << garis(50)<< endl;
        cout << "Username: "; getline(cin, user.username);
        cout << "Password: "; getline(cin, user.password);

        clear();
        getline(DB_USER, line);
        while (getline(DB_USER, line)) {
            int pos = line.find(";");
            string usr = line.substr(0, pos);
            string pwd = line.substr(pos + 1);

            if (user.username == usr && user.password == pwd){
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
    User user_baru;
    string line;
    int i = 0;

    cin.ignore();
    do {
        bool terdaftar = false;
        i++;

        cout << "BUAT AKUN" << endl << garis() << endl;
        cout << "Username (new): "; getline(cin, user_baru.username);
        cout << "Password (new): "; getline(cin, user_baru.password);

        getline(DB_USER, line);
        while (getline(DB_USER, line)){
            int pos = line.find(";");
            string usr = line.substr(0, pos);
            if (usr == user_baru.username){
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
            DB_USER << user_baru.username << ";" << user_baru.password << endl;
            break;
        } else {
            cout << "Pembuatan Akun Gagal" << endl
                 << "Akun telah terdaftar silahkan coba username yang lain" << endl
                 << endl;
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

void print_semua_buku(vector<Buku> &semua_buku){
    if (semua_buku.empty()){
        cout << "Buku tidak tesedia" << endl 
             << "Silahkan tambahkan buku terlebih dahulu" << endl;
    } else {
        cout << "NO  | JUDUL" << endl;
        for (int i=1; i<=semua_buku.size();i++){
            cout << i << "   " << semua_buku[i-1].judul << endl;
        }
    }

}

void daftar_buku(string mode){
    string line;
    while (true){
        vector<Buku> semua_buku = list_buku();
        int size = semua_buku.size();
        int pilih;

        clear();
        cout << "Daftar buku" << endl << garis() << endl;
        print_semua_buku(semua_buku);
        cout << endl;

        if (semua_buku.empty()){
            break;
        }

        if (mode == "update") {
            cout << "Pilih index 1-" << size << " untuk di update buku" << endl;
        } else if (mode == "deletr") {
            cout << "Pilih index 1-" << size << " untuk di delete buku" << endl;
        } else {
            cout << "Pilih index 1-" << size << " untuk melihat detail buku" << endl;
        }

        cout << "Keluar:  " << size + 1 << endl 
             << "urutkan buku: " << size + 2 << endl
             << "pilih [1-" << size + 2 << "]? : ";
        
        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore();
        }
        
        clear();

        if (pilih >= 1 && pilih <= size) {
            Buku &buku = semua_buku[pilih-1];
            cout << endl << "BUKU " << buku.judul 
                 << endl << garis() << endl;
            print_buku(buku);

            if (mode == "update") {
                cout << endl << "Buku yang dipilih" 
                     << endl << "Inputkan data untuk update buku" << endl;

                cin.ignore();
                input_buku(buku);
                simpan_buku(semua_buku);

            } else if ( mode == "delete") {
                semua_buku.erase(semua_buku.begin() + (pilih - 1));
                simpan_buku(semua_buku);
                cout << "Buku berhasil di hapus" << endl;
            } else {
                cout << endl << "Input apa saja untuk melanjutkan "<< endl << ">>> ";
                cin >> line;
            }

        } else if (pilih == size + 1) {
            break;
        } else if (pilih == size + 2) {
            continue;
        }
    }
}

void tambah_buku() {
    vector<Buku> semua_buku = list_buku();
    char ulang;
    do {
        Buku buku_baru;
        cin.ignore(); 
        cout << "Tambah buku" << endl << garis() << endl;
        cout << "Inputkan Data Buku Dibawah" << endl;
        input_buku(buku_baru);
        semua_buku.push_back(buku_baru);

        cout << "Apakah Anda Akan Meng-Input Data Lagi (y/n)? ";
        cin >> ulang;
        clear();
    } while (ulang == 'y' || ulang == 'Y');
    simpan_buku(semua_buku);
}
