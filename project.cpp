#include<iostream>
using namespace std;
int hosting(int city);
long int temp;
double temp_f;
int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, o = 0, a=0 , b=0, c=0, d=0, e=0, f=0, g=0, h=0;
int check_info(int p, int s);
int get_info(int city);
int critera_checker(int city2);
int get_count_of_plots(int city);

struct info
{
	long long lower;
	long long upper;
	long long phone;           //phone number 
	double size_of_plot;       //size marlay
	int type_of_plot;
	bool avail;               //plot available or no (true means yes and false means no)
};
info selling[7][100]; //We have 7 cities and capacity for 100 plots in each as of now;
info buying;
int hosting(int city)    //This function calls function on basis of the city
{
		switch (city) {
		case 0: check_info(0, i); i++; break;
		case 1: check_info(1, j); j++; break;
		case 2: check_info(2, k); k++; break;
		case 3: check_info(3, l); l++; break;
		case 4: check_info(4, m); m++; break;
		case 5: check_info(5, n); n++; break;
		case 6: check_info(6, o); o++; break;
		case 7: check_info(7, a); a++; break;
		case 8: check_info(8, b); b++; break;
		case 9: check_info(9, c); c++; break;
		case 10: check_info(10, d); d++; break;
		case 11: check_info(11, e); e++; break;
		case 12: check_info(12, f); f++; break;
		case 13: check_info(13, g); g++; break;
		case 14: check_info(14,h); h++; break;
		default: cout << "Invalid city";
		}
		return 0;
	}

int check_info(int p, int s) {   //This collects all necessary information about the plot
	if (s >= 6)
	{
		cout << "Error: Capacity reached for this city .\n";
		return 1;
	}
	cout << endl << "Enter the maximum amount for plot in pkr (non negotiable): ";
	cin >> temp;
	selling[p][s].upper = temp;
	cout << endl << "Enter the minimum amount for plot in pkr (non negotiable): ";
	cin >> temp;
	selling[p][s].lower = temp;
	cout << endl << "Enter your phone number for contact: ";
	cin >> temp;
	selling[p][s].phone = temp;
	cout << endl << "Type of plot:\n1)Residential\n2)Commercial\nSelect valid index for the property";
	cin >> temp;
	selling[p][s].type_of_plot = temp;
	cout << endl << "Enter the size of plot in marlay: ";
	cin >> temp_f;
	selling[p][s].size_of_plot = temp_f;
	selling[p][s].avail = true;
	cout << "\nYour plot details are: \nUpper price: " << selling[p][s].upper << "\nlower price: " << selling[p][s].lower;
	cout << "\nSize of property: " << selling[p][s].size_of_plot << "\nPhone number: " << selling[p][s].phone;
	cout << "\ntype of property " << selling[p][s].type_of_plot << "\navailable(true or false): " << selling[p][s].avail;
	s++;
	return 0;
}
int booking(int city)
{
	cout << endl << "Enter the maximum amount you are willing to pay for the plot (PKR): ";
	cin >> temp;
	buying.upper = temp;

	cout << endl << "Enter the minimum amount you are willing to pay for the plot (PKR): ";
	cin >> temp;
	buying.lower = temp;

	cout << endl << "Type of plot you want:\n1) Residential\n2) Commercial\nSelect valid index: ";
	cin >> temp;
	buying.type_of_plot = temp;

	cout << endl << "Enter the required plot size in marla: ";
	cin >> temp_f;
	buying.size_of_plot = temp_f;

	if (city >= 0 && city <= 6)  
		critera_checker(city);
	else
		cout << "Invalid city!";
	return 0;
}
int get_count_of_plots(int city) {
	switch (city) {
	case 0: return i;
	case 1: return j;
	case 2: return k;
	case 3: return l;
	case 4: return m;
	case 5: return n;
	case 6: return o;
	case 7: return a;
	case 8: return b;
	case 9: return c;
	case 10: return d;
	case 11: return e;
	case 12: return f;
	case 13: return g;
	case 14: return h;

	default: return 0;
	}
}

int critera_checker(int city2)
{
	int f= get_count_of_plots(city2);
		for (int x = 0; x < f; x++)   
		{
			if (selling[city2][x].avail)   // only check available plots
			{
				
				bool price_ok = (buying.lower >= selling[city2][x].lower &&
					buying.upper <= selling[city2][x].upper);

				bool type_ok = (buying.type_of_plot == selling[city2][x].type_of_plot);

				bool size_ok = (buying.size_of_plot <= selling[city2][x].size_of_plot);

				
				if (price_ok && type_ok && size_ok)
				{
					cout << "\nPlot Found!\n";
					cout << "Phone: " << selling[city2][x].phone << endl;
					cout << "Price Range: " << selling[city2][x].lower
						<< " - " << selling[city2][x].upper << endl;
					cout << "Size: " << selling[city2][x].size_of_plot << endl;

					selling[city2][x].avail = false;   // mark as booked
					return 1;   // stop after finding the first match
				}
			}
		}

		cout << "\nNo matching plot found in this city.\n";
		return 0;
	}
int main()
{
	int city;
	int chaos;
	bool n = true;
	cout << "====================WELCOME TO PLOTIFY======================\n";
	while (n) {

		cout << "\n1)SELLING\n2)BOOKING\n What do you wanna do ? (1 or 2)";
		cin >> chaos;

		switch (chaos) {
		case 1:
		{
			cout << "The following cities are in our current portfolio; where you can put your plot up for buying: ";
			cout << "\n1\tIslamabad\n2\tRawalpindi\n3\tLahore\n4\tKarachi\n5\tSargodha\n6\tAbottabad\n7\tMultan";
			cout << "\nEnter the index of the city your plot is in: ";
			cin >> city;
			city = city - 1;  //Index of array starts from zero
			hosting(city);
			break;
		}
		case 2:
		{
			cout << "Available plots are in following cities: ";
			cout << "\n1\tIslamabad\n2\tRawalpindi\n3\tLahore\n4\tKarachi\n5\tSargodha\n6\tAbottabad\n7\tMultan";
			cout << "\nEnter the index of the city your plot is in: ";
			cin >> city;
			city = city - 1; //index starts from 0 of an array
			booking(city);
			// jy gayys
			break;
		}
		default:
			cout << "Invalid option (press 1 or 2)";
		}
	
		cout << "\nDo you want to perform another action?(1 or 0)";
		cin >> n;
	}
	return 0;
}