#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void save_all();
void load_all();
void listPlots(int city);
int selling_plot(int CITY);
int check_info(int CITY, int PLOT);
int buying_plot(int city);
int critera_checker(int city);
void index_check_plot(int city);
bool confirm_booking(int city, int plot_index);
double get_valid_size();
char get_valid_unit();

const int NUM_CITIES = 20;
int counting[NUM_CITIES] = {0};

struct info {
    long long client_price;
    long long seller_price;
    string phone;
    double size_of_plot;
    int type_of_plot;
    bool avail;
    string address;
};

info selling[NUM_CITIES][100];
info buying;

const string CITY_NAMES[NUM_CITIES] = {
    "Islamabad", "Rawalpindi", "Lahore", "Karachi", "Sargodha",
    "Abottabad", "Multan", "Faisalabad", "Peshawar", "Quetta",
    "Sialkot", "Gujranwala", "Hyderabad", "Sukkur", "Bahawalpur",
    "Jhelum", "Murree", "Mardan", "Gujrat", "Larkana"
};

double convert_to_marla(double size, char unit) {
    if (unit == 'k' || unit == 'K') return size * 20;
    return size;
}

void display_city_table() {
    cout << "\nWe have the following cities in our portfolio, which city do you want to see plots in\n";
    cout << "\nCities (Choose index of the option):\n";
    const int COLUMNS = 4;
    for (int i = 0; i < NUM_CITIES; ++i) {
        cout << (i + 1) << " " << CITY_NAMES[i] << "\t";
        if ((i + 1) % COLUMNS == 0) cout << "\n";
    }
    cout << "\nChoose index of the option: ";
}

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
                 << "\"" << selling[city][x].address << "\"" << "\n";
        }
    }
    file.close();
}

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

double get_valid_size() {
    double size;
    while (true) {
        if (!(cin >> size) || size <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a positive number: ";
            continue;
        }
        break;
    }
    return size;
}

char get_valid_unit() {
    char unit;
    while (true) {
        cin >> unit;
        if (unit != 'm' && unit != 'M' && unit != 'k' && unit != 'K') {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid unit. Enter 'm' for Marla or 'k' for Kanal: ";
            continue;
        }
        break;
    }
    return unit;
}

