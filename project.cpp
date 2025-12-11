#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void save_all(); 
void load_all(); 
void listPlots(int city);
int hosting(int city);
int check_info(int p, int s);
int booking(int city);
int critera_checker(int city2);

// counters for each city
int counting[]={0,0,0,0,0,0,0};

struct info {
    long long client_price;
    long long seller_price;
    string phone;
    double size_of_plot;
    int type_of_plot;
    bool avail;
	string address;
};

info selling[7][100];
info buying;

// -------------------------------
// Save ALL plots to ONE file
// -------------------------------
void save_all() {
    // This function now correctly saves the entire state of the selling array
    ofstream file("plots_db.txt");

    for (int city = 0; city < 7; city++) {
        int count = counting[city];

        for (int x = 0; x < count; x++) {
            file << city << " "
                 << selling[city][x].seller_price<< " "
                 << selling[city][x].phone << " "
                 << selling[city][x].size_of_plot << " "
                 << selling[city][x].type_of_plot << " "
                 << selling[city][x].address << " "
                 << selling[city][x].avail << "\n";
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
        selling[city][ counting[city] ] = temp; // store record
        counting[city]++;                       // increment correct city's counter
    }

    file.close();
}
// // -------------------------------
// MAIN INFO ENTRY FOR SELLER
// (save_all() is REMOVED from here)
// -------------------------------
int check_info(int p, int s) {
    if (s >= 100) {
        cout << "City capacity full.\n";
        return 1;
    }
    int temp;
	bool working=true;
    cout << "\nEnter the non negotiable minimum price for plot(PKR): ";
    cin >> selling[p][s].seller_price;

    cout << "Enter your phone number: ";
    cin >> selling[p][s].phone;

	cout<<"Enter address: ";
	cin >> selling[p][s].address;

	while(working)
	{    
		cout << "Type of plot (1=Residential,2=Commercial): ";
    cin >> temp;
	   if(temp!=1&&temp!=2)
	
	cout<<"ENTER 1 OR 2!, TRY AGAIN!";
	else 
	   working=false;
}
	selling[p][s].type_of_plot=temp;
	
    cout << "Enter size (marla): ";
    cin >> selling[p][s].size_of_plot;

    selling[p][s].avail = true;

    cout << "\nYour plot has been added.\n";

    return 0;
}
    int hosting(int city) {
    if (city < 0 || city > 6) {
        cout << "Invalid city.\n";
        return 1;
    }
   if(counting[city]>=100){
        cout<<"City capacity full.\n";
        return 1;
   }
   check_info(city, counting[city]);
   //incremment the counter for that city
   counting[city]++;  

    // SAVE THE DATA NOW that the new plot is in the array AND the counter is updated
    save_all(); 
    return 0;
}

// -------------------------------
// BUYING / BOOKING
// -------------------------------
int booking(int city) {
    cout << "\nEnter max price you can pay: ";
    cin >> buying.client_price;

    cout << "Type of plot (1=Residential,2=Commercial): ";
    cin >> buying.type_of_plot;

    cout << "Enter required size (marla): ";
    cin >> buying.size_of_plot;

    if (city >= 0 && city <= 6)
        critera_checker(city);
    else
        cout << "Invalid city.";

    return 0;
}

// -------------------------------
// CRITERIA CHECKER
// -------------------------------
int critera_checker(int city2) {
    int f = counting[city2];

    for (int x = 0; x < f; x++) {

        if (!selling[city2][x].avail) continue; //skips iteration where plot is not available
       long accomodate = selling[city2][x].seller_price-buying.client_price;   //to help user find plots slightly above his budget
        bool price_ok =
            (buying.client_price >= selling[city2][x].seller_price)||(accomodate<50000);

        bool type_ok =
            (buying.type_of_plot == selling[city2][x].type_of_plot);

        bool size_ok =
            (buying.size_of_plot <= selling[city2][x].size_of_plot);

        if (price_ok && type_ok && size_ok) {

            cout << "\nPlot Found!\n";
            cout << "Phone: " << selling[city2][x].phone << endl;
            cout << "Price Range: " << selling[city2][x].seller_price << endl;
            cout << "Size: " << selling[city2][x].size_of_plot << endl;

            selling[city2][x].avail = false;

            save_all(); // update file ()
            return 1;
        }
    }

    cout << "\nNo matching plot found.\n";
    return 0;
}
//PROGRAM THAT PRINTS ALL PLOTS IN A CITY:
void listPlots(int city) {
    if (city >= 0 && city <= 6) {
        cout << "\nCity " << city << " has " << counting[city] << " plots:\n";
        for (int i = 0; i < counting[city]; ++i) {
            cout << "Index " << i
                 << " | phone: " << selling[city][i].phone
                 << " | price: " << selling[city][i].seller_price
                 << " | size: " << selling[city][i].size_of_plot
                 << " | type: " << selling[city][i].type_of_plot
                 << " | address: " << selling[city][i].address
                 << " | available: " << (selling[city][i].avail ? "yes" : "no") << '\n';
        }
    }
    else {
        cout << "Invalid city.\n";
    }
}
// -------------------------------
// MAIN PROGRAM
// -------------------------------
int main() {
    load_all();  // load data at start

    int city, opt;
    bool running = true;

    cout << "====================WELCOME TO PLOTIFY======================\n";

    while (running) {
                cout << "\nMenu:\n1) List plots\n2) Add selling plot\n3) Book a plot\n4) Exit\nChoose: ";
        int opt;
        cin >> opt;

        if (cin.fail()) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Error: Please enter a valid number!\n";
    return 0; // or loop back to menu
                    }
        switch (opt) {
           case 1: {
                     cout << "Cities:\n1 Islamabad\n2 Rawalpindi\n3 Lahore\n4 Karachi\n5 Sargodha\n6 Abottabad\n7 Multan\nChoose: ";
                     cin  >> city;
                     city -= 1;
                       listPlots(city);
                    break;
           }     
            case 2: cout << "Cities:\n1 Islamabad\n2 Rawalpindi\n3 Lahore\n4 Karachi\n5 Sargodha\n6 Abottabad\n7 Multan\nChoose: ";
                cin >> city;
                city -= 1;
                hosting(city);
                break;
            case 3:    cout << "Cities:\n1 Islamabad\n2 Rawalpindi\n3 Lahore\n4 Karachi\n5 Sargodha\n6 Abottabad\n7 Multan\nChoose: ";
                cin >> city;
                city -= 1;
                booking(city);
                break;
            case 4: running = false; break;
            default: cout << "Invalid option.\n";
        }
    }

    return 0;
}
