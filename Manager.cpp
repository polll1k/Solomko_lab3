#include "Manager.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;
int readChoice(int minValue, int maxValue) {
    int value;
    while (true) {
        cin >> value;
        if (!cin.fail() && value >= minValue && value <= maxValue) {
            Logger::write(value);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите значение еще раз: ";
    }
}
void Manager::addPipe() {
    Pipe pipe;
    cin >> pipe;
    pipes.emplace(pipe.getId(), pipe);
    cout << "Труба добавлена. ID: " << pipe.getId() << endl;
}

void Manager::addStation() {
    CompressorStation cs;
    cin >> cs;
    stations.emplace(cs.getId(), cs);
    cout << "КС добавлена. ID: " << cs.getId() << endl;
}

void Manager::showObjects() {
    if (pipes.empty() && stations.empty()) {
        cout << "Объектов нет" << endl;
        return;
    }
    cout << "Трубы:" << endl;
    for (const auto& pair : pipes) {
        pair.second.print();
        cout << endl;
    }
    cout << "Компрессорные станции:" << endl;
    for (const auto& pair : stations) {
        pair.second.print();
        cout << endl;
    }
}

void Manager::searchPipes() {
    if (pipes.empty()) {
        cout << "Труб нет" << endl;
        return;
    }

    cout << "1. По названию" << endl;
    cout << "2. По ремонту" << endl;
    cout << "0. Назад" << endl;

    int searchChoice = readChoice(0, 2);

    if (searchChoice == 0) {
        return;
    }

    vector<int> foundIds;

    if (searchChoice == 1) {
        string text;
        cout << "Введите название: ";
        getline(cin, text);
        Logger::write(text);
        foundIds = findMatches(pipes, [&](const Pipe& obj) {
            return obj.matchesName(text);
        });
    }
    else if (searchChoice == 2) {
        cout << "В ремонте? 1/0: ";
        int repair = readChoice(0, 1);

        foundIds = findMatches(pipes, [&](const Pipe& obj) {
            return obj.matchesRepair(repair);
        });
    }
    if (foundIds.empty()) {
        cout << "Ничего не найдено" << endl;
        return;
    }

    cout << "Найдено труб: " << foundIds.size() << endl;
    cout << "1. Изменить все найденные" << endl;
    cout << "2. Выбрать по ID" << endl;
    cout << "0. Назад" << endl;

    int editChoice = readChoice(0, 2);

    if (editChoice == 0) {
        return;
    }

    vector<int> selectedIds;

    if (editChoice == 1) {
        selectedIds = foundIds;
    }
    else if (editChoice == 2) {
        cout << "Введите ID труб, для окончания 0" << endl;
        while (true) {
            cout << "ID: ";
            int id = readChoice(0, numeric_limits<int>::max());

            if (id == 0) {
                break;
            }

            bool found = find(foundIds.begin(), foundIds.end(), id)
                         != foundIds.end();

            bool selected = find(selectedIds.begin(), selectedIds.end(), id)
                            != selectedIds.end();

            if (found && !selected) {
                selectedIds.push_back(id);
            }
            else if (!found) {
                cout << "Такой трубы среди найденных нет" << endl;
            }
            else {
                cout << "Эта труба уже выбрана" << endl;
            }
        }
    }

    if (selectedIds.empty()) {
        cout << "Трубы не выбраны" << endl;
        return;
    }

    cout << "1. В ремонт" << endl;
    cout << "2. Из ремонта" << endl;
    cout << "3. Удалить" << endl;
    cout << "0. Отмена" << endl;

    int action = readChoice(0, 3);

    if (action == 0) {
        return;
    }

    for (int id : selectedIds) {
        if (action == 1) {
            pipes.at(id).setRepair(true);
        }
        else if (action == 2) {
            pipes.at(id).setRepair(false);
        }
        else if (action == 3) {
            pipes.erase(id);
        }
    }

    cout << "Действие выполнено" << endl;
}

void Manager::searchStations() {
    if (stations.empty()) {
        cout << "КС нет" << endl;
        return;
    }

    cout << "1. По названию" << endl;
    cout << "2. По проценту незадействованных цехов" << endl;
    cout << "0. Назад" << endl;

    int searchChoice = readChoice(0, 2);

    if (searchChoice == 0) {
        return;
    }

    vector<int> foundIds;

    if (searchChoice == 1) {
        string text;
        cout << "Введите название: ";
        getline(cin, text);
        Logger::write(text);
        foundIds = findMatches(stations, [&](const CompressorStation& obj) {
            return obj.matchesName(text);
        });
    }
    else if (searchChoice == 2) {
        double percent;
        cout << "Введите минимальный процент: ";
        cin >> percent;

        while (cin.fail() || percent < 0 || percent > 100) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Введите процент от 0 до 100: ";
            cin >> percent;
        }

        Logger::write(percent);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        foundIds = findMatches(stations, [&](const CompressorStation& obj) {
            return obj.matchesIdlePercent(percent);
        });
    }

    if (foundIds.empty()) {
        cout << "Ничего не найдено" << endl;
        return;
    }

    cout << "Введите ID КС: ";
    int id = readChoice(1, numeric_limits<int>::max());

    bool found = find(foundIds.begin(), foundIds.end(), id)
                 != foundIds.end();

    if (!found) {
        cout << "Такой КС среди найденных нет" << endl;
        return;
    }

    cout << "1. Запустить цех" << endl;
    cout << "2. Остановить цех" << endl;
    cout << "3. Удалить КС" << endl;
    cout << "0. Отмена" << endl;

    int action = readChoice(0, 3);

    if (action == 1) {
        stations.at(id).startWorkshop();
    }
    else if (action == 2) {
        stations.at(id).stopWorkshop();
    }
    else if (action == 3) {
        stations.erase(id);
        cout << "КС удалена" << endl;
    }
}
void Manager::saveData() {
    string filename;

    cout << "Введите имя файла: ";
    getline(cin, filename);
    Logger::write(filename);

    ofstream file(filename);

    if (!file) {
        cout << "Ошибка файла" << endl;
        return;
    }

    if (pipes.empty()) {
        file << 0 << endl;
    }
    else {
        file << 1 << endl;
        file << pipes.size() << endl;

        for (const auto& pair : pipes) {
            file << pair.second;
        }
    }

    if (stations.empty()) {
        file << 0 << endl;
    }
    else {
        file << 1 << endl;
        file << stations.size() << endl;

        for (const auto& pair : stations) {
            file << pair.second;
        }
    }
    if (!file) {
        cout << "Ошибка записи файла" << endl;
        return;
    }

    cout << "Данные сохранены" << endl;
}

