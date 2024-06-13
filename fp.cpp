#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <regex>

using namespace std;

void clearScreen() 
{        // Fungsi untuk membersihkan layar tiap pemanggilan fungsi
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

class Goods 
{
protected:
    string goods_id;
    string goods_name;
    string goods_category;
    int stock_amount;
    string date;

public:
    Goods(string goods_id, string goods_name, string goods_category, int stock_amount, string date)
        : goods_id(goods_id), goods_name(goods_name), goods_category(goods_category), stock_amount(stock_amount), date(date) {}

    string get_goods_code() const 
    { 
        return goods_id; 
    }
    string get_goods_name() const 
    { 
        return goods_name; 
    }
    string get_goods_category() const 
    { 
        return goods_category; 
    }
    
    int get_stock_amount() const 
    {
        return stock_amount; 
    }
    string get_date() const 
    { 
        return date; 
    }

    void set_goods_name(const string &goods_name) 
    { 
        this ->goods_name = goods_name; 
    }
    
    void set_goods_category(const string &goods_category) 
    { 
        this ->goods_category = goods_category; 
    }
    
    void set_stock_amount(int stock_amount) 
    { 
        this ->stock_amount = stock_amount; 
    }
    
    void set_data(const string &date) 
    { 
        this->date = date; 
    }

    virtual void display_goods() const 
    {
        cout << "ID: " << goods_id << "\nNama: " << goods_name << "\nKategori: " << goods_category
             << "\nJumlah: " << stock_amount << "\nTanggal: " << date << endl;
    }
};

class incoming_goods : public Goods 
{
private:
    int original_stock_amount;
    int current_stock_amount;
    string supplier;

public:
    incoming_goods(string goods_id, string goods_name, string goods_category, int stock_amount, string date, string supplier)
        : Goods(goods_id, goods_name, goods_category, stock_amount, date), original_stock_amount(stock_amount), current_stock_amount(stock_amount), supplier(supplier) {}

    int get_original_stock_amount() const 
    { 
        return original_stock_amount; 
    }
    
    int get_current_stock_amount() const 
    { 
        return current_stock_amount; 
    }
    
    void set_current_stock_amount(int amount) 
    { 
        current_stock_amount = amount; 
    }
    string get_supplier() const 
    { 
        return supplier;
    }
    
    void set_supplier(const string &supplier) 
    { 
        this ->supplier = supplier; 
    }

    void display_goods() const override 
    {
        Goods::display_goods();
        cout << "Supplier: " << supplier << endl << endl;
    }
};

class outgoing_goods : public Goods 
{
private:
    string destination;

public:
    outgoing_goods(string goods_id, string goods_name, string goods_category, int stock_amount, string date, string destination)
        : Goods(goods_id, goods_name, goods_category, stock_amount, date), destination(destination) {}

    string get_destination() const 
    { 
        return destination; 
    }
    
    void set_destination(const string &destination) 
    { 
        this ->destination = destination; 
    }

    void display_goods() const override 
    {
        Goods::display_goods();
        cout << "Destination: " << destination << endl << endl;
    }
};

vector <outgoing_goods>outgoing_goods_list;
vector <incoming_goods>incoming_goods_list;

class inventory_management 
{
private:
    //vector <incoming_goods>incoming_goods_list;

    incoming_goods *find_incoming_goods_list(const string &goods_id) 
    {
        for (auto& goods : incoming_goods_list) 
        {
            if (goods .get_goods_code() == goods_id) 
            {
                return &goods;
            }
        }
        
        return nullptr;
    }

    outgoing_goods *find_outgoing_goods_list(const string &goods_id) 
    {
        for (auto& goods : outgoing_goods_list) 
        {
            if (goods.get_goods_code() == goods_id) 
            {
                return &goods;
            }
        }
        
        return nullptr;
    }

    bool isValidDate(const string &date) 
    {
        // Check format YYYY-MM-DD
        regex datePattern(R"((\d{4})-(\d{2})-(\d{2}))");
        smatch match;
    
        if (!regex_match(date, match, datePattern)) 
        {
            return false;
        }

        int year = stoi(match[1] .str());
        int month = stoi(match[2] .str());
        int day = stoi(match[3] .str());

        if (month < 1 || month > 12) 
        {
            return false;
        }

        vector <int>daysInMonth = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        // Check for leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
        {
            daysInMonth[1] = 29;
        }

        if (day < 1 || day > daysInMonth[month - 1]) 
        {
            return false;
        }

        return true;
    }

public:
    void add_incoming_goods();
    void add_outgoing_goods();
    void view_incoming_goods();
    void view_outgoing_goods();
    void find_incoming_goods();
    void find_outgoing_goods();
    void update_incoming_goods();
    void update_outgoing_goods();
    void delete_incoming_goods();
    void delete_outgoing_goods();
    void manage_incoming_goods();
    void manage_outgoing_goods();
};

void inventory_management::add_incoming_goods() 
{
    string goods_id, goods_name, goods_category, date, supplier;

    char next;
    int stock_amount;

    do
    {
        clearScreen();
        bool is_duplicate;

        // Looping sampai pengguna memasukkan kode barang yang tidak duplikat
        do
        {
            is_duplicate = false;
            cout << "Masukkan ID Barang: ";
            cin >> goods_id;

            // Memeriksa apakah kode barang sudah ada
            for (const auto &goods : incoming_goods_list) 
            {
                if (goods.get_goods_code() == goods_id) 
                {
                    is_duplicate = true;
                    cout << "\nKode barang sudah ada. Silakan masukkan kode barang yang berbeda." << endl;
                    break;
                }
            }
        } while (is_duplicate); // ulangi jika duplikat

        cout << "Masukkan Nama Barang: ";
        cin .ignore();
        getline(cin, goods_name);
        cout << "Masukkan Kategori Barang: ";
        cin .ignore();
        getline(cin, goods_category);

        while (true)
        {
            cout << "Masukkan Jumlah Stok: ";
            cin >> stock_amount;

            if (cin.fail())
            {
                cin.clear(); // Menghapus flag kesalahan
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Mengabaikan input yang salah
                cout << "\nInput tidak valid. Silahkan masukkan angka." << endl;
            }

            else
            {
                break; // Input valid, keluar dari loop
            }
        }

        // Validate date input
        do
        {
            cout << "Masukkan Tanggal (YYYY-MM-DD): ";
            cin >> date;

            if (!isValidDate(date))
            {
                cout << "\nFormat tanggal tidak valid atau hari/bulan yang diinput di luar rentang. Silakan coba lagi.\n";
            }
        } while (!isValidDate(date));

        cout << "Masukkan Supplier: ";
        cin .ignore();
        getline(cin, supplier);

        incoming_goods newGoods(goods_id, goods_name, goods_category, stock_amount, date, supplier);
        incoming_goods_list .push_back(newGoods);

        cout << "\nIncoming goods added successfully!" << endl;
        cout << "Tekan 1 untuk menambahkan barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pemasukan Barang: ";
        cin >> next;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::view_incoming_goods() 
{
    char next;
    do
    {
        if (incoming_goods_list .empty())
        {
            cout << "Tidak ada barang dalam daftar." << endl;
        }

        else
        {
            // Pilihan sorting
            clearScreen();
            int sort_option;
            cout << "=== Daftar Barang Pemasukan ===\n";
            cout << "Pilih kriteria sorting:\n";
            cout << "1. Berdasarkan ID Barang\n";
            cout << "2. Berdasarkan Tanggal\n";
            cout << "3. Berdasarkan Jumlah Stok\n";
            cout << "Masukkan pilihan (1-3): ";
            cin >> sort_option;

            // Sorting berdasarkan pilihan
            switch (sort_option) 
            {
                case 1:
                    clearScreen();
                    sort(incoming_goods_list .begin(), incoming_goods_list .end(),
                        [](const incoming_goods &a, const incoming_goods &b) 
                        {
                            return a .get_goods_code() < b .get_goods_code();
                        });
                    break;
                case 2:
                    clearScreen();
                    sort(incoming_goods_list .begin(), incoming_goods_list .end(),
                        [](const incoming_goods &a, const incoming_goods &b) 
                        {
                            return a .get_date() < b .get_date();
                        });
                    break;
                case 3:
                    clearScreen();
                    sort(incoming_goods_list .begin(), incoming_goods_list .end(),
                        [](const incoming_goods &a, const incoming_goods &b) 
                        {
                            return a .get_current_stock_amount() < b .get_current_stock_amount();
                        });
                    break;
                default:
                    clearScreen();
                    cout << "Pilihan tidak valid. Menampilkan tanpa sorting.\n";
                    break;
            }

            for (const auto &goods : incoming_goods_list) 
            {
                goods .display_goods();
            }
        }
        cout << "\nKetik 1 untuk sorting lagi, atau klik bebas untuk balik ke Menu Manajemen Barang.";
        cin >> next;

        while (getchar() != '\n');
    } while (next == '1');
}

void inventory_management::find_incoming_goods()
{
    string search_key;
    char next;
    do
    {
        clearScreen();
        cout << "Cari berdasarkan:\n1. ID Barang\n2. Tanggal\nPilih opsi: ";
        cin >> next;

        if (next == '1')
        {
            cout << "Masukkan ID Barang yang dicari: ";
            cin >> search_key;
            cout << endl;

            bool found = false;
            for (const auto &goods : incoming_goods_list)
            {
                if (goods .get_goods_code() == search_key)
                {
                    goods .display_goods();
                    found = true;

                    break;
                }
            }

            if (!found)
            {
                cout << "\nBarang tidak ditemukan." << endl;
            }
        }

        else if (next == '2')
        {
            cout << "Masukkan Tanggal (YYYY-MM-DD): ";
            cin >> search_key;

            bool found = false;
            for (const auto &goods : incoming_goods_list)
            {
                if (goods.get_date() == search_key)
                {
                    goods.display_goods();
                    found = true;
                }
            }

            if (!found)
            {
                cout << "\nTidak ada barang yang ditemukan pada tanggal tersebut." << endl;
            }
        }

        else
        {
            cout << "\nPilihan tidak valid." << endl;
        }
        cout << "\nTekan 1 untuk mencari barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pemasukan Barang: ";
        cin >> next;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::update_incoming_goods() 
{
    string goods_id;
    char next;
    do
    {
        clearScreen();
        cout << "Masukkan ID Barang yang Akan Diubah: ";
        cin >> goods_id;

        bool found = false;
        for (auto &goods : incoming_goods_list)
        {
            if (goods.get_goods_code() == goods_id) 
            {
                string goods_name, goods_category, date, supplier;
            
                int stock_amount;
                cout << "\nMasukkan Nama Barang Baru: ";
                cin .ignore();
                getline(cin, goods_name);
                cout << "Masukkan Kategori Barang Baru: ";
                cin .ignore();
                getline(cin, goods_category);
                
                while (true)
                {
                    cout << "Masukkan Jumlah Stok Baru: ";
                    cin >> stock_amount;

                    if (cin .fail())
                    {
                        cin .clear(); // Menghapus flag kesalahan
                        cin .ignore(numeric_limits<streamsize>::max(), '\n'); // Mengabaikan input yang salah
                        cout << "\nInput tidak valid. Silahkan masukkan angka." << endl;
                    }

                    else
                    {
                        break; // Input valid, keluar dari loop
                    }
                }

                do
                {
                    cout << "Masukkan Tanggal Baru (YYYY-MM-DD): ";
                    cin >> date;

                    if (!isValidDate(date))
                    {
                        cout << "\nFormat tanggal tidak valid atau hari/bulan yang diinput di luar rentang. Silakan coba lagi." << endl;
                    }
                } while (!isValidDate(date));

                cout << "Masukkan Supplier Baru: ";
                cin .ignore();
                getline(cin, supplier);

                goods .set_goods_name(goods_name);
                goods .set_goods_category(goods_category);
                goods .set_stock_amount(stock_amount);
                goods .set_data(date);
                goods .set_supplier(supplier);

                cout << "Incoming goods updated successfully!" << endl;
                found = true;

                break;
            } 
        }

        if (!found)
        {
            cout << "Barang tidak ditemukan." << endl;
        }
        cout << "\nTekan 1 untuk mengubah barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pemasukan Barang: ";
        cin >> next;
        cin .ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::delete_incoming_goods() 
{
    string goods_id;
    char next;
    
    do
    {
        clearScreen();
        cout << "Masukkan ID Barang yang Akan Dihapus: ";
        cin >> goods_id;

        bool found = false;
        for (auto it = incoming_goods_list .begin(); it != incoming_goods_list .end(); ++it) 
        {
            if (it ->get_goods_code() == goods_id) 
            {
                incoming_goods_list .erase(it);
                cout << "Barang berhasil dihapus!" << endl;
                found = true;

                break;
            }
        }

        if (!found)
        {
            cout << "Barang tidak ditemukan." << endl;
        }
        cout << "\nTekan 1 untuk menghapus barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pemasukan Barang: ";
        cin >> next;
        cin .ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::manage_incoming_goods() 
{
    clearScreen();
    int choices;
    do {
        clearScreen();
        cout << "\n===============================================" << endl;
        cout << "        MENU MANAJEMEN PEMASUKAN BARANG" << endl;
        cout << "===============================================" << endl;
        cout << "1. Tambah Data Pemasukan Barang Baru" << endl;
        cout << "2. Tampilkan Semua Data Pemasukan Barang" << endl;
        cout << "3. Cari Data Pemasukan Barang" << endl;
        cout << "4. Ubah Data Pemasukan Barang" << endl;
        cout << "5. Hapus Data Pemasukan Barang" << endl;
        cout << "6. Kembali ke Menu utama" << endl;
        cout << "Pilih opsi: ";
        cin >> choices;

        switch (choices) 
        {
            case 1:
                clearScreen();
                add_incoming_goods();
                break;
            case 2:
                clearScreen();
                view_incoming_goods();
                break;
            case 3:
                clearScreen();
                find_incoming_goods();
                break;
            case 4:
                clearScreen();
                update_incoming_goods();
                break;
            case 5:
                clearScreen();
                delete_incoming_goods();
                break;
            case 6:
                clearScreen();
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    } 
    
    while (choices != 6);
}

/*class outgoing_inventory_management 
{
private:
//    vector <outgoing_goods>outgoing_goods_list;

    outgoing_goods *find_outgoing_goods_list(const string &goods_id) 
    {
        for (auto& goods : outgoing_goods_list) 
        {
            if (goods .get_goods_code() == goods_id) 
            {
                return &goods;
            }
        }
        
        return nullptr;
    }

    bool isValidDate(const string &date) 
    {
        // Check format YYYY-MM-DD
        regex datePattern(R"((\d{4})-(\d{2})-(\d{2}))");
        smatch match;
    
        if (!regex_match(date, match, datePattern)) 
        {
            return false;
        }

        int year = stoi(match[1] .str());
        int month = stoi(match[2] .str());
        int day = stoi(match[3] .str());

        if (month < 1 || month > 12) 
        {
            return false;
        }

        vector <int>daysInMonth = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        // Check for leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
        {
            daysInMonth[1] = 29;
        }

        if (day < 1 || day > daysInMonth[month - 1]) 
        {
            return false;
        }

        return true;
    }

public:
    void add_outgoing_goods();
    void view_outgoing_goods();
    void find_outgoing_goods();
    void update_outgoing_goods();
    void delete_outgoing_goods();
    void manage_outgoing_goods();
};*/

void inventory_management::add_outgoing_goods()
{
   string goods_id, date, destination;
   char next;
   int stock_amount;

   do
   {
        clearScreen();
        // Loop untuk memastikan ID barang yang valid
        incoming_goods* incoming_goods = nullptr;
        do
        {      
            cout << "Masukkan ID Barang untuk Pengeluaran: ";
            cin >> goods_id;

            incoming_goods = find_incoming_goods_list(goods_id);

            if (!incoming_goods) 
            {
                cout << "Barang dengan ID " << goods_id << " tidak ditemukan di daftar Barang Pemasukan.\n" << endl;
            }

            else
            {
                cout << "Barang ditemukan:\n";
                incoming_goods ->display_goods();
            }
        } while (!incoming_goods);

        string goods_name = incoming_goods ->get_goods_name();
        string goods_category = incoming_goods ->get_goods_category();

        do
        {
            cout << "Masukkan Jumlah Stok: ";
            cin >> stock_amount;

            if (cin .fail() || stock_amount > incoming_goods ->get_current_stock_amount() || stock_amount <= 0) 
            {
                cin .clear();
                cin .ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Jumlah yang salah. Tolong masukkan jumlah yang benar.\n";
            } 
            
            else 
            {
                break;
            }
        } while (true);

        do
        {
            cout << "Masukkan Tanggal (YYYY-MM-DD): ";
            cin >> date;

            if (!isValidDate(date))
            {
                cout << "\nFormat tanggal tidak valid atau hari/bulan yang diinput di luar rentang. Silakan coba lagi.\n";
            }
        } while (!isValidDate(date));

        cout << "Masukkan Tujuan Pengiriman: ";
        cin .ignore();
        getline(cin, destination);

        outgoing_goods newGoods(goods_id, goods_name, goods_category, stock_amount, date, destination);
        outgoing_goods_list.push_back(newGoods);

        // Update jumlah stock
        incoming_goods->set_current_stock_amount(incoming_goods ->get_current_stock_amount() - stock_amount);

        cout << "\nBarang pengeluaran berhasil ditambahkan!" << endl;
        cout << "Tekan 1 untuk menambahkan barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pengeluaran Barang: ";
        cin >> next;
        cin .ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::view_outgoing_goods()
{
    char next;
    do
    {
        if (outgoing_goods_list.empty())
        {
            cout << "Tidak ada barang dalam daftar." << endl;
        }
        else
        {
            // Pilihan sorting
            clearScreen();
            int sort_option;
            cout << "=== Daftar Barang Pengeluaran ===\n";
            cout << "Pilih kriteria sorting:\n";
            cout << "1. Berdasarkan ID Barang\n";
            cout << "2. Berdasarkan Tanggal\n";
            cout << "3. Berdasarkan Jumlah Stok\n";
            cout << "Masukkan pilihan (1-3): ";
            cin >> sort_option;

            // Sorting berdasarkan pilihan
            switch (sort_option) 
            {
                case 1:
                    clearScreen();
                    sort(outgoing_goods_list .begin(), outgoing_goods_list .end(),
                        [](const outgoing_goods &a, const outgoing_goods &b) 
                        {
                            return a .get_goods_code() < b .get_goods_code();
                        });
                    break;
                case 2:
                    clearScreen();
                    sort(outgoing_goods_list .begin(), outgoing_goods_list .end(),
                        [](const outgoing_goods &a, const outgoing_goods &b) 
                        {
                            return a .get_date() < b .get_date();
                        });
                    break;
                case 3:
                    clearScreen();
                    sort(outgoing_goods_list .begin(), outgoing_goods_list .end(),
                        [](const outgoing_goods &a, const outgoing_goods &b) 
                        {
                            return a .get_stock_amount() < b .get_stock_amount();
                        });
                    break;
                default:
                    clearScreen();
                    cout << "Pilihan tidak valid. Menampilkan tanpa sorting.\n";
                    break;
            }

            for (const auto &goods : outgoing_goods_list)
            {
                goods.display_goods();
            }
        }
        cout << "\nKetik 1 untuk sorting lagi, atau klik bebas untuk balik ke Menu Manajemen Pengeluaran Barang.";
        cin >> next;

        while (getchar() != '\n');
    } while (next == '1');
}

void inventory_management::find_outgoing_goods()
{
    string search_key;
    char next;
    do
    {
        clearScreen();
        cout << "Cari berdasarkan:\n1. ID Barang\n2. Tanggal\nPilih opsi: ";
        cin >> next;

        if (next == '1')
        {
            cout << "Masukkan ID Barang yang dicari: ";
            cin >> search_key;

            bool found = false;
            for (const auto &goods : outgoing_goods_list)
            {
                if (goods.get_goods_code() == search_key)
                {
                    goods.display_goods();
                    found = true;
                }
            }

            if (!found)
            {
                cout << "\nBarang tidak ditemukan." << endl;
            }
        }
        else if (next == '2')
        {
            cout << "Masukkan Tanggal (YYYY-MM-DD): ";
            cin >> search_key;

            bool found = false;
            for (const auto &goods : outgoing_goods_list)
            {
                if (goods.get_date() == search_key)
                {
                    goods.display_goods();
                    found = true;
                }
            }

            if (!found)
            {
                cout << "\nTidak ada barang yang ditemukan pada tanggal tersebut." << endl;
            }
        }
        else
        {
            cout << "\nPilihan tidak valid." << endl;
        }

        cout << "\nTekan 1 untuk mencari barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pengeluaran Barang: ";
        cin >> next;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::update_outgoing_goods()
{
    string goods_id;
    char next;
    do
    {
        clearScreen();
        cout << "Masukkan ID Barang yang Akan Diubah: ";
        cin >> goods_id;
        

        bool found = false;
        for (auto &goods : outgoing_goods_list)
        {
            if (goods.get_goods_code() == goods_id)
            {
                // Barang pengeluaran ditemukan, sekarang cari barang pemasukan terkait
                incoming_goods* incoming_goods = find_incoming_goods_list(goods_id);
                if (!incoming_goods)
                {
                    cout << "Barang dengan ID " << goods_id << " tidak ditemukan di daftar Barang Pemasukan.\n" << endl;
                    continue;
                }
                cout << "Barang ditemukan:\n";
                incoming_goods ->display_goods();

                string goods_name = incoming_goods ->get_goods_name();
                string goods_category = incoming_goods ->get_goods_category();
                string date, destination;
                int stock_amount;

                while (true)
                {
                    cout << "Masukkan Jumlah Stok Baru: ";
                    cin >> stock_amount;
                    
                    // Hitung stok total yang akan tersedia setelah perubahan
                    int total_stock = incoming_goods ->get_current_stock_amount() + goods .get_stock_amount();
                    if (cin .fail() || stock_amount > total_stock || stock_amount <= 0) 
                    {
                        cin .clear();
                        cin .ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Jumlah yang salah. Tolong masukkan jumlah yang benar.\n";
                    } 
                    
                    else 
                    {
                        // Perbarui stok barang pemasukan
                        incoming_goods ->set_current_stock_amount(total_stock - stock_amount);
                        break;
                    }
                }

                do
                {
                    cout << "Masukkan Tanggal Baru (YYYY-MM-DD): ";
                    cin >> date;

                    if (!isValidDate(date))
                    {
                        cout << "\nFormat tanggal tidak valid atau hari/bulan yang diinput di luar rentang. Silakan coba lagi." << endl;
                    }
                } while (!isValidDate(date));

                cout << "Masukkan Tujuan Pengiriman Baru: ";
                cin.ignore();
                getline(cin, destination);

                goods.set_goods_name(goods_name);
                goods.set_goods_category(goods_category);
                goods.set_stock_amount(stock_amount);
                goods.set_data(date);
                goods.set_destination(destination);

                cout << "Barang pengeluaran berhasil diperbarui!" << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "Barang pengeluaran dengan ID " << goods_id << " tidak ditemukan." << endl;
        }

        cout << "\nTekan 1 untuk mengubah barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pengeluaran Barang: ";
        cin >> next;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::delete_outgoing_goods()
{
    string goods_id;
    char next;

    do
    {
        clearScreen();
        cout << "Masukkan ID Barang yang Akan Dihapus: ";
        cin >> goods_id;

        bool found = false;
        for (auto it = outgoing_goods_list.begin(); it != outgoing_goods_list.end(); ++it)
        {
            if (it->get_goods_code() == goods_id)
            {
                incoming_goods *incoming_goods = find_incoming_goods_list(goods_id);
                
                if (!incoming_goods)
                {
                    cout << "Barang dengan ID " << goods_id << " tidak ditemukan di daftar Barang Pemasukan.\n" << endl;
                    continue;
                }
                // Mengembalikan jumlah stok ke barang pemasukan
                incoming_goods->set_current_stock_amount(incoming_goods->get_current_stock_amount() + it->get_stock_amount());
                
                outgoing_goods_list.erase(it);
                cout << "Barang berhasil dihapus!" << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "Barang tidak ditemukan." << endl;
        }

        cout << "\nTekan 1 untuk menghapus barang lagi, atau tekan karakter lain untuk kembali ke Menu Manajemen Pengeluaran Barang: ";
        cin >> next;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer
    } while (next == '1');
}

void inventory_management::manage_outgoing_goods()
{
   clearScreen();
   int choices;
   do
   {
       clearScreen();
       cout << "\n===============================================" << endl;
       cout << "        MENU MANAJEMEN PENGELUARAN BARANG" << endl;
       cout << "===============================================" << endl;
       cout << "1. Tambah Data Pengeluaran Barang Baru" << endl;
       cout << "2. Tampilkan Semua Data Pengeluaran Barang" << endl;
       cout << "3. Cari Data Pengeluaran Barang" << endl;
       cout << "4. Ubah Data Pengeluaran Barang" << endl;
       cout << "5. Hapus Data Pengeluaran Barang" << endl;
       cout << "6. Kembali ke Menu Utama" << endl;
       cout << "Pilih opsi: ";
       cin >> choices;

       switch (choices)
       {
       case 1:
           clearScreen();
           add_outgoing_goods();
           break;
       case 2:
           clearScreen();
           view_outgoing_goods();
           break;
       case 3:
           clearScreen();
           find_outgoing_goods();
           break;
       case 4:
           clearScreen();
           update_outgoing_goods();
           break;
       case 5:
           clearScreen();
           delete_outgoing_goods();
           break;
       case 6:
           clearScreen();
           break;
       default:
           cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
       }
   } while (choices != 6);
}

// REPORT
class report_management 
{
private:
//    vector<incoming_goods> incoming_goods_list;
//    vector<outgoing_goods> outgoing_goods_list;

public:
    void report();
    void report_incoming_goods();
    void report_outgoing_goods();
    void report_stock_goods();
};

void report_management::report_incoming_goods() 
{
    clearScreen();
    cout << "=== Laporan Barang Masuk ===\n";
    
    for (const auto &goods : incoming_goods_list) 
    {
        goods.display_goods();
        //cout << "Jumlah: " << goods .get_original_stock_amount() << endl; // Original stock amount
    }
    cout << "\nTekan enter untuk kembali ke Menu Laporan.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void report_management::report_outgoing_goods() 
{
    clearScreen();
    cout << "=== Laporan Barang Keluar ===\n";
    
    for (const auto &goods : outgoing_goods_list) 
    {
        goods.display_goods();
    }
    cout << "\nTekan enter untuk kembali ke Menu Laporan.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void report_management::report_stock_goods() 
{
    clearScreen();
    cout << "=== Laporan Stok Barang ===\n";
    cout << "Barang Masuk:\n";
    
    for (const auto &goods : incoming_goods_list) 
    {
        cout << "ID: " << goods. get_goods_code() << endl;
        cout << "Nama: " << goods. get_goods_name() << endl;
        cout << "Jumlah: " << goods. get_current_stock_amount() << endl << endl; // Current stock amount
    }
    cout << "\nBarang Keluar:\n";
    
    for (const auto &goods : outgoing_goods_list) 
    {
        cout << "ID: " << goods. get_goods_code() << endl;
        cout << "Nama: " << goods. get_goods_name() << endl;
        cout << "Jumlah: " << goods. get_stock_amount() << endl << endl;
    }
    cout << "\nTekan enter untuk kembali ke Menu Laporan.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void report_management::report() 
{
    char next;
    do {
        clearScreen();
        cout << "=== Laporan Barang ===\n";
        cout << "1. Laporan Barang Masuk\n";
        cout << "2. Laporan Barang Keluar\n";
        cout << "3. Laporan Stok Barang\n";
        cout << "4. Kembali ke Menu Utama\n";
        cout << "Pilih menu (1-4): ";
        cin >> next;

        switch (next) {
            case '1':
                report_incoming_goods();
                break;
            case '2':
                report_outgoing_goods();
                break;
            case '3':
                report_stock_goods();
                break;
            case '4':
                return;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
                break;
        }
    } while (next != '4');
}
// END REPORT

void main_menu() {
    inventory_management inventory_management;
    report_management report;

    int choices;
    do {
        clearScreen();
        cout << "\n===============================================" << endl;
        cout << "                  MENU UTAMA" << endl;
        cout << "===============================================" << endl;
        cout << "1. Menu Manajemen Pemasukan Barang" << endl;
        cout << "2. Menu Manajemen Pengeluaran Barang" << endl;
        cout << "3. Laporan" << endl;
        cout << "4. Keluar dari Program" << endl;
        cout << "Pilih opsi: ";
        cin >> choices;

        switch (choices) {
            case 1:
                inventory_management.manage_incoming_goods();
                break;
            case 2:
                inventory_management.manage_outgoing_goods();
                break;
            case 3:
            //    clearScreen();
                report.report();
                break;
            case 4:
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    } while (choices != 4);
}

int main() {
    main_menu();

    return 0;
}
