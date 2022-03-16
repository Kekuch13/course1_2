#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct Student {
    string name; // ФИО
    int group = 0;   // номер группы
    int marks[5] = {}; // оценки
    int scholarship = 0; // стипендия

    Student* next = nullptr; // указатель на следующего студента в группе
};

struct Group {
    int number = 0; // номер группы

    Student* student = nullptr; // указатель на список студентов
    Group* next = nullptr; // указатель на следующую группу
};

void insert(Student*& curr, Group*& top) { 
    // вставка студента curr
    // изначально top - укзатель на начало списка групп

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
        // создаем нового студента
        // записываем в его поля прочитанные данные
        Student* tmp = new Student;

        getline(file, tmp->name);
        if (tmp->name.empty()) {
            file.ignore(32767, '\n');
            getline(file, tmp->name);
        }
        file >> tmp->group;
        for (int & mark : tmp->marks) file >> mark;
        file >> tmp->scholarship;

        insert(tmp, top); // вставляем нового студента tmp в список
    }
    file.close();
}

void insert_student_console(Group*& top, int num) { // считывание данных о студенте с консоли
    // создаем нового студента
    // записываем в его поля введенные данные
    Student* tmp = new Student; 

    cout << "\nВведите ФИО студента: \n";
    cin.ignore(32767, '\n');
    getline(cin, tmp->name);
    if (!num) { // если номер группы заранее неизвестен, то вводим с консоли
        cout << "Введите номер группы: ";
        cin >> tmp->group;
    }
    else tmp->group = num; // иначе записываем уже известный
    cout << "Введите 5 оценок: ";
    for (int & mark : tmp->marks) cin >> mark;
    cout << "Введите размер стипендии: ";
    cin >> tmp->scholarship;

    insert(tmp, top); // вставляем нового студента tmp в список
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
            // запоминаем указатель на первого студента в группе
            // т.к. в процессе перебора студентов он будет изменяться
            Student* ptr = curr->student; 

            while (curr->student != nullptr) { 
                // выводим данные студента
                cout << "ФИО студента: " << curr->student->name << endl;
                cout << "Номер группы: " << curr->student->group << endl;
                cout << "Оценки: ";
                for (int mark : curr->student->marks) cout << mark << " ";
                cout << endl << "Размер стипендии: " << curr->student->scholarship << "\n\n";
                curr->student = curr->student->next;
            }
            curr->student = ptr; // восстанавливаем указатель на первого студента в группе
            curr = curr->next;
        }
    }

    if (n == 2) { // вывод данных о потоке в файл "stud_out.txt"
        ofstream file;
        file.open("stud_out.txt");
        
        while (curr != nullptr) {
            // запоминаем указатель на первого студента в группе
            // т.к. в процессе перебора студентов он будет изменяться
            Student* ptr = curr->student;

            while (curr->student != nullptr) {
                // записываем в файл данные студента
                file << "ФИО студента: " << curr->student->name << endl;
                file << "Номер группы: " << curr->student->group << endl;
                file << "Оценки: ";
                for (int mark : curr->student->marks) file << mark << " ";
                file << endl << "Размер стипендии: " << curr->student->scholarship << "\n\n";
                curr->student = curr->student->next;
            }
            curr->student = ptr; // восстанавливаем указатель на первого студента в группе
            curr = curr->next;
        }
        file.close();
    }
}

Group* search_group(Group*& curr, int num) { // поиск группы с номером num
    if (curr == nullptr) return nullptr; // если такой группы нет возвращаем nullptr

    if (curr->number == num) return curr; // если нашли такую группу возвращаем указатель на неё
    else search_group(curr->next, num);
    return nullptr;
}

void del_student(Student*& curr, const string& name_) {
    // удаление одного студента с ФИО name_ 
    // изначально curr указывает на первого студента в конкретной группе, номер этой группы ранее ввел пользователь

    if (curr == nullptr) { // если такой студент не нашелся
        cout << "Такого студента не существует\n";
        return;
    }

    if (curr->name == name_) { // если такой студент нашелся
        Student* ptr = curr->next; 
        delete curr;
        curr = ptr;
    }
    else del_student(curr->next, name_); // если ФИО не совпали - идем дальше 
}

void del_group(Group*& curr, int num) { // удаление целой группы с номером num
    if (curr == nullptr) { // если такой группы нет
        cout << "\nТакой группы не существует\n";
        return;
    }

    if (curr->number == num) { // если группа с таким номером существует
        Group* ptr = curr->next;
        Student* tmp;
        while (curr->student != nullptr) { // по очереди удаляем всех студентов данной группы
            tmp = curr->student->next;
            delete curr->student;
            curr->student = tmp;
        }
        delete curr; // удаляем саму группу
        curr = ptr; // меняем значение указателя на следующую группу
    }
    else del_group(curr->next, num);
}

