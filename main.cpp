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

int cek_user(string username);
void clear();
bool login(Users *user);
void daftar_akun();
vector<Books> list_buku();


int main(){
    Users user;
    bool auth = false;
    char pilih;

    clear();
    while (true) {
        cout << endl;
        cout << "           Selamat Datang Di Perpustakaan          " << endl;
        cout << "---------------------------------------------------" << endl; 
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
        case '2':
            daftar_akun();
            break;
        case '3':
            cout << "Program Berhasil Keluar" << endl;
            return 0;
        default:
            break;
        }

        if (auth) {
            break;
        }
    }

    cout << "Login telah berhasil" << endl;

    while (auth){
        clear();
        cout << "       Selamat Datang Di Perpustakaan      " << endl;
        cout << "-------------------------------------------" << endl;
        cout << "1) Daftar Buku \n2) Buat buku \n3) Update buku \n4) Delete buku " << endl;
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
    int i = 0;

    do {
        i++;
        cout << "                       Login                       " << endl
             << "---------------------------------------------------" << endl;
        // Input Username dan Password
        Users auth_user;
        cout << "Username: ";  cin >> auth_user.username;
        cout << "Password: ";  cin >> auth_user.password;

        clear();

        // Cek username apakah ada dalam database?
        // Dalam pengecekan akan mengembalikan posisi baris
        // dimana username tersebut ada
        int terdaftar = cek_user(auth_user.username);
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

        // Mencari password
        int pos = line.find(";");    
        string pwd = line.substr(pos+1);

        // Cek apakah password yang ada di databse dan di input tidak sama
        // jika tidak lewati
        if (auth_user.password != pwd){
            cout << "Username dan password salah" << endl;
            continue;
            // return false;
        }

        // Simpan informasi user ke dalam variable 
        *user = auth_user;
        DB_USER.close();
        return true;
    
    // Kesempatan login sebanyak 3x
    } while(i < 3);

    DB_USER.close();
    return false;
}

void daftar_akun(){
    int terdaftar;
    int spasi;
    Users new_user;
    
    cout << "           Membuat akun Baru               " << endl
         << "-------------------------------------------" << endl
         << "Pastikan username dan password tida ada spasi." << endl;

    while (true) {

        // Input Username
        cout << "Username (new): "; 
        cin >> new_user.username;
        
        // jika username telah terdaftar maka
        // ulang pembuatan username
        if (cek_user(new_user.username)) {
            cout << "Akun Telah Terdaftar, coba untuk login." << endl;
            continue;
        }

        // Mencari spasi di dalam username
        spasi = new_user.username.find(" ");
        
        // Jika tidak ada spasi dan panjang username lebih dari 5
        // maka berhenti
        if (spasi < 0 && new_user.username.length() > 5) {
            break;
        }
        // Ulang input ketika username terdapat spasi
        // dan kurang dari 5 huruf
        cout << "Username tidak boleh pake spasi dan minimal ada 5 huruf" << endl;
    }

    while (true) {
        // Input password
        cout << "password (new): "; 
        cin >> new_user.password;
        
        // Cek spasi
        spasi = new_user.password.find(" ");

        // Jika tidak ada spasi dan panjang password lebih dari 5
        // maka berhenti
        if (spasi < 0 && new_user.password.length() > 5) {
            break;
        }
        // Ulang input ketika terdapat spasi
        // dan password kurang dari 5 huruf 
        cout << "Password tidak boleh pake spasi dan minimal ada 5 huruf" << endl;
    }

    cout << "Silahkan login untuk melanjutkan" << endl;

    // simpan username dan password ke dalam databse
    fstream DB_USER(DB_USER_NAME, ios::app);
    DB_USER << new_user.username << ";" << new_user.password << endl;
    DB_USER.close();
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
        // dan simpan ke variable buku
        getline(ss, buku.isbn, ';');
        getline(ss, buku.judul, ';');
        getline(ss, buku.penulis, ';');
        getline(ss, buku.tahun, ';');
        getline(ss, buku.halaman, ';');
        getline(ss, buku.penerbit);

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
                cout << "Judul: " << curr_buku.judul << endl
                     << "ISBN: " << curr_buku.isbn << endl
                     << "Penulis:" << curr_buku.penulis << endl
                     << "Tahun: " << curr_buku.tahun << endl
                     << "Halaman: " << curr_buku.halaman << endl
                     << "Penerbit: " << curr_buku.penerbit << endl;

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
