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
    string pinjam;
    string isbn_buku;
    string at_pinjam;
};

struct Books {
    string isbn;
    string judul;
    string penulis;
    string tahun;
    string halaman;
    string penerbit;
    string tersedia;
};

int cek_user(string username) {
    fstream DB_USER(DB_USER_NAME);
    string line;
    int i = 1;

    // Melewari baris pertama dalam csv
    getline(DB_USER, line);

    while (getline(DB_USER, line)) {
        // Deklarasi variable
        stringstream ss(line);
        string get_username;

        // Mencari dan mendapatkan username dari database
        // int pos = line.find(",");
        // get_username = line.substr(0, pos);
        getline(ss, get_username, ';');

        // cek username sama yang ada di database
        if (get_username == username){
            // Mengembalikan posisi baris dari username
            DB_USER.close();
            return i;
        }
        i++;
    }
    // Mengembalikan 0 jika username tidak ada dalam database
    DB_USER.close();
    return 0;
}

void clear(){
    // Untuk menghapus interface pada terminal
    cout << "\e[1;1H\e[2J";
}

bool login(Users *user){
    fstream DB_USER(DB_USER_NAME);
    string username, password;
    int i = 0;

    do {
        i++;
        cout << "====================================" << endl
            << "               Login                " << endl
            << "------------------------------------" << endl;
        // Input Username dan Password
        cout << "Username: "; getline(cin, username);
        cout << "Password: "; getline(cin, password);
        
        clear();

        // Cek username apakah ada dalam database?
        // Dalam pengecekan akan mengembalikan posisi baris
        // dimana username tersebut ada
        int terdaftar = cek_user(username);
        // Cek apakah username tidak terdaftar
        if (!terdaftar) {
            cout << "Username dan password salah" << endl;
            continue;
            // return false;
        }

        // Lewati baris lainnya untuk mencari baris yang telah terdaftar
        string line;
        for (int i=0; i <= terdaftar; i++){
            getline(DB_USER, line);
        }

        // deklar variable
        stringstream ss(line);
        Users new_user;

        // Mencari password
        // int pos = line.find(",");    
        // string pwd = line.substr(pos+1);
        getline(ss, new_user.username, ';');
        getline(ss, new_user.password, ';');

        // Cek apakah password yang ada di databse dan di input tidak sama
        // jika tidak lewati
        if (password != new_user.password){
            cout << "Username dan password salah" << endl;
            continue;
            // return false;
        }

        // Simpan informasi user ke dalam variable 
        *user = new_user;
        DB_USER.close();
        return true;

    } while(i < 3);    
    DB_USER.close();
    return false;
}

void daftar(){
    string username, password;
    int terdaftar;
    int spasi;
    
    cout << "====================================" << endl
         << "        Membuat akun Baru           " << endl
         << "------------------------------------" << endl
         << "Pastikan username dan password tida ada " << endl
         << "spasi." << endl;

    while (true) {
        // Input Username
        cout << "Username (new): "; 
        getline(cin, username);
        
        // jika username telah terdaftar maka
        // ulang pembuatan username
        if (cek_user(username)) {
            cout << "Akun Telah Terdaftar, coba untuk login." << endl;
            continue;
        }

        // Mencari spasi di dalam username
        spasi = username.find(" ");
        
        // Jika tidak terdapat spasi dan panjang username lebih dari 5
        // Maka lewati
        if (spasi != -1 || username.length() < 5) {
            cout << "Username tidak boleh pake spasi dan minimal ada 5 huruf" << endl;
            continue;
        }
        // Program berhenti ketika username valid
        break;
    }

    while (true) {
        // Input password
        cout << "password (new): "; 
        getline(cin, password);
        
        // Cek spasi
        spasi = password.find(" ");

        // Jika tidak terdapat spasi dan panjang password lebih dari 5
        // Maka lewati
        if (spasi != -1 || password.length() < 5) {
            cout << "Password tidak boleh pake spasi dan minimal ada 5 huruf" << endl;
            continue;
        }
        // password valid
        break;
    }

    // simpan username dan password ke dalam databse
    fstream DB_USER(DB_USER_NAME, ios::app);
    DB_USER << username << ";" << password << ";null;null;null" << endl;
}