void edit_student(Student*& curr, Group*& top, const string& name_) {
    // радактирование данных студента с именем name_
    // изначально curr указывает на студента в конкретной группе, номер группы ранеее ввел пользователь

    if (curr == nullptr) { // если дошли до конца списка и не нашли студента
        cout << "Такого студента не существует\n";
        return;
    }

    if (curr->name == name_) { // если студент нашелся  
        // выводим перед меню изначальные данные студента
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

                // копируем все данные студента в нового студента ptr
                ptr->name = curr->name;
                ptr->group = num;
                for (int i = 0; i < 5; ++i) ptr->marks[i] = curr->marks[i];
                ptr->scholarship = curr->scholarship;

                if (search_group(top, curr->group)->student->next == nullptr) del_group(top, curr->group); // если студент в группе один, то удалаяем всю группу
                else del_student(curr, curr->name); // иначе удаляем только студента curr

                insert(ptr, top); // вставляем "нового" студента ptr в базу
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
    // Выводим на экран номера всех групп, не имеющих двоечников,
    // выводим такие группы в порядке убывания среднего (по группе) балла

    bool log;
    int i, j;
    float sred, sum, kol;
    vector<float> sr_ball; // вектор для среднего балла в подходящих группах  
    vector<int> groups; // вектор для номеров подходящих групп

    while (tmp != nullptr) { // обходим все группы
        // запоминаем указатель на первого студента в группе
        // т.к. в процессе перебора студентов он будет изменяться
        Student* ptr = tmp->student;

        kol = 0;
        log = true;
        sum = 0;
        while (tmp->student != nullptr) { // смотрим всех студентов в группе            
            for (i = 0; i < 5; ++i) {
                if (tmp->student->marks[i] <= 2) break; // если встретилась двойка, то выходим из цикла             
                sum += tmp->student->marks[i]; // считаем сумму оценок студента
            }
            kol++; // считаем количество студентов
            if (i < 5) { // если встретилась двойка (цикл for не доработал до конца)
                log = false;
                break;                
            }
            tmp->student = tmp->student->next; // переходим к следующему студенту
        }
        if (log) { // если в группе нет двоечников
            sred = sum / (5 * kol); // считаем средний балл
            sr_ball.push_back(sred); //добавляем в вектор средних баллов посчитаный ранее балл
            groups.push_back(tmp->number); // добавляем группу в вектор групп
            // получилось, что номер группы и её средний балл имеют одинаковый индекс в векторах
        }
        tmp->student = ptr; // восстанавливаем указатель на первого студента в группе
        tmp = tmp->next; // переходим к следующей группе
    }

    if (sr_ball.empty()) {
        cout << "\nТаких групп нет\n";
        return;
    }
    
    // создаем массив индексов
    vector<int> index(groups.size()); 
    for (i = 0; i < groups.size(); ++i) index[i] = i;

    // сортировка пузырьком вектора средних баллов 
    // параллельно значения в индексном массиве тоже переставляются
    // по итогу значения в индексном массиве соответствуют индексам групп по убыванию среднего балла
    for (i = 0; i + 1 < sr_ball.size(); ++i) {
        for (j = 0; j + 1 < sr_ball.size() - i; ++j) {
            if (sr_ball[j] < sr_ball[j + 1]) {
                swap(sr_ball[j], sr_ball[j + 1]);
                swap(index[j], index[j + 1]);
            }
        }
    }

    // выводим подходящие группы и значение среднего балла
    cout << "\nГруппа -- средний балл:\n";
    for (i = 0; i < index.size(); ++i) {
        cout << " " << groups[index[i]] << " -- " << sr_ball[i] << endl;
    }
}

void dolg(Group* curr) {
    // Выводим перечень групп, упорядоченный по значению показателя «процент должников»(с указанием значения показателя)

    int i, j;
    float dolg, kol;
    vector<float> pers_dolg; // вектор для значений "процент должников"
    vector<int> groups; // вектор для номеров групп 

    while (curr != nullptr) {
        // запоминаем указатель на первого студента в группе
        // т.к. в процессе перебора студентов он будет изменяться
        Student* ptr = curr->student;

        kol = 0;
        dolg = 0;
        while (curr->student != nullptr) {
            for (i = 0; i < 5; ++i) {
                if (curr->student->marks[i] <= 2) break;
            }
            kol++; // считаем кол-во студентов в группе
            if (i < 5) dolg++; // если у студента есть двойка или меньше (т.е. цикл for не доработал до конца), то записывем студента в должники
            curr->student = curr->student->next; // переходим к следующему студенту
        }   
        groups.push_back(curr->number); // добавляем группу в вектор групп
        pers_dolg.push_back(100 * dolg / kol); // добавляем в вектор "процент должников" для данной группы
        // получилось, что номер группы и её "процент должников" имеют одинаковый индекс в векторах

        curr->student = ptr; // восстанавливаем указатель на первого студента в группе
        curr = curr->next; // переходим к следующей группе
    }

    // создаем массив индексов
    vector<int> index(groups.size());
    for (i = 0; i < groups.size(); ++i) index[i] = i;

    // сортировка пузырьком вектора значений "процент должников" 
    // параллельно значения в индексном массиве тоже переставляются
    // по итогу значения в индексном массиве соответствуют порядку групп по убыванию процента должников
    for (i = 1; i < pers_dolg.size(); ++i) {
        for (j = 0; j < pers_dolg.size() - i; ++j) {
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
        case 1: 
            // ввод данных о студентах
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
        case 2: 
            // ввод данных о студентах одной группы
            insert_group(top);
            break;
        case 3:
            // вывод данных о потоке
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            print_all(top);
            break;
        case 4:
            // редактирование данных студента
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
        case 5:
            // удаление конкретного студента
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
        case 6:
            // удаление конкретной группы
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            cout << "Введите номер группы: ";
            cin >> num;
            del_group(top, num);
            break;
        case 7:
            // группы без двоечников
            if (top == nullptr) {
                cout << "Данные о студентах отсутствуют" << endl;
                break;
            }
            without_twos(top);
            break;
        case 8:
            // процент должников во всех группах
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
