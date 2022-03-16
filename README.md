# Курсовая работа по программированию за 1 курс "Студенческий поток"

## Задание:
Программа должна поддерживать систему меню,
пункты которых соответствуют выполнению функций,
предусмотренных общей частью задания.
Предлагаемые структуры данных должны учитывать
изначальную неопределенность возможного количества групп и студентов в группах.

### Необходимо
1) Разработать (и программно реализовать) динамические структуры данных
   (односвязные списки) и алгоритмы их обработки,
   позволяющие поддерживать выполнение следующих функций:
    - консольный ввод/вывод данных о всех студентах потока;
    - файловый ввод/вывод данных о потоке;
    - редактирование данных о студентах и группах потока,
      включающее операции добавления/удаления групп и студентов;
3) Разработать и реализовать алгоритмы обработки базы данных, предусмотренные персональным заданием.

### Персональные задания
1) Вывести на экран номера всех групп, не имеющих двоечников,
   в порядке убывания среднего (по группе) балла;
2) Вывести перечень групп, упорядоченный по значению показателя
   «процент должников» (с указанием значения показателя).
   
***

### Описание структур данных
Программа включает в себя два односвязных списка: `Student` и `Group`

`Student` содержит:
- `name` - ФИО (до 50 символов);
- `group` - номер группы;
- `marks[5]`набор из пяти оценок за последнюю сессию (без указания предметов);
- `scholarship` - размер стипендии.

`Group` содержит:
- `number` - номер группы;
- `student` - указатель на список студентов;
- `next` - указатель на следующую группу.

***

### Описание функций для персональных заданий
- Функция `without_twos` - вывод номеров всех групп, не имеющих двоечников,
  в порядке убывания среднего (по группе) балла:
```c++ 
void without_twos(Group* tmp) {
    bool log;
    int i, j;
    float sred, sum, kol; // kol - кол-во студентов в группе
    vector<float> sr_ball; // вектор для среднего балла в подходящих группах  
    vector<int> groups; // вектор для номеров подходящих групп

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
            sr_ball.push_back(sred);
            groups.push_back(tmp->number);
            // получилось, что номер группы и её средний балл имеют одинаковый индекс в векторах
        }
        tmp->student = ptr;
        tmp = tmp->next; 
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
```
- Функция `dolg` - вывод переченя групп, упорядоченных по значению показателя
  «процент должников» (с указанием значения показателя).
```c++
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
```
