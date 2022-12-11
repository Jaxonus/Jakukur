#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <locale>

using namespace std;
string 
//ko�a maszyny, chi, psi i mu											d�ugo�ci:
chi1="01010101010101010101010101010101010101010",						//41
chi2="0011001100110011001100110011001",									//31
chi3="00011100011100011100011100011",									//29
chi4="00001111000011110000111100",										//26
chi5="00000111110000011111000",											//23
mu1="0101010101010101010101010101010101010101010101010101010101010",	//61
mu2="0011001100110011001100110011001100110",							//37
psi1="0101010101010101010101010101010101010101010",						//43
psi2="00110011001100110011001100110011001100110011001",					//47
psi3="000111000111000111000111000111000111000111000111000",				//51
psi4="00001111000011110000111100001111000011110000111100001",			//53
psi5="00000111110000011111000001111100000111110000011111000001111",		//59
//stringi wpsiywane przez u�ytkownika i wypisywane przez program
input, output, 
//string bufora do odczytu tekstu
buffer; 
//zmienne ktore przechowuja: w kodowaniu - warto�ci na obecnej pozycji ko�a, w konfiguracji - rozmiar ko�a
int ic1, ic2, ic3, ic4, ic5, im1, im2, ip1, ip2, ip3, ip4, ip5, 
//indeksy k�
i = 0, j = 0, k = 0, 
//obecna warto�� kodu chi XOR psi
codekey, 
//obecna warto�� znaku zakodowanego 
encoded, 
//warto�� kontrolna do sterowania menu
control=0; 
//fstreamy do obs�ugi plik�w
ifstream infile;
ofstream outfile;
//mapa reprezentuj�ca ZMODYFIKOWANY kod ITA2 (orygina� zawiera znaki kontrolne kt�rych nie koduj�, ze wzgl�du na mo�liwo�� powstawania b��d�w)
map<char, int>ITA2mod{
	{'@',0x00},	{'e',0x01},	{'#',0x02},	{'a',0x03},	{' ',0x04},	{'s',0x05},	{'i',0x06},	{'u',0x07},
	{'%',0x08},	{'d',0x09},	{'r',0x0a},	{'j',0x0b},	{'n',0x0c},	{'f',0x0d},	{'c',0x0e},	{'k' ,0x0f},
	{'t',0x10},	{'z',0x11},	{'l',0x12},	{'w',0x13},	{'h',0x14},	{'y',0x15},	{'p',0x16},	{'q',0x17},
	{'o',0x18},	{'b',0x19},	{'g',0x1a},	{'^',0x1b},	{'m',0x1c},	{'x',0x1d},	{'v',0x1e},	{'*',0x1f},
	{'_',0x20},	{'3',0x21},	{'{',0x22},	{'-',0x23},	{'}',0x24},	{0x27,0x25},{'8',0x26},	{'7',0x27},
	{'<',0x28},	{'>',0x29},	{'4',0x2a},	{0x5c,0x2b},{',',0x2c},	{'!',0x2d},	{':',0x2e},	{'(',0x2f},
	{'5',0x30},	{'+',0x31},	{')',0x32},	{'2',0x33},	{'$',0x34},	{'6',0x35},	{'0',0x36},	{'1',0x37},
	{'9',0x38},	{'?',0x39},	{'&',0x3a},	{'|',0x3b},	{'.',0x3c},	{'/',0x3d},	{'=',0x3e},	{'`',0x3f}
};
//znajd� znak (klucz w mapie) na podstawie jego warto�ci
char find_mapkey(map<char,int>input, int value)
{
	for (auto it = input.begin(); it != input.end(); it++)
		if (it->second == value)
			return it->first;
}
//szybka konwersje znaku liczby na liczb� i vice versa, potrzebna gdy� sekwencje k� s� przechowywane jako string
int char2num(char in)
{
	return in - 48;
}
char num2char(int in)
{
	return in + 48;
}
//generacja klucza koduj�cego jako chi XOR psi
int calc_codekey(int c1,int c2,int c3,int c4,int c5,int p1,int p2,int p3,int p4,int p5)
{
	//warto�ci na kolejnych ko�ach chi/psi to kolejne bity klucza pi�ciobitowego
	int chifull = (c1 << 4) + (c2 << 3) + (c3 << 2) + (c4 << 1) + c5;
	int psifull = (p1 << 4) + (p2 << 3) + (p3 << 2) + (p4 << 1) + p5;
	//ko�cowy klucz to xor powy�szych warto�ci
	return chifull ^ psifull;
}
//szybka pomoc jakie znaki s� obs�ugiwane
void texthelp(map<char, int>input)
{
	int a = 1;
	cout << "Dopuszcalne znaki (plus spacja):\n";
	for (auto it = input.begin(); it != input.end(); it++)
	{
		cout << it->first<<" ";
		a++;
		//�eby �adnie to wygl�da�o, 14 znak�w na lini�
		if (a % 14 == 0)
			cout << "\n";
	}
	cout << "\n";
}
//kodowanie wiadomo�ci
void encode(string toencode)
{
	//reset indeks�w na ko�ach
	i = 0, j = 0, k = 0;
	for (i = 0; i < toencode.length(); i++)
	{
		//je�eli pojawia si� nowa linia, to wpisz na sztywno CR LF, bo ich znaki kontrolne nie s� kodowane
		if (toencode[i] == 13 && toencode[i + 1] == 10)
		{ 
			//wpisuje tylko LF, bo w windows domyslnie przed LF jest i tak wstawiany CR, nie wiem jak to mo�e wygl�da� w innych systemach
			output += 10;
			i++;
		}
		//inaczej koduj normalnie
		else 
		{
			//rotacja k�
			ic1 = char2num(chi1[i % 41]);
			ic2 = char2num(chi2[i % 31]);
			ic3 = char2num(chi3[i % 29]);
			ic4 = char2num(chi4[i % 26]);
			ic5 = char2num(chi5[i % 23]);
			im1 = char2num(mu1[i % 61]);

			im2 = char2num(mu2[k % 37]);
			//mu2 obraca si� tylko wtedy je�eli mu1 mia�o 1
			if (im1 == 1)k += 1;
			//psi obracaj� si� tylko wtedy gdy mu2 ma 1
			if (im2 == 1)j += 1;
			ip1 = char2num(psi1[j % 43]);
			ip2 = char2num(psi2[j % 47]);
			ip3 = char2num(psi3[j % 51]);
			ip4 = char2num(psi4[j % 53]);
			ip5 = char2num(psi5[j % 59]);
			//oblicz klucz, zakoduj znak, wpisz znak do tekstu
			codekey = calc_codekey(ic1, ic2, ic3, ic4, ic5, ip1, ip2, ip3, ip4, ip5);
			encoded = ITA2mod[toencode[i]] ^ codekey;
			output += find_mapkey(ITA2mod, encoded);
		}
	}
}
//opcje menu
void attract()
{
	cout << "\nCo chcesz zrobi�?\n";
	cout << "1 - Wy�wietl obs�ugiwane znaki\n";
	cout << "2 - Kr�tki opis maszyny Lorenza - historia i dzia�anie\n";
	cout << "3 - Autotest dzia�ania maszyny\n";
	cout << "4 - Poka� obecn� konfiguracj� k�\n";
	cout << "5 - Konfiguracja k� maszyny\n";
	cout << "6 - Kodowanie/Dekodowanie w konsoli\n";
	cout << "7 - Kodowanie/dekodowanie w pliku\n";
	cout << "8 - Zako�cz prac�\n";
}
//szybki test dzia�ania maszyny
void test()
{
	//zakoduj przyk�adow� wiadomo��
	input = "to jest test dzialania maszyny lorenza";
	encode(input);
	cout << "Tekst niezakodowany:\n" << input << "\nTekst zakodowany:\n" << output;
	//teraz jako input jest wiadomo�� zakodowana
	input = output;
	//z racji tego �e do output zawsze znaki s� appendowane, kasuj� obecn� zawarto�� output
	output.clear();
	//dekoduj zakodowan� przyk�adow�
	encode(input);
	cout << "\nTekst zdekodowany:\n" << output;
	//wyczy�� stringi, inputu teoretycznie nie trzeba, ale profilaktycznie nie zaszkodzi
	input.clear();
	output.clear();
}
//relatywnie szybki opis dzia�ania maszyny
void lorenzinfo()
{
	cout << "	By�a to wykorzystywana podczas IIW� przez niemiecki sztab generalny (Oberkommando das Wehrmacht, OKW) maszyna generuj�ca kod strumieniowy oparty na szyfrze Vernama, opartym na bitowych operacjach XOR.";
	cout << "W por�wnaniu do bardziej s�awnej Enigmy, maszyna Lorenza s�u�y�a do kodowania wiadomo�ci telegraficznych przesy�anych poprzez dalekopisy, do kt�rych by�a bezpo�rednio podpinana.";
	cout << "Maszyna lorenza bywa potocznie nazywana 'Tunny' (Tu�czyk), gdy� taki kryptonim zosta� jej nadany przez Brytyjskich cryptoanalityk�w, poniewa� w ich �argonie, niemieckie transmisje telegraficzne nazywano potocznie 'Fish', czyli ryby.\n";
	cout << "	Dzia�anie maszyny opiera si� kodowaniu znaku na podstawie kombinacji z�batek - 2 grupy po 5 z�batek koduj�cych (chi i psi) i 2 z�batki kontrolne (mu).";
	cout << "Ka�da z z�batek ma inn� ilo�� z�b�w - chi1 - 31,chi2 - 31,chi3 - 29,chi4 - 26,chi5 - 23,mu1 - 61,mu2 - 37,psi1 - 43,psi2 - 47,psi3 - 51,psi4 - 53,psi5 - 59. Ich d�ugo�ci to warto�ci wzgl�dnie pierwsze, co w teorii zapewnia bardzo du�y okres czasu zanim sekwencja si� powt�rzy.";
	cout << "Obecnie wpisywany znak jest kodowany poprzez operacj� XOR jego kodu znaku (opartego na standaryzowanym kodowaniu telegraficznym ITA2, swoistym prekursorze ASCII) z kluczem bitowym, kt�ry powsta� z XOR-a obecnej kombinacji warto�ci na ko�ach chi i psi.";
	cout << "Dekodowanie dzia�a tak samo, je�eli klucz jest taki sam jak podczas zakodowania to XOR zakodowanego znaku z kluczem daje nam oryginalny znak";
	cout << "Ko�a chi i mu1 przechodz� o jedn� pozycj� do przodu po zakodowaniu ka�dego znaku, ko�o mu2 tylko wtedy je�eli mu1 jest na warto�ci 1, a ko�a psi tylko gdy mu2 jest na warto�ci 1.\n";
	cout << "	Aby zapewni� nieprzywidywalno�� warto�ci koduj�cych, przed transmisj� nale�a�o skonfigurowa� ko�a. Kombinacje k� by�y przesy�ane mi�dzy stacjami jako sekwencja 12 liter, gdzie ka�da litera odpowiada�a pewnej sekwencji na kole w tzw. ksi��ce QEP, u�ywanej do konfiguracji";
	cout << "Oczywi�cie, podobnie jak w przypadku Enigmy, niedopatrzenia pod wzgl�dem przesy�u klucza konfiguracyjnego (gdy� tylko on by� niekodowany) przyczyni�y si� do zdekodowania dzia�ania Maszyny Lorenza przez wywiad Brytyjski mi�dzy Sierpniem '41 i Styczniem '42\n";
	cout << "	Poni�szy symulator nie symuluje w 100% dzia�anie oryginalnej maszyny, gdy� n.p. o ile kodowanie znak�w ITA2 jest emulowane, to nie w ca�o�ci - na potrzeb� stabilno�ci emulacja nie uwzgl�dnia znak�w kontrolnych.";
	cout << "Osobi�cie wa�niejsze by�o oddanie ducha dzia�ania maszyny ni� jej 100% rekonstrukcja\n";
	cout << "	Dodatkowo, ze wzgl�du na to, �e w ITA2 nie ma rozr�nienia na du�e i ma�e litery, ten program obs�uguje tylko ma�e litery (�eby nie trzyma� shifta czy wciska� caps lock)\n";
}
//konfiguracja jednego ko�a
string wheelconfig(string wheel,int wheelsize,string configseq) 
{
	//wyczy�� obecn� zawarto�� ko�a
	wheel.clear();
	//je�eli sekwencja jest za kr�tka, powiel j� dop�ki nie b�dzie wi�ksza b�d� r�wna od rozmiaru ko�a
	if (configseq.length() < wheelsize)
	{
		for (int m = configseq.length();m<wheelsize;m+=m)
		{
			configseq += configseq;
		}
	}
	//je�eli sekwencja jest za d�uga (bo taka by�a wpisana, lub taka si� wygenerowa�a powy�ej), przytnij koniec zeby byla w sam raz
	 if(configseq.length()>wheelsize)
	{
		configseq.erase(configseq.begin()+wheelsize, configseq.end());
	}
	 //skonfiguruj ko�o na podstawie sekwencji, zadzia�aj� wszystkie znaki ascii bo i tak s� konwertowane na 0 lub 1
	 for (int n = 0; n !=wheelsize; n++)
	 {
		 wheel+= num2char(configseq[n]%2);
	 }
	 return wheel;
}
//kofiguracja k� maszyny
void configwheels()
{
	cout << "\nKonfiguracja k�, podaj sekwencj� (zerojedynkowe lub alfanumeryczne)\n";
	ic1 = chi1.length();
	cout << "Dla chi 1:";
	cin >> input;
	chi1 = wheelconfig(chi1, ic1, input);
	ic2 = chi2.length();
	cout << "Dla chi 2:";
	cin >> input;
	chi2 = wheelconfig(chi2, ic2, input);
	ic3 = chi3.length();
	cout << "Dla chi 3:";
	cin >> input;
	chi3 = wheelconfig(chi3, ic3, input);
	ic4 = chi4.length();
	cout << "Dla chi 4:";
	cin >> input;
	chi4 = wheelconfig(chi4, ic4, input);
	ic5 = chi5.length();
	cout << "Dla chi 5:";
	cin >> input;
	chi5 = wheelconfig(chi5, ic5, input);
	im1 = mu1.length();
	cout << "Dla mu 1:";
	cin >> input;
	mu1 = wheelconfig(mu1, im1, input);
	im2 = mu1.length();
	cout << "Dla mu 2:";
	cin >> input;
	mu2 = wheelconfig(mu2, im2, input);
	ip1 = psi1.length();
	cout << "Dla psi 1:";
	cin >> input;
	psi1 = wheelconfig(psi1, ip1, input);
	ip2 = psi2.length();
	cout << "Dla psi 2:";
	cin >> input;
	psi2 = wheelconfig(psi2, ip2, input);
	ip3 = psi3.length();
	cout << "Dla psi 3:";
	cin >> input;
	psi3 = wheelconfig(psi3, ip3, input);
	ip4 = psi4.length();
	cout << "Dla psi 4:";
	cin >> input;
	psi4 = wheelconfig(psi4, ip4, input);
	ip5 = psi5.length();
	cout << "Dla psi 5:";
	cin >> input;
	psi5 = wheelconfig(psi5, ip5, input);
}
//wy�wietlanie obecnej konfiguracji k�
void wheelinfo()
{
	cout << "Obecne sekwencje na ko�ach maszyny:\n";
	cout << "Chi 1 - " << chi1<<"\n";
	cout << "Chi 2 - " << chi2 << "\n";
	cout << "Chi 3 - " << chi3 << "\n";
	cout << "Chi 4 - " << chi4 << "\n";
	cout << "Chi 5 - " << chi5 << "\n";
	cout << "Mu 1 - " << mu1 << "\n";
	cout << "Mu 2 - " << mu2 << "\n";
	cout << "Psi 1 - " << psi1 << "\n";
	cout << "Psi 2 - " << psi2 << "\n";
	cout << "Psi 3 - " << psi3 << "\n";
	cout << "Psi 4 - " << psi4 << "\n";
	cout << "Psi 5 - " << psi5 << "\n";
}
//kodowanie/dekodowanie w konsoli
void writeandcode()
{
	cout << "\nPodaj tekst do zakodowania\n";
	//cin.ignore() jest potrzebny, inaczej getline(cin,input) nie dzia�a w p�tli g��wnej programu
	cin.ignore();
	//pozwala na wpisanie dowolnego ci�gu znak�w odseparowanych spacjami
	getline(cin, input);
	//zakoduj i wy�wietlij
	encode(input);
	cout << output;
	//wyczy�� stringi, inputu teoretycznie nie trzeba, ale profilaktycznie nie zaszkodzi
	input.clear();
	output.clear();
}
//kodowanie/dekodowanie pliku tekstowego
void filecode()
{
	cout << "\nPodaj nazw� pliku wej�ciowego lub �cie�k� do niego (najlepiej .txt)\n";
	cin.ignore();
	getline(cin, input);
	cout << "Wczytuj�...\n";
	infile.open(input, ios::in | ios::binary);
	if (infile) {
		infile.seekg(0, ios::end);
		buffer.resize(infile.tellg());
		infile.seekg(0, ios::beg);
		infile.read(&buffer[0], buffer.size());
		infile.close();
		cout << "Odczyt pomy�lny, koduj� wiadomo��...\n";
		encode(buffer);
		cout << "Wiadomo�� zakodowana, zapisuj� do pliku 'output.txt'\n";
		outfile.open("output.txt");
		outfile << output;
		cout << "Zapis zako�czony sukcesem\n";
		outfile.close();
		input.clear();
		output.clear();
	}
	else cout << "B��d odczytu pliku wej�ciowego, sprawd� nazw� i/lub �cie�k�\n";
}
//g��wna p�tla programu
int main()
{
	//�eby wy�wietla� polskie znaki
	setlocale(LC_ALL, "pl_PL");
	cout << "Symulator maszyny Lorenza";
	while (control != 8) {
		switch(control){
		case 1:
			texthelp(ITA2mod);
			control = 0;
			break;
		case 2:
			lorenzinfo();
			control = 0;
			break;
		case 3:
			test();
			control = 0;
			break;
		case 4:
			wheelinfo();
			control = 0;
			break;
		case 5:
			configwheels();
			control = 0;
			break;
		case 6:
			writeandcode();
			control = 0;
			break;
		case 7:
			filecode();
			control = 0;
			break;
		}
		attract();
		cin >> control;
	}
}