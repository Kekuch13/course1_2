#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

struct Student {
    string name;
    int group = 0;
    int marks[5] = {};
    int scholarship = 0;

    Student* next = nullptr;
};

struct Group {
    int number = 0;

    Student* student = nullptr;
    Group* next = nullptr;
};

void insert(Student*& curr, Group*& top) { // вставка студента
    if (top == nullptr) { // если такой группы как у студента ещё нет, то создаем новую группу и записываем туда студента
        top = new Group;
        top->number = curr->group;
        top->student = curr;
        return;
    }

    if (curr->group == top->number) { // если номер группы совпал, то добавляем в начало группы этого студента        
        curr->next = top->student;        
        top->student = curr;
        return;
    }
    else insert(curr, top->next);
}

void insert_student_file(Group*& top) { // считываение данных о потоке из файла "stud_in.txt"
    ifstream file;
    file.open("stud_in.txt");

    while (!file.eof()) {
        Student* tmp = new Student;

        getline(file, tmp->name);
        if (tmp->name.empty()) {
            file.ignore(32767, '\n');
            getline(file, tmp->name);
        }
        file >> tmp->group;
        for (int & mark : tmp->marks) file >> mark;
        file >> tmp->scholarship;

        insert(tmp, top);
    }
    file.close();
}

void insert_student_console(Group*& top, int num) { // считывание данных о студенте с консоли
    Student* tmp = new Student; 

    cout << "\nВведите ФИО студента: \n";
    cin.ignore(32767, '\n');
    getline(cin, tmp->name);
    if (!num) {
        cout << "Введите номер группы: ";
        cin >> tmp->group;
    }
    else tmp->group = num;
    cout << "Введите 5 оценок: ";
    for (int & mark : tmp->marks) cin >> mark;
    cout << "Введите размер стипендии: ";
    cin >> tmp->scholarship;

    insert(tmp, top);
}

void insert_group(Group*& top) { // вставка нескольких студентов из одной группы
    int a, num;

    cout << "Введите номер группы: ";
    cin >> num;
    while (true) {
        insert_student_console(top, num);
        cout << "Хотите продолжить?\n";
        cin >> a;
        if (a == 0) break;
    }
}

void print_all(Group* curr) { // вывод данных о потоке
    int n;

    cout << "----------------------------------------------" << endl;
    cout << "| 1. Вывод данных о потоке на экран" << endl;
    cout << "| 2. Вывод данных о потоке в файл" << endl;
    cout << "----------------------------------------------" << endl;
    cin >> n;

    if (n == 1) { // вывод данных о потоке в консоль
        while (curr != nullptr) {
            Student* ptr = curr->student; 

            while (curr->student != nullptr) {
                cout << "ФИО студента: " << curr->student->name << endl;
                cout << "Номер группы: " << curr->student->group << endl;
                cout << "Оценки: ";
                for (int mark : curr->student->marks) cout << mark << " ";
                cout << endl << "Размер стипендии: " << curr->student->scholarship << "\n\n";
                curr->student = curr->student->next;
            }
            curr->student = ptr;
            curr = curr->next;
        }
    }

    if (n == 2) { // вывод данных о потоке в файл "stud_out.txt"
        ofstream file;
        file.open("stud_out.txt");
        
        while (curr != nullptr) {
            Student* ptr = curr->student;

            while (curr->student != nullptr) {
                file << "ФИО студента: " << curr->student->name << endl;
                file << "Номер группы: " << curr->student->group << endl;
                file << "Оценки: ";
                for (int mark : curr->student->marks) file << mark << " ";
                file << endl << "Размер стипендии: " << curr->student->scholarship << "\n\n";
                curr->student = curr->student->next;
            }
            curr->student = ptr;
            curr = curr->next;
        }
        file.close();
    }
}

Group* search_group(Group*& curr, int num) { // поиск группы с номером num
    if (curr == nullptr) return nullptr;

    if (curr->number == num) return curr;
    else search_group(curr->next, num);
    return nullptr;
}

void del_student(Student*& curr, const string& name_) {// удаление одного студента с ФИО name_
    if (curr == nullptr) {
        cout << "Такого студента не существует\n";
        return;
    }

    if (curr->name == name_) {
        Student* ptr = curr->next; 
        delete curr;
        curr = ptr;
    }
    else del_student(curr->next, name_);
}

