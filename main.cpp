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


int cek_user(string username);
void clear();
bool login(Users *user);
void daftar();
vector<Books> list_buku();


int main(){
    Users user;
    bool auth = false;
    char pilih;

    clear();
    while (true) {
        cout << endl;
        cout << "       Selamat Datang Di Perpustakaan      " << endl;
        cout << "-------------------------------------------" << endl; 
        cout << "Silahkan login terlebih dahulu untuk masuk ke dalam perpustakaan" << endl;
        cout << "atau daftar bila tidak mempunyai akun" << endl;
        cout << "1) Login \n2) Daftar \n3) Keluar \n>>> ";

        cin >> pilih;
        clear();

        switch (pilih)
        {
        case '1':
            if (login(&user)){
                auth = true;
            }
            break;
        default:
            break;
        }

        if (auth) {
            break;
        }
    }

    while (auth){
        clear();
        cout << "       Selamat Datang Di Perpustakaan      " << endl;
        cout << "-------------------------------------------" << endl;
        cout << "1) Daftar Buku \n2) Buat buku \n3) Update buku \n4)Delete buku " << endl;
        cout << "pilih menu: ";

        cin >> pilih;
        switch (pilih)
        {
        case '1':
            break;
        
        default:
            break;
        }

    }
}


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
        cout << "Username: "; 
        cin >> username;
        cout << "Password: "; 
        cin >> password;
        
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
        cin >> username;
        
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
        cin >> username;
        
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

vector<Books> list_buku(){
    fstream DB_BOOK(DB_BOOK_NAME);
    string line;
    bool ada;
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
        ada = true;
    }
    if (!ada) {
        cout << "Silahkan masuk sebagai admin untuk" << endl << "menambahkan buku" << endl;
    }
    
    DB_BOOK.close();
    return all_books;
}

int daftar_buku(){
    // Header
    while (true) {
        cout << "====================================" << endl
             << "             Daftar Buku            " << endl
             << "------------------------------------" << endl;

        // Menampilkan semua buku
        // deklarasi variable
        string pilih_buku, line;
        vector<Books> semua_buku = list_buku();

        // Jika buku di perpustakkaan
        // return
        if (semua_buku.empty()) {
            cout << endl <<"Tekan apa saja untuk melanjutkan" << endl;
            cin >> line;
            return 0;
        }

        // Detail Buku
        cout << "Pilih buku untuk melihat detail buku atau" << endl
                << "input "<< semua_buku.size() + 1 <<" untuk kembali" << endl
                << ">> ";

        // mendapatkan index dari buku
        cin >> pilih_buku;

        try {
            // Convert string menjadi int
            int pilihan = stoi(pilih_buku);

            // plihan 1 dan tidak lebih dari jumlah buku
            if (pilihan >= 1 && pilihan <= semua_buku.size()){

                // Mendapatkan buku by index
                Books curr_buku = semua_buku[pilihan-1];

                clear();

                // Header
                cout << "                Buku                " << endl
                     << "------------------------------------" << endl;

                // Print Informasi buku
                cout << "Judul: " << curr_book.judul << endl
                     << "ISBN: " << curr_book.isbn << endl
                     << "Penulis:" << curr_book.penulis << endl
                     << "Tahun: " << curr_book.tahun << endl
                     << "Halaman: " << curr_book.halaman << endl
                     << "Penerbit: " << curr_book.penerbit << endl;

                // tekan apa saja
                cout <<"Tekan apa saja untuk melanjutkan" << endl;
                cin >> line;
                continue;
            
            // kaluar dari daftar buku
            } else if (semua_buku.size() + 1) {
                return 0;
            }
        } catch (invalid_argument& e){
            // Menangkap bila terjadi salah input oleh user
            cout << endl <<"invalid_argument. \nTekan apa saja untuk melanjutkan" << endl;
            cin >> line;
        }
    }
}