int check_info(int CITY, int PLOT) {
    if (PLOT >= 100) {
        cout << "City capacity full.\n";
        return 1;
    }
    int temp_type;
    while (true) {
        cout << "\nSelect plot type:\n1) Residential\n2) Commercial\nType index of the option (1 or 2): ";
        if (!(cin >> temp_type) || (temp_type != 1 && temp_type != 2)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        break;
    }
    selling[CITY][PLOT].type_of_plot = temp_type;

    cout << "Enter size (number followed by unit m for marla, k for kanal, e.g., 5 k or 10 m): ";
    double size = get_valid_size();
    char unit = get_valid_unit();
    selling[CITY][PLOT].size_of_plot = convert_to_marla(size, unit);

    cin.ignore();
    cout << "Enter detailed address: ";
    getline(cin, selling[CITY][PLOT].address);

    cout << "Enter your phone number: ";
    cin >> selling[CITY][PLOT].phone;

    cout << "Enter minimum asking price (PKR): ";
    while (!(cin >> selling[CITY][PLOT].seller_price) || selling[CITY][PLOT].seller_price <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter a positive number for price: ";
    }

    selling[CITY][PLOT].avail = true;
    cout << "\nYour plot has been added.\n";
    return 0;
}

int selling_plot(int CITY) {
    if (CITY < 0 || CITY >= NUM_CITIES) {
        cout << "Invalid city selection.\n";
        return 1;
    }
    if (counting[CITY] >= 100) {
        cout << "City capacity full.\n";
        return 1;
    }
    check_info(CITY, counting[CITY]);
    counting[CITY]++;
    save_all();
    return 0;
}

int critera_checker(int city) {
    if (city < 0 || city >= NUM_CITIES) return 0;
    int count = counting[city];
    for (int x = 0; x < count; x++) {
        if (!selling[city][x].avail) continue;
        long price_diff = selling[city][x].seller_price - buying.client_price;
        bool price_ok = (buying.client_price >= selling[city][x].seller_price) || (price_diff < 50000);
        bool type_ok = (buying.type_of_plot == selling[city][x].type_of_plot);
        bool size_ok = (buying.size_of_plot <= selling[city][x].size_of_plot);
        if (price_ok && type_ok && size_ok) {
            if (confirm_booking(city, x)) return 1;
        }
    }
    cout << "\nNo matching plot found.\n";
    return 0;
}

int buying_plot(int city) {
    cout << "\nEnter maximum price you can pay: ";
    while (!(cin >> buying.client_price) || buying.client_price <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter a positive number for price: ";
    }

    int temp_type;
    while (true) {
        cout << "\nSelect required plot type:\n1) Residential\n2) Commercial\nType index of the option (1 or 2): ";
        if (!(cin >> temp_type) || (temp_type != 1 && temp_type != 2)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        break;
    }
    buying.type_of_plot = temp_type;

    cout << "Enter required size (number followed by unit m for marla, k for kanal, e.g., 2 k or 10 m): ";
    double size = get_valid_size();
    char unit = get_valid_unit();
    buying.size_of_plot = convert_to_marla(size, unit);

    if (city >= 0 && city < NUM_CITIES)
        critera_checker(city);
    else
        cout << "Invalid city.\n";
    return 0;
}

bool confirm_booking(int city, int plot_index) {
    if (city < 0 || city >= NUM_CITIES || plot_index < 0 || plot_index >= counting[city]) {
        cout << "Invalid plot selection.\n";
        return false;
    }
    info &plot = selling[city][plot_index];
    if (!plot.avail) {
        cout << "This plot is already booked.\n";
        return false;
    }
    char confirm;
    double kanal = plot.size_of_plot / 20.0;
    cout << "\n--- Plot Details ---\n";
    cout << "Price: " << plot.seller_price << endl;
    cout << "Size: " << plot.size_of_plot << " marla (" << kanal << " kanal)\n";
    cout << "Type: " << (plot.type_of_plot == 1 ? "Residential" : "Commercial") << endl;
    cout << "Address: " << plot.address << endl;
    cout << "\nAre you sure you want to buy this plot? (y/n): ";
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        plot.avail = false;
        save_all();
        cout << "Plot successfully booked! You can contact the seller.\n";
        return true;
    } else {
        cout << "Booking cancelled.\n";
        return false;
    }
}

void index_check_plot(int city) {
    if (city < 0 || city >= NUM_CITIES) {
        cout << "Invalid city selection.\n";
        return;
    }
    if (counting[city] == 0) {
        cout << "No plots available in this city.\n";
        return;
    }
    int ind;
    bool booking_successful = false;
    while (!booking_successful) {
        cout << "Enter the plot index you want to buy (1 to "
             << counting[city] << ") or enter 0 to cancel: ";
        if (!(cin >> ind)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        if (ind == 0) {
            cout << "Plot selection cancelled.\n";
            return;
        }
        if (ind < 1 || ind > counting[city]) {
            cout << "Index not available. Please enter a number between 1 and " << counting[city] << ".\n";
            continue;
        }
        int plot_index = ind - 1;
        if (!selling[city][plot_index].avail) {
            cout << "Sorry, plot (Index " << ind << ") is already booked. Please choose another index.\n";
            continue;
        }
        if (confirm_booking(city, plot_index)) booking_successful = true;
    }
}

void listPlots(int city) {
    if (city >= 0 && city < NUM_CITIES) {
        cout << "\n--- Available Plots in " << CITY_NAMES[city] << " (" << city + 1 << ") ---\n";
        bool found_plots = false;
        cout << "----------------------------------------------------------------------------------------------------------------------\n";
        cout << "| Index | Price (PKR) | Size (Marla) | Size (Kanal) | Type        | Address                                                     |\n";
        cout << "----------------------------------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < counting[city]; ++i) {
            if (selling[city][i].avail) {
                found_plots = true;
                double marla = selling[city][i].size_of_plot;
                double kanal = marla / 20.0;
                cout << "| " << setw(5) << left << i + 1
                     << " | " << setw(11) << left << selling[city][i].seller_price
                     << " | " << setw(12) << left << marla
                     << " | " << setw(12) << left << fixed << setprecision(2) << kanal
                     << " | " << setw(11) << left << (selling[city][i].type_of_plot == 1 ? "Residential" : "Commercial")
                     << " | " << setw(57) << left << selling[city][i].address.substr(0, 57) << " |\n";
            }
        }
        cout << "----------------------------------------------------------------------------------------------------------------------\n";
        if (!found_plots) cout << "No currently available plots listed for " << CITY_NAMES[city] << ".\n";
    } else cout << "Invalid city.\n";
}

int main() {
    load_all();
    int city;
    int last_listed_city = -1;
    bool running = true;
    bool index_selection_option = false;

    cout << "\n\t\t==================================================\n";
    cout << "\t\t\t\t  WELCOME TO PLOTIFY\n";
    cout << "\t\t==================================================\n";
    cout << "Plotify is a console-based real estate assistant, simplifying the process of listing "
         << "properties for sale and finding the perfect plot to match your requirements and budget.\n";

    while (running) {
        cout << "\n\t--- Main Menu ---\n";
        cout << "\t1) List available plots\n";
        cout << "\t2) Add a selling plot\n";
        cout << "\t3) Book a plot\n";
        cout << "\t4) Exit\n";
        cout << "\n\tChoose index of the option: ";

        int opt;
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n\tError: Please enter a valid number!\n";
            continue;
        }

        switch (opt) {
            case 1: {
                display_city_table();
                if (!(cin >> city)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\n\tError: Invalid city number!\n";
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
                if (!(cin >> city)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\n\tError: Invalid city number!\n";
                    break;
                }
                city -= 1;
                selling_plot(city);
                break;
            }
            case 3: {
                int option;
                if (index_selection_option && last_listed_city >= 0) {
                    cout << "\nYou can either book by index from the last city you saw list of, or search by criteria in any city.\n";
                    cout << "1) Enter index from the last listed city\n";
                    cout << "2) Find plot through exact criteria\n";
                    cout << "Choose index of the option: ";
                    if (!(cin >> option)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input\n";
                        break;
                    }
                    if (option == 1) index_check_plot(last_listed_city);
                    else if (option == 2) {
                        display_city_table();
                        if (!(cin >> city)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Error: Invalid city number!\n";
                            break;
                        }
                        city -= 1;
                        buying_plot(city);
                    } else cout << "Invalid command\n";
                } else {
                    display_city_table();
                    if (!(cin >> city)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Error: Invalid city number!\n";
                        break;
                    }
                    city -= 1;
                    buying_plot(city);
                }
                index_selection_option = false;
                break;
            }
            case 4:
                running = false;
                cout << "\n\tThank you for using Plotify!\n\tGoodbye!\n";
                break;
            default:
                cout << "\n\tInvalid menu option. Please choose 1, 2, 3, or 4.\n";
                break;
        }
    }
    return 0;
}
