#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Function Prototypes
void save_all();
void load_all();
void listPlots(int city);
int hosting(int city);
int check_info(int p, int s);
int booking(int city);
int critera_checker(int city2);
void index_check_plot(int city2);

// Constants
const int NUM_CITIES = 20; 
int counting[NUM_CITIES] = {0};

// Structures
struct info {
    long long client_price;
    long long seller_price;
    string phone;
    double size_of_plot;
    int type_of_plot; // 1=Residential, 2=Commercial
    bool avail;
    string address;
};

// Data
info selling[NUM_CITIES][100]; 
info buying;

// City names
const string CITY_NAMES[NUM_CITIES] = {
    "Islamabad", "Rawalpindi", "Lahore", "Karachi", "Sargodha", 
    "Abottabad", "Multan", "Faisalabad", "Peshawar", "Quetta", 
    "Sialkot", "Gujranwala", "Hyderabad", "Sukkur", "Bahawalpur", 
    "Jhelum", "Murree", "Mardan", "Gujrat", "Larkana"
};

// Display city menu
void display_city_table() {
    cout << "\nCities (Choose the corresponding number):\n";
    const int COLUMNS = 4;
    for (int i = 0; i < NUM_CITIES; ++i) {
        cout << (i + 1) << " " << CITY_NAMES[i] << "\t";
        if ((i + 1) % COLUMNS == 0) cout << "\n";
    }
    cout << "\nChoose: ";
}

// Save plots to file
void save_all() {
    ofstream file("plots_db.txt");
    for (int city = 0; city < NUM_CITIES; city++) {
        for (int x = 0; x < counting[city]; x++) {
            file << city << " "
                 << selling[city][x].seller_price << " "
                 << selling[city][x].phone << " "
                 << selling[city][x].size_of_plot << " "
                 << selling[city][x].type_of_plot << " "
                 << selling[city][x].avail << " "
                 << "\"" << selling[city][x].address << "\""
                 << "\n";
        }
    }
    file.close();
}

// Load plots from file
void load_all() {
    ifstream file("plots_db.txt");
    if (!file) return;

    int city;
    info temp;
    while (file >> city
                >> temp.seller_price >> temp.phone
                >> temp.size_of_plot >> temp.type_of_plot >> temp.avail) {
        if (city < 0 || city >= NUM_CITIES) continue;
        file.ignore();
        getline(file, temp.address);
        if (!temp.address.empty() && temp.address.front() == '"' && temp.address.back() == '"')
            temp.address = temp.address.substr(1, temp.address.size() - 2);

        selling[city][counting[city]] = temp;
        counting[city]++;
    }
    file.close();
}

// Seller input
int check_info(int p, int s) {
    if (s >= 100) {
        cout << "City capacity full.\n";
        return 1;
    }

    int temp_type;
    cout << "\nEnter minimum asking price (PKR): ";
    cin >> selling[p][s].seller_price;

    cout << "Enter your phone number: ";
    cin >> selling[p][s].phone;
    cin.ignore();

    cout << "Enter detailed address: ";
    getline(cin, selling[p][s].address);

    bool valid = false;
    while (!valid) {
        cout << "Type of plot (1=Residential, 2=Commercial): ";
        cin >> temp_type;
        if (temp_type != 1 && temp_type != 2)
            cout << "ENTER 1 OR 2! Try again.\n";
        else
            valid = true;
    }
    selling[p][s].type_of_plot = temp_type;

    cout << "Enter size (marla): ";
    cin >> selling[p][s].size_of_plot;

    selling[p][s].avail = true;
    cout << "\nYour plot has been added.\n";
    return 0;
}

// Hosting a plot
int hosting(int city) {
    if (city < 0 || city >= NUM_CITIES) {
        cout << "Invalid city selection.\n";
        return 1;
    }
    if (counting[city] >= 100) {
        cout << "City capacity full.\n";
        return 1;
    }
    check_info(city, counting[city]);
    counting[city]++;
    save_all();
    return 0;
}

// Booking a plot by criteria
int booking(int city) {
    cout << "\nEnter maximum price you can pay: ";
    cin >> buying.client_price;

    cout << "Type of plot (1=Residential, 2=Commercial): ";
    cin >> buying.type_of_plot;

    cout << "Enter required size (marla): ";
    cin >> buying.size_of_plot;

    if (city >= 0 && city < NUM_CITIES)
        critera_checker(city);
    else
        cout << "Invalid city.\n";

    return 0;
}

