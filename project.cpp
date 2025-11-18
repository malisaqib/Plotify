#include<iostream>
using namespace std;
int hosting(int city);
long int temp;
double temp_f;
int i=0,j = 0,k = 0,l = 0,m = 0,n = 0,o = 0;
int check_info(int p, int s);

struct info
{
	long long lower;
	long long upper;
	long long phone;   //phone number 
	double size_of_plot; //size marlay
	int type_of_plot;
	bool avail; //plot available or no (true means yes and false means no)
};
info selling[7][6]; //We have 7 cities and capacity for 6 plots in each as of now;
int check_info(int p, int s) {
	if (s >= 6)
	{
		cout << "Error: Capacity reached for this city (Max 6 plots).\n";
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
	cout << "Your plot details are: \n upper price: " << selling[p][s].upper << "\nlower price: " << selling[p][s].lower;
	cout << "\nSize of property: " << selling[p][s].size_of_plot << "\nPhone number: " << selling[p][s].phone;
	cout << "\ntype of property " << selling[p][s].type_of_plot << "\navailable(true or false): " << selling[p][s].avail;
	s++;
	return 0;
}
int hosting(int city)
{
	switch (city) {
	case 0:check_info(0, i);break;
	case 1:check_info(1, j);break;
	case 2:check_info(2, k);break;
	case 3:check_info(3, l);break;
	case 4:check_info(4, m);break;
	case 5:check_info(5, n);break;
	case 6:check_info(6, o);break;
	default: cout << "Invalid city"; 
	}
	return 0;	
}
int main()
{
	int city;
	int chaos;
	cout << "HELLO! WELCOME TO PLOTIFY\n1)BOOKING\n2)SELLING\n What do you wanna do?(1 or 2)";
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
		break;
	}
	default:
		cout << "Invalid option (press 1 or 2)";
	}
	 return 0;
}