void del_group(Group*& curr, int num) { // удаление целой группы с номером num
    if (curr == nullptr) {
        cout << "\nТакой группы не существует\n";
        return;
    }

    if (curr->number == num) {
        Group* ptr = curr->next;
        Student* tmp;
        while (curr->student != nullptr) {
            tmp = curr->student->next;
            delete curr->student;
            curr->student = tmp;
        }
        delete curr;
        curr = ptr;
    }
    else del_group(curr->next, num);
}

void edit_student(Student*& curr, Group*& top, const string& name_) { // радактирование данных студента с именем name_
    if (curr == nullptr) { // если дошли до конца списка и не нашли студента
        cout << "Такого студента не существует\n";
        return;
    }

    if (curr->name == name_) {
        cout << endl;
        cout << "ФИО студента: " << curr->name << endl;
        cout << "Номер группы: " << curr->group << endl;
        cout << "Оценки: ";
        for (int mark : curr->marks) cout << mark << " ";
        cout << endl << "Размер стипендии: " << curr->scholarship << "\n\n";

        int log;
        Student* ptr = new Student;

        cout << endl;
        cout << "| 0. Вывести данные студента\n";
        cout << "| 1. Изменить ФИО\n";
        cout << "| 2. Изменить номер группы\n";
        cout << "| 3. Ввести новые оценки\n";
        cout << "| 4. Изменить размер стипендии\n";
        cout << "| 5. Выход\n\n";  
        cin >> log;
        if (log >= 5) return;
            
        switch (log)
        {
            case 0: // вывод текущих данных студента
                cout << endl;
                cout << "ФИО студента: " << curr->name << endl;
                cout << "Номер группы: " << curr->group << endl;
                cout << "Оценки: ";
                for (int mark : curr->marks) cout << mark << " ";
                cout << endl << "Размер стипендии: " << curr->scholarship << "\n\n";
                break;
            case 1: // измененние ФИО
                cout << "\nВведите новые ФИО: ";
                cin.ignore(32767, '\n');
                getline(cin, curr->name);
                break;
            case 2: // изменение номера группы
                int num;
                cout << "\nВведите новый номер группы: ";
                cin >> num;

                ptr->name = curr->name;
                ptr->group = num;
                for (int i = 0; i < 5; ++i) ptr->marks[i] = curr->marks[i];
                ptr->scholarship = curr->scholarship;

                if (search_group(top, curr->group)->student->next == nullptr) del_group(top, curr->group); // если студент в группе один, то удалаяем всю группу
                else del_student(curr, curr->name); // иначе удаляем только студента curr

                insert(ptr, top);
                break;
            case 3: // изменяем оценки студента
                cout << "\nВведите новые оценки ";
                for (int & mark : curr->marks) cin >> mark;
                break;
            case 4: // изменяем размер стипендии студента
                cout << "\nВведите новый размер стипендии: ";
                cin >> curr->scholarship;
                break;
            default:
                break;
        }        
    }
    else edit_student(curr->next, top, name_);
}

void without_twos(Group* tmp) {
    bool log;
    int i, j;
    float sred, sum, kol; // kol - кол-во студентов в группе
    vector<pair<float, int>> groups; // пары: средний балл -- группа

    while (tmp != nullptr) {
        Student* ptr = tmp->student;

        kol = 0;
        log = true;
        sum = 0;
        while (tmp->student != nullptr) {
            for (i = 0; i < 5; ++i) {
                if (tmp->student->marks[i] <= 2) break;
                sum += tmp->student->marks[i];
            }
            kol++;
            if (i < 5) {
                log = false;
                break;
            }
            tmp->student = tmp->student->next;
        }
        if (log) {
            sred = sum / (5 * kol);
            groups.push_back({sred,tmp->number});
        }
        tmp->student = ptr;
        tmp = tmp->next;
    }

    if (groups.empty()) {
        cout << "\nТаких групп нет\n";
        return;
    }

    sort(groups.begin(), groups.end(), greater<pair<float, int>>());
    for(auto x : groups) {
        cout << " " << x.second << " -- " << x.first << endl;
    }
}