void Manager::loadData() {
    string filename;

    cout << "Введите имя файла: ";
    getline(cin, filename);
    Logger::write(filename);

    ifstream file(filename);

    if (!file) {
        cout << "Файл не найден" << endl;
        return;
    }

    int marker;
    int count;

    unordered_map<int, Pipe> newPipes;
    unordered_map<int, CompressorStation> newStations;

    if (!(file >> marker)) {
        cout << "Ошибка файла" << endl;
        return;
    }

    if (marker == 1) {
        if (!(file >> count) || count < 0) {
            cout << "Ошибка количества труб" << endl;
            return;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < count; i++) {
            Pipe pipe;
            file >> pipe;

            if (file.fail()) {
                cout << "Ошибка чтения труб" << endl;
                return;
            }

            newPipes.emplace(pipe.getId(), pipe);
        }
    }
    else if (marker == 0) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "Ошибка файла" << endl;
        return;
    }

    if (!(file >> marker)) {
        cout << "Ошибка файла" << endl;
        return;
    }

    if (marker == 1) {
        if (!(file >> count) || count < 0) {
            cout << "Ошибка количества КС" << endl;
            return;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < count; i++) {
            CompressorStation cs;
            file >> cs;

            if (file.fail()) {
                cout << "Ошибка чтения КС" << endl;
                return;
            }

            newStations.emplace(cs.getId(), cs);
        }
    }
    else if (marker == 0) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "Ошибка файла" << endl;
        return;
    }

    pipes = newPipes;
    stations = newStations;

    cout << "Данные загружены" << endl;
}
void Manager::run() {
    while (true) {
        cout << endl;
        cout << "1. Добавить трубу" << endl;
        cout << "2. Добавить КС" << endl;
        cout << "3. Просмотреть все объекты" << endl;
        cout << "4. Поиск труб" << endl;
        cout << "5. Поиск КС" << endl;
        cout << "6. Сохранить" << endl;
        cout << "7. Загрузить" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        int choice = readChoice(0, 7);
        if (choice == 0) {
            cout << "Выход" << endl;
            break;
        }

        if (choice == 1) {
            addPipe();
        }
        else if (choice == 2) {
            addStation();
        }
        else if (choice == 3) {
            showObjects();
        }
        else if (choice == 4) {
            searchPipes();
        }
        else if (choice == 5) {
            searchStations();
        }
        else if (choice == 6) {
            saveData();
        }
        else if (choice == 7) {
            loadData();
        }
    }
}