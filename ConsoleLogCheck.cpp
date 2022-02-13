#include <iostream>
//#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <windows.h>

using namespace std;

//Проверка на соответствие символов слову с определенной позиции в строке
//Если слово совпадает, то передаётся 0.
bool check_word(int pos, string in, string word) {
    int clck = 0;
    while (word[clck]) {
        if (in[pos] != word[clck]) { return 0; }
        clck++; pos++;
    }
    return 1;
}

//Проверка на присутствие набора символов(слова) в строке
bool check_string (string in, string word, int &count) {
    int count_this = 0;
    int clck = 0; 
    while (in[clck]) {
        //проверка на нужное слово производится при совпадении текущего символа в строке с первым символом искомого слова
        //при совпадении возвращает 1
        if (in[clck] == word[0]) {
            if (check_word(clck, in, word)) { count_this++;}
        }
        clck++;
    }
    if (count_this > 0) { count += count_this; return 1; }
    return 0;
}

//Считывание строк из файла, проверка каждой, копирование строк в резервный файл.
//0 - file not open
//1 - not found
//2 - found
int check_and_copy(string path, string element, int &count) {
    //Загрузка файла в вектор, по указанному пути
    ifstream data(path);
    count = 0;
    //Проверка на открытие файла
    if (!data.is_open()) { return 0; }
    vector <string> text;
    
    while (!data.eof()) {
        string tmp;
        if (getline(data, tmp)) {
            text.push_back(tmp);
        }
    }
    data.close();

    //Построчный поиск
    for (int i = 0; i < text.size(); i++) {
        check_string(text.at(i), element, count);
    }
    //Сохранение копии файла
    ofstream dataout("last_checked_log.txt");
    if (dataout.is_open()) {
        for (int i = 0; i < text.size(); i++) {
            dataout << text.at(i) << endl;
        }
    }
    dataout.close();

    if (count > 0) { return 2; }
    return 1;
}

//Загрузка из конфига параметров пути и искомого слова
bool loadcfg(string &path, string &word) {
    ifstream data("config.cfg");
    vector <string> text;
    int count, path_pos = 0,word_pos = 0, clck = 0;

    //Проверка на открытие файла
    if (!data.is_open()) { return 1; }

    //Read config
    while (!data.eof()) {
        string tmp;
        if (getline(data, tmp)) {
            text.push_back(tmp);
        }
    }
    data.close();
    
    //Check config
    for (int i = 0; i < text.size(); i++) {
        if (check_string(text.at(i), "path=", count)) { path_pos = i + 1; };
        if (check_string(text.at(i), "word=", count)) { word_pos = i + 1; };
    }
    if (path_pos > 0 and word_pos > 0 and path_pos != word_pos) {
    
        clck = 5;
        while (text.at(path_pos-1)[clck] != 0) {
            path += text.at(path_pos - 1)[clck];
            clck++;
        }
        clck = 5;
        while (text.at(word_pos - 1)[clck] != 0) {
            word += text.at(word_pos - 1)[clck];
            clck++;
        }

        for (int i = 0; i < text.size(); i++) {
            cout << text.at(i) << endl;
        }
        return 0;
    }

    return 1;
}

int main()
{
    setlocale(LC_ALL, "rus");
    int count = 0;
    string path, word;
    if (loadcfg(path, word)) {
        cout << "Error reading the configuration file" << endl;
        system("PAUSE");
        return 1;
    }
    
    switch (check_and_copy(path, word, count)) {
    case 0:
        cout << "Error to open file" << endl;
        break;
    case 1:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | 10));
        cout << word << " not found" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | 15));
        break;
    case 2:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | 4));
        cout << word << " found " << count << " times" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((0 << 4) | 15));
        break;
    }

    system("PAUSE");
    return 0;
}

