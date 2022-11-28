#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <locale>

using namespace std;
string 
//ko³a maszyny, chi, psi i mu
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
input, output; //stringi wpsiywane przez u¿ytkownika i wypisywane przez program
int ic1, ic2, ic3, ic4, ic5, im1, im2, ip1, ip2, ip3, ip4, ip5, //wartoœci na obecnej pozycji ko³a
i = 0, j = 0, k = 0, //indeksy kó³
codekey, //obecna wartoœæ kodu chi XOR psi
encoded, //obecna wartoœæ znaku zakodowanego 
control=0; //wartoœæ kontrolna do sterowania menu
map<char, int>ITA2mod{ //mapa reprezentuj¹ca ZMODYFIKOWANY kod ITA2 (orygina³ zawiera znaki kontrolne których nie wykorzystujê, ze wzglêdu na mo¿liwoœæ powstawania b³êdów)
	{'@',0x00},	{'e',0x01},	{'#',0x02},	{'a',0x03},	{' ',0x04},	{'s',0x05},	{'i',0x06},	{'u',0x07},
	{'%',0x08},	{'d',0x09},	{'r',0x0a},	{'j',0x0b},	{'n',0x0c},	{'f',0x0d},	{'c',0x0e},	{'k' ,0x0f},
	{'t',0x10},	{'z',0x11},	{'l',0x12},	{'w',0x13},	{'h',0x14},	{'y',0x15},	{'p',0x16},	{'q',0x17},
	{'o',0x18},	{'b',0x19},	{'g',0x1a},	{'^',0x1b},	{'m',0x1c},	{'x',0x1d},	{'v',0x1e},	{'*',0x1f},
	{'_',0x20},	{'3',0x21},	{'{',0x22},	{'-',0x23},	{'}',0x24},	{0x27,0x25},{'8',0x26},	{'7',0x27},
	{'<',0x28},	{'>',0x29},	{'4',0x2a},	{0x5c,0x2b},{',',0x2c},	{'!',0x2d},	{':',0x2e},	{'(',0x2f},
	{'5',0x30},	{'+',0x31},	{')',0x32},	{'2',0x33},	{'$',0x34},	{'6',0x35},	{'0',0x36},	{'1',0x37},
	{'9',0x38},	{'?',0x39},	{'&',0x3a},	{'|',0x3b},	{'.',0x3c},	{'/',0x3d},	{'=',0x3e},	{'`',0x3f}
};
char find_mapkey(map<char,int>input, int value)//znajdŸ znak na podstawie jego wartoœci
{
	for (auto it = input.begin(); it != input.end(); it++)
		if (it->second == value)
			return it->first;
}
int char2num(char in)//szybka konwersja znaku liczby na liczbê, potrzebna gdy¿ sekwencje kó³ s¹ przechowywane jako string
{
	return in - 48;
}
int calc_codekey(int c1,int c2,int c3,int c4,int c5,int p1,int p2,int p3,int p4,int p5)//generacja klucza koduj¹cego jako chi XOR psi
{
	int chifull = (c1 << 4) + (c2 << 3) + (c3 << 2) + (c4 << 1) + c5;
	int psifull = (p1 << 4) + (p2 << 3) + (p3 << 2) + (p4 << 1) + p5;
	return chifull ^ psifull;
}
void texthelp(map<char, int>input)//szybka pomoc jakie znaki s¹ obs³ugiwane
{
	int a = 1;
	cout << "Dopuszcalne znaki (plus spacja):\n";
	for (auto it = input.begin(); it != input.end(); it++)
	{
		cout << it->first<<" ";
		a++;
		if (a % 14 == 0)
			cout << "\n";
	}
	cout << "\n";
}
void encode(string toencode)//kodowanie wiadomoœci
{
	i = 0, j = 0, k = 0;
	for (i = 0; i < toencode.length(); i++)
	{
		ic1 = char2num(chi1[i % 41]);
		ic2 = char2num(chi2[i % 31]);
		ic3 = char2num(chi3[i % 29]);
		ic4 = char2num(chi4[i % 26]);
		ic5 = char2num(chi5[i % 23]);
		im1 = char2num(mu1[i % 61]);

		im2 = char2num(mu2[k % 37]);
		if (im1 == 1)k += 1;
		if (im2 == 1)j += 1;
		ip1 = char2num(psi1[j % 43]);
		ip2 = char2num(psi2[j % 47]);
		ip3 = char2num(psi3[j % 51]);
		ip4 = char2num(psi4[j % 53]);
		ip5 = char2num(psi5[j % 59]);

		codekey = calc_codekey(ic1, ic2, ic3, ic4, ic5, ip1, ip2, ip3, ip4, ip5);
		encoded = ITA2mod[toencode[i]] ^ codekey;
		output += find_mapkey(ITA2mod, encoded);
	}
}
void attract()//opcje menu
{
	cout << "\nCo chcesz zrobiæ?\n";
	cout << "1 - Podaj obslugiwane znaki\n";
	cout << "2 - Jak dzia³a maszyna Lorenza?\n";
	cout << "3 - Konfiguracja maszyny\n";
	cout << "4 - Test dzia³ania\n";
	cout << "5 - Kodowanie/Dekodowanie w konsoli\n";
	cout << "6 - Kodowanie/dekodowanie w pliku\n";
	cout << "7 - Zakoñcz pracê\n";
}
void test()//szybki test dzia³ania maszyny
{
	input = "to jest test dzialania maszyny lorenza";
	encode(input);
	cout << "Tekst niezakodowany:\n" << input << "\nTekst zakodowany:\n" << output;
	input = output;
	output.clear();
	encode(input);
	cout << "\nTekst zdekodowany:\n" << output;
	input.clear();
	output.clear();
}
void lorenzinfo()//relatywnie szybki opis dzia³ania maszyny
{
	cout << "	By³a to wykorzystywana podczas IIWŒ przez niemiecki sztab generalny (Oberkommando das Wehrmacht, OKW) maszyna generuj¹ca kod strumieniowy oparty na szyfrze Vernama, opartym na bitowych operacjach XOR.";
	cout << "W porównaniu do bardziej s³awnej Enigmy, maszyna Lorenza s³u¿y³a do kodowania wiadomoœci telegraficznych przesy³anych poprzez dalekopisy, do których by³a bezpoœrednio podpinana.";
	cout << "Maszyna lorenza jest te¿ potocznie nazywana 'Tunny' (Tuñczyk), gdy¿ taki kryptonim zosta³ jej nadany przez Brytyjskich cryptoanalityków, poniewa¿ w ich ¿argonie, niemieckie transmisje telegraficzne nazywano potocznie 'Fish', czyli ryby.\n";
	cout << "	Dzia³anie maszyny opiera siê kodowaniu znaku na podstawie kombinacji zêbatek - 2 grupy po 5 zêbatek koduj¹cych (chi i psi) i 2 zêbatki kontrolne (mu).";
	cout << "Ka¿da z zêbatek ma inn¹ iloœæ zêbów - chi1 -31,chi2-31,chi3-29,chi4-26,chi5-23,mu1-61,mu2-37,psi1-43,psi2-47,psi3-51,psi4-53,psi5-59. Ich d³ugoœci to wartoœci wzglêdnie pierwsze, co w teorii zapewnia bardzo du¿y okres czasu zanim sekwencja siê powtórzy.";
	cout << "Obecnie wpisywany znak jest kodowany poprzez operacjê XOR jego kodu znaku (opartego na standaryzowanym alfabecie telegraficznym ITA2, swoistym prekursorze ASCII) z kluczem bitowym, który powsta³ z XOR-a obecnej kombinacji wartoœci na ko³ach chi i psi.";
	cout << "Dekodowanie dzia³a tak samo, je¿eli klucz jest taki sam jak podczas zakodowania to XOR zakodowanego znaku z kluczem daje nam oryginalny znak";
	cout << "Ko³a chi i mu1 przechodz¹ o jedn¹ pozycjê do przodu po zakodowaniu ka¿dego znaku, ko³o mu2 tylko wtedy je¿eli mu1 jest na wartoœci 1, a ko³a psi tylko gdy mu2 jest na wartoœci 1.\n";
	cout << "	Aby zapewniæ nieprzywidywalnoœæ wartoœci koduj¹cych, przed transmisj¹ nale¿a³o skonfigurowaæ ko³a. Kombinacje kó³ by³y przesy³ane miêdzy stacjami jako sekwencja 12 liter, gdzie ka¿da litera odpowiada³a pewnej sekwencji na kole w tzw. ksi¹¿ce QEP, u¿ywanej do konfiguracji\n";
	cout << "Oczywiœcie, podobnie jak w przypadku Enigmy, niedopatrzenia pod wzglêdem przesy³u klucza konfiguracyjnego (gdy¿ tylko on by³ niekodowany) przyczyni³y siê do zdekodowania dzia³ania Maszyny Lorenza przez wywiad Brytyjski miêdzy Sierpniem '41 i Styczniem '42\n";
}
void configwheels()//kofiguracja maszyny na podstawie pliku tekstowego 
{}
void writeandcode()//kodowanie/dekodowanie w konsoli
{
	cout << "\nPodaj tekst do zakodowania\n";
	cin.ignore();
	getline(cin, input);
	encode(input);
	cout << output;
	input.clear();
	output.clear();
}
void filecode()//kodowanie/dekodowanie pliku tekstowego
{}
int main()//g³ówna pêtla programu
{
	setlocale(LC_ALL, "pl_PL");
	cout << "Symulator maszyny Lorenza";
	while (control != 7) {
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
			cout << "DO ZROBIENIA";
			control = 0;
			break;
		case 4:
			test();
			control = 0;
			break;
		case 5:
			writeandcode();
			control = 0;
			break;
		case 6:
			cout << "DO ZROBIENIA";
			break;
		}
		attract();
		cin >> control;
	}
}