void garis(int jumlah = 50, string type_baris = "=") {
    for (int i = 1; i <= jumlah; i++) {
        cout << type_baris;
    }
    cout << endl;
}

int main(){
    Users user;
    string pilih;

    clear();
    while (true) {
        // Input 
        cout << "====================================" << endl
             << "   Selamat Datang Di Perpustakaan   " << endl
             << "------------------------------------" << endl  
             << "Silahkan login terlebih dahulu untuk " << endl 
             << "masuk ke dalam perpustakaan atau "<< endl 
             << "daftar bila tidak mempunyai akun" << endl
             << "1) Login " << endl 
             << "2) Daftar"   << endl 
             << "3) Keluar" << endl
             << ">> ";
        getline(cin, pilih);
        
        clear();

        if (pilih == "1" && login(&user)){
            cout << user.username << user.password << endl;
            break;
        } else if (pilih == "2") {
            // Untuk daftar akun
            daftar();
            clear();
            continue;
        } else if (pilih == "3") {
            break;
            // keluar program
            // return 0;
        }
    }
}

vector<Books> daftar_buku(){
    fstream DB_BOOK(DB_BOOK_NAME);
    string line;
    vector<Books> all_books;
    int i = 1;

    // Melewati 1 barus
    getline(DB_BOOK, line);

    cout << "No  Judul" << endl;

    while (getline(DB_BOOK, line)) {
        stringstream ss(line);
        Books buku;

        // Dapatkan semua informasi buku
        getline(ss, buku.isbn, ';');
        getline(ss, buku.judul, ';');
        getline(ss, buku.penulis, ';');
        getline(ss, buku.tahun, ';');
        getline(ss, buku.halaman, ';');
        getline(ss, buku.penerbit, ';');
        getline(ss, buku.tersedia);

        // simpan buku di all books
        all_books.push_back(buku);

        // print judul buku
        cout << i << "   " << buku.judul << endl; 
        i++;
    }
    DB_BOOK.close();
    return all_books;
}

void detail_buku(Books *book) {
    Books curr_book = *book;
    
    cout << "====================================" << endl
         << "                Buku                " << endl
         << "------------------------------------" << endl;
    
    cout << "Judul: " << curr_book.judul << endl
         << "ISBN: " << curr_book.isbn << endl
         << "Penulis:" << curr_book.penulis << endl
         << "Tahun: " << curr_book.tahun << endl
         << "Halaman: " << curr_book.halaman << endl
         << "Penerbit: " << curr_book.penerbit << endl;
}


int noneaaa() {



    Users user;
    string pilih;

    user.username = "2206042";
    user.password = "itg@garut";

    while (true){
        cout << "====================================" << endl
            << "   Selamat Datang Di Perpustakaan   " << endl
            << "------------------------------------" << endl
            << "1) Daftar Buku" << endl
            << "2) Pinjam Buku" << endl
            << "3) Kembalikan Buku" << endl
            << "4) Keluar" << endl
            << "Pilih menu: ";
        
        getline(cin, pilih);
        clear();

        if (pilih == "1") {
            // deklarasi variable
            string pilih_buku;
            

            // Header
            cout << "====================================" << endl
                 << "             Daftar Buku            " << endl
                 << "------------------------------------" << endl;

            // Menampilkan semua buku
            vector<Books> semua_buku = daftar_buku();

            // Detail Buku
            cout << "Pilih buku untuk melihat detail buku atau" << endl
                 << "input "<< semua_buku.size() + 1 <<" untuk kembali" << endl
                 << ">> ";
            
            // Input pilihan buku
            // getline(cin, pilih_buku);
            
            // if (pilih_buku == semua_buku.size() + 1) {
            //     continue;
            // } else if ( pilih_buku ) {

            // }

            

            clear();
            continue;
        } else if (pilih == "2") {

        } else if (pilih == "3") {

        } else {
            clear();
            cout << "Silahkan Pilih Menu yang tersedia";
        }
    }
}