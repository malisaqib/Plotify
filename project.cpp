#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // Still needed for setprecision if used elsewhere

using namespace std;

// Function Prototypes
void save_all();
void load_all();
void listPlots(int city);
int hosting(int city);
int check_info(int p, int s);
int booking(int city);
int critera_checker(int city2);

// Total cities supported (Indices 0 to 19)
const int NUM_CITIES = 20; 
int counting[NUM_CITIES] = {0}; // All counters initialized to 0

struct info {
    long long client_price;
    long long seller_price;
    string phone;
    double size_of_plot;
    int type_of_plot; // 1=Residential, 2=Commercial
    bool avail;
    string address;
};

// Main data array (20 cities, 100 plots max each)
info selling[NUM_CITIES][100]; 
info buying;

// City Names for display
const string CITY_NAMES[NUM_CITIES] = {
    "Islamabad", "Rawalpindi", "Lahore", "Karachi", "Sargodha", 
    "Abottabad", "Multan", "Faisalabad", "Peshawar", "Quetta", 
    "Sialkot", "Gujranwala", "Hyderabad", "Sukkur", "Bahawalpur", 
    "Jhelum", "Murree", "Mardan", "Gujrat", "Larkana"
};

// --- Helper function to display the city menu simply ---
void display_city_table() {
    cout << "\nCities (Choose the corresponding number):\n";
    const int COLUMNS = 4;
    
    for (int i = 0; i < NUM_CITIES; ++i) {
        cout << (i + 1) << " " << CITY_NAMES[i] << "\t"; // Use a simple tab
        
        if ((i + 1) % COLUMNS == 0) {
            cout << "\n";
        }
    }
    cout << "\nChoose: ";
}

// -------------------------------
// Save ALL plots to file
// -------------------------------
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
                 << "\"" << selling[city][x].address << "\"" // Quoted address
                 << "\n";
        }
    }

    file.close();
}

// -------------------------------
// Load ALL plots from file
// -------------------------------
void load_all() {
    ifstream file("plots_db.txt");
    if (!file) return;

    int city;
    info temp;

    while (file >> city
                 >> temp.seller_price >> temp.phone
                 >> temp.size_of_plot >> temp.type_of_plot >> temp.avail)
    {
        if (city < 0 || city >= NUM_CITIES) continue;

        file.ignore(); // Consume the space before the quoted address
        getline(file, temp.address);

        // Remove quotes around the address loaded from file
        if (!temp.address.empty() && temp.address.front() == '"' && temp.address.back() == '"')
            temp.address = temp.address.substr(1, temp.address.size() - 2);

        selling[city][counting[city]] = temp;
        counting[city]++;
    }

    file.close();
}

// -------------------------------
// MAIN INFO ENTRY FOR SELLER
// -------------------------------
int check_info(int p, int s) {
    if (s >= 100) {
        cout << "City capacity full.\n";
        return 1;
    }

    int temp_type;
    bool working = true;

    cout << "\nEnter minimum asking price (PKR): ";
    cin >> selling[p][s].seller_price;

    cout << "Enter your phone number: ";
    cin >> selling[p][s].phone;

    cin.ignore(); // Clear buffer for getline

    cout << "Enter detailed address: ";
    getline(cin, selling[p][s].address);

    while (working) {
        cout << "Type of plot (1=Residential, 2=Commercial): ";
        cin >> temp_type;
        if (temp_type != 1 && temp_type != 2)
            cout << "ENTER 1 OR 2! Try again.\n";
        else
            working = false;
    }
    selling[p][s].type_of_plot = temp_type;

    cout << "Enter size (marla): ";
    cin >> selling[p][s].size_of_plot;

    selling[p][s].avail = true;
    cout << "\nYour plot has been added.\n";

    return 0;
}

// -------------------------------
// HOSTING: Add plot flow
// -------------------------------
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

// -------------------------------
// BUYING: Get criteria
// -------------------------------
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

// -------------------------------
// CRITERIA CHECKER
// -------------------------------
int critera_checker(int city2) {
    if (city2 < 0 || city2 >= NUM_CITIES) return 0;
    
    int count = counting[city2];

    for (int x = 0; x < count; x++) {

        if (!selling[city2][x].avail) continue;

        // Check 1: Price difference tolerance
        long price_diff = selling[city2][x].seller_price - buying.client_price;
        bool price_ok = 
            (buying.client_price >= selling[city2][x].seller_price) || (price_diff < 50000);

        bool type_ok = (buying.type_of_plot == selling[city2][x].type_of_plot);
        bool size_ok = (buying.size_of_plot <= selling[city2][x].size_of_plot);

        if (price_ok && type_ok && size_ok) {

            cout << "\n--- Match Found in " << CITY_NAMES[city2] << " ---" << endl;
            cout << "Seller Contact: " << selling[city2][x].phone << endl;
            cout << "Plot Address: " << selling[city2][x].address << endl;
            cout << "Seller Price: " << selling[city2][x].seller_price << endl;
            cout << "Size: " << selling[city2][x].size_of_plot << " marla" << endl;

            selling[city2][x].avail = false;
            save_all(); // Update availability status in file
            return 1;
        }
    }

    cout << "\nNo matching plot found.\n";
    return 0;
}

// -------------------------------
// LIST PLOTS (Option 1)
// -------------------------------
void listPlots(int city) {
    if (city >= 0 && city < NUM_CITIES) { 
        cout << "\n--- Available Plots in " << CITY_NAMES[city] << " (" << city + 1 << ") ---\n";
        
        bool found_plots = false;
        
        for (int i = 0; i < counting[city]; ++i) {
            if (selling[city][i].avail) {
                found_plots = true;
                cout << "Plot Index " << i+1 << ":\n"
                     << "  Price: " << selling[city][i].seller_price
                     << " | Size: " << selling[city][i].size_of_plot
                     << " | Type: " << (selling[city][i].type_of_plot == 1 ? "Residential" : "Commercial")
                     << " | Address: " << selling[city][i].address << '\n';
            }
        }
        if (!found_plots) {
            cout << "No currently available plots listed for " << CITY_NAMES[city] << ".\n";
        }
    } else {
        cout << "Invalid city.\n";
    }
}

// -------------------------------
// MAIN PROGRAM
// -------------------------------
int main() {
    load_all();

    int city;
    bool running = true;

    cout << "====================WELCOME TO PLOTIFY======================\n";

    while (running) {
        cout << "\n--- Main Menu ---\n1) List available plots\n2) Add a selling plot\n3) Book a plot\n4) Exit\nChoose: ";
        int opt;
        cin >> opt;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Please enter a valid number!\n";
            continue;
        }

        switch (opt) {
            case 1:
            case 2:
            case 3:
                display_city_table();
                cin >> city;
                if (cin.fail()) { // Check for failure after city input too
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Error: Invalid city number!\n";
                    break;
                }
                city -= 1; // Convert to 0-based index

                if (opt == 1) listPlots(city);
                else if (opt == 2) hosting(city);
                else if (opt == 3) booking(city);
                break;
                
            case 4:
                running = false; 
                break;
                
            default:
                cout << "Invalid menu option.\n";
        }
    }

    return 0;
}