void dolg(Group* curr) {
    int i, j;
    float dolg, kol; // kol - кол-во студентов в группе, dolg - кл-во должников
    vector<float> pers_dolg; // вектор для значений "процент должников"
    vector<int> groups; // вектор для номеров групп

    while (curr != nullptr) {
        Student* ptr = curr->student;

        kol = 0;
        dolg = 0;
        while (curr->student != nullptr) {
            for (i = 0; i < 5; ++i) {
                if (curr->student->marks[i] <= 2) break;
            }
            kol++;
            if (i < 5) dolg++;
            curr->student = curr->student->next;
        }
        groups.push_back(curr->number);
        pers_dolg.push_back(100 * dolg / kol);
        // получилось, что номер группы и её "процент должников" имеют одинаковый индекс в векторах

        curr->student = ptr;
        curr = curr->next;
    }

    // создаем массив индексов
    vector<int> index(groups.size());
    for (i = 0; i < groups.size(); ++i) index[i] = i;

    // сортировка пузырьком вектора значений "процент должников"
    // параллельно значения в индексном массиве тоже переставляются
    // по итогу значения в индексном массиве соответствуют порядку групп по убыванию процента должников
    for (i = 0; i + 1< pers_dolg.size(); ++i) {
        for (j = 0; j + 1 < pers_dolg.size() - i; ++j) {
            if (pers_dolg[j] < pers_dolg[j + 1]) {
                swap(pers_dolg[j], pers_dolg[j + 1]);
                swap(index[j], index[j + 1]);
            }
        }
    }

    // выводим подходящие группы и значение "процент должников"
    cout << "\nГруппа -- процент должников:\n";
    for (i = 0; i < index.size(); ++i) {
        cout << " " << groups[index[i]] << " -- " << pers_dolg[i] << "%" << endl;
    }
}


int main()
{
    int num, log = 1;
    string str;
    Group* ptr;
    Group* top = nullptr;
    setlocale(0, "RUS");

    while (true) {
        cout << "----------------------------------------------" << endl;
        cout << "| 1. Ввести данные о студентах\n"; 
        cout << "| 2. Ввести данные о группе\n"; 
        cout << "| 3. Вывести данные о всех студентах потока\n"; 
        cout << "| 4. Редактировать данные студента\n";  
        cout << "| 5. Удалить студента\n"; 
        cout << "| 6. Удалить группу\n"; 
        cout << "| 7. Группы без двоечников\n";
        cout << "| 8. Процент должников по группам\n";
        cout << "| 9. Выход\n";
        cout << "----------------------------------------------" << endl;
        cin >> log;
        if (log <= 0 || log >= 9) break;

        switch (log)
        {
        case 1: // ввод данных о студентах
            int a, n;

            cout << "----------------------------------------------" << endl;
            cout << "| 1. Ввод с клавиатуры" << endl;
            cout << "| 2. Ввод из файла" << endl;
            cout << "----------------------------------------------" << endl;
            cin >> n;

            if (n == 1) {
                while (true) {
                    insert_student_console(top, 0);
                    cout << "Хотите продолжить?\n";
                    cin >> a;
                    if (a == 0) break;
                }
            }
            if (n == 2) insert_student_file(top);

            break;
        case 2: // ввод данных о студентах одной группы
            insert_group(top);
            break;
        case 3: // вывод данных о потоке
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            print_all(top);
            break;
        case 4: // редактирование данных студента
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            cout << "Введите номер группы: ";
            cin >> num;
            cout << "Введите Фио студента: ";
            cin.ignore(32767, '\n');
            getline(cin, str);
            ptr = search_group(top, num); // группа студента
            if (ptr == nullptr) {
                cout << "\nТакой группы не существует\n";
                break;
            }
            else edit_student(ptr->student, top, str);
            break;
        case 5: // удаление конкретного студента
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }  
            cout << "Введите номер группы: ";
            cin >> num;
            cout << "Введите Фио студента: ";
            cin.ignore(32767, '\n');
            getline(cin, str);
            ptr = search_group(top, num); // группа студента 
            if (ptr == nullptr) {
                cout << "\nТакой группы не существует\n";
                break;
            }
            else
                if (ptr->student->next == nullptr) del_group(top, ptr->number); // если студент в греппе один, то удалаяем всю группу
                else del_student(ptr->student, str); // иначе удаляем только студента
            break;
        case 6: // удаление конкретной группы
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            cout << "Введите номер группы: ";
            cin >> num;
            del_group(top, num);
            break;
        case 7: // группы без двоечников
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            without_twos(top);
            break;
        case 8: // процент должников во всех группах
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            dolg(top);
            break;
        default:
                break;
        }
    }

    while (top != nullptr) { // удаление всех групп
        while (top->student != nullptr) { // удаление всех студентов в группе
            Student* tmp = top->student->next;
            delete top->student;
            top->student = tmp;
        }
        ptr = top->next;
        delete top;
        top = ptr;
    }

    return 0;
}