// Criteria checker
int critera_checker(int city2) {
    if (city2 < 0 || city2 >= NUM_CITIES) return 0;
    int count = counting[city2];

    for (int x = 0; x < count; x++) {
        if (!selling[city2][x].avail) continue;

        long price_diff = selling[city2][x].seller_price - buying.client_price;
        bool price_ok = (buying.client_price >= selling[city2][x].seller_price) || (price_diff < 50000);
        bool type_ok = (buying.type_of_plot == selling[city2][x].type_of_plot);
        bool size_ok = (buying.size_of_plot <= selling[city2][x].size_of_plot);

        if (price_ok && type_ok && size_ok) {
            cout << "\n--- Match Found in " << CITY_NAMES[city2] << " ---" << endl;
            cout << "Seller Contact: " << selling[city2][x].phone << endl;
            cout << "Plot Address: " << selling[city2][x].address << endl;
            cout << "Seller Price: " << selling[city2][x].seller_price << endl;
            cout << "Size: " << selling[city2][x].size_of_plot << " marla" << endl;

            selling[city2][x].avail = false;
            save_all();
            return 1;
        }
    }
    cout << "\nNo matching plot found.\n";
    return 0;
}

// Buy plot by index
void index_check_plot(int city2) {
    if (city2 < 0 || city2 >= NUM_CITIES) {
        cout << "Invalid city selection.\n";
        return;
    }
    if (counting[city2] == 0) {
        cout << "No plots available in this city.\n";
        return;
    }

    int ind;
    cout << "Enter the plot index you want to buy (1 to " << counting[city2] << "): ";
    cin >> ind;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter a number.\n";
        return;
    }

    ind -= 1;
    if (ind < 0 || ind >= counting[city2]) {
        cout << "Invalid plot index.\n";
        return;
    }
    if (!selling[city2][ind].avail) {
        cout << "This plot is already booked.\n";
        return;
    }

    char confirm;
    cout << "\nPlot Details:\n";
    cout << "Price: " << selling[city2][ind].seller_price << endl;
    cout << "Size: " << selling[city2][ind].size_of_plot << " marla\n";
    cout << "Type: " << (selling[city2][ind].type_of_plot == 1 ? "Residential" : "Commercial") << endl;
    cout << "Address: " << selling[city2][ind].address << endl;

    cout << "\nConfirm booking? (y/n): ";
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        selling[city2][ind].avail = false;
        save_all();
        cout << "Plot successfully booked!\n";
    } else {
        cout << "Booking cancelled.\n";
    }
}

// List plots
void listPlots(int city) {
    if (city < 0 || city >= NUM_CITIES) {
        cout << "Invalid city.\n";
        return;
    }

    cout << "\n--- Available Plots in " << CITY_NAMES[city] << " ---\n";
    bool found = false;
    for (int i = 0; i < counting[city]; ++i) {
        if (selling[city][i].avail) {
            found = true;
            cout << "Plot Index " << i + 1 << ": "
                 << "Price: " << selling[city][i].seller_price
                 << " | Size: " << selling[city][i].size_of_plot
                 << " | Type: " << (selling[city][i].type_of_plot == 1 ? "Residential" : "Commercial")
                 << " | Address: " << selling[city][i].address << endl;
        }
    }
    if (!found) cout << "No currently available plots in this city.\n";
}

// Main
int main() {
    load_all();
    int city;
    int last_listed_city = -1;
    bool running = true;
    bool index_selection_option = false;

    cout << "====================WELCOME TO PLOTIFY======================\n";

    while (running) {
        cout << "\n--- Main Menu ---\n"
             << "1) List available plots\n"
             << "2) Add a selling plot\n"
             << "3) Book a plot\n"
             << "4) Exit\n"
             << "Choose: ";

        int opt;
        cin >> opt;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Please enter a valid number!\n";
            continue;
        }

        switch (opt) {
            case 1: {
                display_city_table();
                cin >> city;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Error: Invalid city number!\n";
                    break;
                }
                city -= 1;
                listPlots(city);
                index_selection_option = true;
                last_listed_city = city;
                break;
            }
            case 2: {
                display_city_table();
                cin >> city;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Error: Invalid city number!\n";
                    break;
                }
                city -= 1;
                hosting(city);
                break;
            }
            case 3: {
                if (index_selection_option && last_listed_city >= 0) {
                    city = last_listed_city;

                    int option;
                    cout << "How do you want to buy a plot?\n"
                         << "1) Enter index from the plot list\n"
                         << "2) Find plot through exact criteria\n";
                    cin >> option;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input\n";
                        break;
                    }

                    if (option == 1)
                        index_check_plot(city);
                    else if (option == 2)
                        booking(city);
                    else
                        cout << "Invalid command\n";

                    index_selection_option = false;
                } else {
                    display_city_table();
                    cin >> city;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Error: Invalid city number!\n";
                        break;
                    }
                    city -= 1;
                    booking(city);
                    index_selection_option = false;
                }
                break;
            }
            case 4: {
                running = false;
                break;
            }
            default: {
                cout << "Invalid menu option.\n";
                break;
            }
        }
    }

    return 0;
}
