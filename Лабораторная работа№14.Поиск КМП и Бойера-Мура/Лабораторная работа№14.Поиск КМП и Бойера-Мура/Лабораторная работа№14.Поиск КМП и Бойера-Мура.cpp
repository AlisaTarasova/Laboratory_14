#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Date
{
	int Day, Month, Year;
};

struct Address
{
	int Home;
	string Street;
};

struct Human
{
	string FIO;
	Date DateOfBirth;
	Address Address1;

	void Generate()
	{
		string names[] = { "Андрей", "Михаил", "Сергей", "Артём", "Дмитрий" };
		string surnames[] = { "Петров" , "Соловьёв", "Иванов","Гончаров","Пушкин" };
		string patronymic[] = { "Андреевич", "Владимирович", "Максимович", "Кирилович", "Ильич" };

		string street[] = { "Ленина","Екатерининская","Революция","Карла Маркса","Куйбышева" };

		FIO = surnames[rand() % 5] + " " + names[rand() % 5] + " " + patronymic[rand() % 5];
		DateOfBirth.Day = 1 + rand() % 28;
		DateOfBirth.Month = 1 + rand() % 12;
		DateOfBirth.Year = 1800 + rand() % 220;
		Address1.Home = 1 + rand() % 50;
		Address1.Street = street[rand() % 5];
	}
	void Print()
	{
		cout << "ФИО: " << FIO << endl;
		cout << "Дата рождения: " << DateOfBirth.Day << "." << DateOfBirth.Month << "." << DateOfBirth.Year << endl;
		cout << "Адрес: " << Address1.Street << ", " << Address1.Home << endl << endl;
	}
};

void Creation(Human*& hum, int n)
{
	for (int i = 0; i < n; i++)
	{
		hum[i].Generate();
	}
}

void Printmas(Human*& hum, int n)
{
	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			cout << "Элемент " << i + 1 << ": " << endl;
			hum[i].Print();
		}
	}
	else
		cout << "Массив пуст.";
}

bool KMP(Human* hum,int n, Human key)
{
	string Str="";
	string Podstr=to_string(key.DateOfBirth.Day)+ to_string(key.DateOfBirth.Month)  + to_string(key.DateOfBirth.Year);
	for (int i = 0; i < n; i++)
	{
		string s;
		s = to_string(hum[i].DateOfBirth.Day)  + to_string(hum[i].DateOfBirth.Month)  + to_string(hum[i].DateOfBirth.Year);
		Str = Str + s;
	}

	int* p = new int[Podstr.size()];
	int j = 0;
	p[j] = 0;

	for (int i=1;i< Podstr.size();i++)
	{
		if (Podstr[i] == Podstr[j])
		{
			p[i] = j + 1;
			i++;
			j++;
		}
		else
			if (j == 0)
			{
				p[i] = 0;
				i++;
			}
			else
				j = p[j - 1];
	}

	bool res = false;

	for (int j = 0, i = 0; i < Str.size(); ++i) {
		while ((j > 0) && (Podstr[j] != Str[i]))
			j = p[j - 1];
		j++;
		if (j == Podstr.size())
			res = true;
	}

	return res;
}

struct Cell
{
	char Symbol;
	int Bias;

	Cell(char c, int b)
	{
		Symbol = c;
		Bias = b;
	}
};

bool BM(Human* hum, int n, Human key)
{
	string listOfNumbers;

	string substring = "";
	substring += to_string(key.DateOfBirth.Day) + to_string(key.DateOfBirth.Month) + to_string(key.DateOfBirth.Year);

	for (int i = 0; i < n; i++)
	{
		string newLine = "";
		newLine += to_string(hum[i].DateOfBirth.Day) + to_string(hum[i].DateOfBirth.Month) + to_string(hum[i].DateOfBirth.Year);;
		listOfNumbers += newLine;
	}

	//cout << substring << endl;
	//cout << listOfNumbers << endl;

	// Создание таблицы
	vector<Cell> table;
	for (int i = substring.size() - 2; i >= 0; i--)
	{
		// Поиск равных символов в таблице 
		bool isEqual = false;
		for (int j = 0; j < table.size() && !isEqual; j++)
		{
			isEqual = table[j].Symbol == substring[i];
		}
		if (!isEqual)
		{
			table.push_back(Cell(substring[i], substring.size() - i - 1));
		}
	}

	// Добавление последнего элемента
	reverse(table.begin(), table.end());
	bool isEqual = false;
	for (int j = 0; j < table.size() && !isEqual; j++)
	{
		isEqual = table[j].Symbol == substring[substring.size() - 1];
	}
	if (!isEqual)
	{
		table.push_back(Cell(substring[0], substring.size()));
	}

	// Поиск
	bool isElementFind = false;
	for (int i = substring.size() - 1; !isElementFind && i < listOfNumbers.size(); )
	{
		// сравнение символов
		int j = substring.size() - 1;
		while (j >= 0 && listOfNumbers[i - substring.size() + 1 + j] == substring[j]) j--;

		if (j < 0)
		{
			isElementFind = true;
		}
		else
		{
			// поиск символа в таблице
			int k = 0;
			while (k < table.size() && table[k].Symbol != listOfNumbers[i]) k++;
			if (k == table.size())
			{
				i += substring.size();
			}
			else
			{
				i += table[k].Bias;
			}
		}
	}
	return isElementFind;
}




int main()
{
	setlocale(0, "");
	system("color F0");

	int n = 0;
	while (n <= 0)
	{
		cout << "Введите количество элементов в массиве: ";
		cin >> n;
	}

	Human* hum = new Human[n];
	Creation(hum, n);
	Printmas(hum, n);

	Human key;
	cout << "Введите дату рождения, по которой необходимо искать элемент: ";
	cin >> key.DateOfBirth.Day;
	cin>> key.DateOfBirth.Month;
	cin>> key.DateOfBirth.Year;

	int v;
	cout << "МЕНЮ:" << endl;
	cout << "1 - ПОИСК МЕТОДОМ КНУТА-МОРРИСА-ПРАТТА:" << endl;
	cout << "2 -ПОИСК МЕТОДОМ БОЙЕРА-МУРА:" << endl;
	cin >> v;
	if (v == 1)
	{
		if (KMP(hum,n,key))
			cout << "Элемент найден!";
		else
			cout << "Элменет не найден!";
	}
	else
		if (v == 2)
		{
			if(BM(hum, n, key))
			cout << "Элемент найден!";
			else
			cout << "Элменет не найден!";
		}
		else
		{
			cout << "Введено некорректное значение." << endl;
		}
	return 0;
}

