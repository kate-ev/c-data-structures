#include <fstream>

struct Date {
    Date() {
        day = 1;
        month = 11;
        year = 1111;
    }

    int day;
    int month;
    int year;

    static int calculateAge(int current_year, int birth_year) {
        return current_year - birth_year;
    }

    static void printDate(Date *date, std::fstream &output) {
        char dot = '.';
        if (date->day < 10) {
            output << "0";
        }
        output << date->day;
        output << dot;
        if (date->month < 10) {
            output << "0";
        }
        output << date->month;
        output << dot;
        output << date->year;
    }
};

struct Worker {
    Worker() {
        for (char &j : name) {
            j = '\0';
        }
        for (char &k : surname) {
            k = '\0';
        }
    }

    char name[31]{};
    char surname[31]{};
    Date *birthday{};
    struct Worker *next{};
};

struct Worker_With_Age {
    Worker *worker;
    int age;
    struct Worker_With_Age *l;
};

class Priority_Worker_Results_List {
public:
    Worker_With_Age *f;
public:
    Priority_Worker_Results_List() {
        f = nullptr;
    }

    static bool compareNames(const char *name1, const char *name2) {
        int i = 0;
        while (name1[i] != '\0' || name2[i] != '\0') {
            if (toLower(name1[i]) != toLower(name2[i])) {
                return false;
            }
            i++;
        }
        return true;
    }

    void insert(Worker *w1, Date *anniversary) {
        Worker_With_Age *t, *q;
        t = new Worker_With_Age;
        t->worker = w1;
        t->age = Date::calculateAge(anniversary->year, w1->birthday->year);
        int w1_age = Date::calculateAge(anniversary->year, w1->birthday->year);
        if (f == nullptr || w1_age > f->age || (w1_age == f->age && compareTwoNames(w1->surname, f->worker->surname)) ||
            (w1_age == f->age && compareNames(w1->surname, f->worker->surname) && compareTwoNames(w1->name, f->worker->name))) {
            t->l = f;
            f = t;
        } else {
            q = f;
            while (q->l != nullptr) {
                if (q->l->age > w1_age) {
                    q = q->l;
                    continue;
                }
                if (q->l->age == w1_age && (compareTwoNames(q->l->worker->surname, w1->surname))) {
                    q = q->l;
                    continue;
                }
                if (q->l->age == w1_age && (compareNames(w1->surname, q->l->worker->surname)) && (compareTwoNames(q->l->worker->name, w1->name))) {
                    q = q->l;
                    continue;
                }
                break;
            }
            t->l = q->l;
            q->l = t;
        }
    }

    static bool compareTwoNames(const char *arr1, const char *arr2) {
        int arr1_length = getCount(arr1);
        int arr2_length = getCount(arr2);
        int i = 0;
        while ((i < arr1_length) && (i < arr2_length)) {
            if (toLower(arr1[i]) < toLower(arr2[i])) return true;
            else if (toLower(arr1[i]) > toLower(arr2[i])) return false;
            i++;
        }

        if (arr1_length < arr2_length) return true;
        else return false;
    }

    static int toLower(int ch) {
        if (ch >= 'A' && ch <= 'Z') {
            return ('a' + ch - 'A');
        } else {
            return ch;
        }
    }

    static int getCount(const char *elements) {
        int count = 0;
        while (elements[count] != '\0') count++;
        return count;
    }

    void printPriorityResult(std::fstream &output) const {
        Worker_With_Age *ptr;
        ptr = f;
        if (f == nullptr)
            output << " ";
        else {
            while (ptr != nullptr) {
                output << ptr->age << ' ' << ptr->worker->name << ' ' << ptr->worker->surname << std::endl;
                ptr = ptr->l;
            }
        }
    }
};

class Worker_List {
public:
    Worker *head, *tail;
public:
    Worker_List() {
        head = nullptr;
        tail = nullptr;
    }

    void addWorker(const char *name, int name_length, const char *surname, int surname_length, Date *birth) {
        auto *current = new Worker;
        for (int i = 0; i < name_length; i++) {
            current->name[i] = name[i];
        }
        for (int i = 0; i < surname_length; i++) {
            current->surname[i] = surname[i];
        }
        current->birthday = new Date;
        current->birthday->day = birth->day;
        current->birthday->month = birth->month;
        current->birthday->year = birth->year;
        if (!checkIfNotDuplicate(current)) {
            return;
        }
        current->next = nullptr;
        if (head == nullptr) {
            head = current;
            tail = current;
        } else {
            tail->next = current;
            tail = current;
        }
    }


    Date *findClosestAnniversary(Date *today) const {
        Worker *current = head;
        Date *closest = calculateNextBirthday(head->birthday, today);
        int difference = calculateDifferenceBetweenDates(closest, today);
        while (current) {
            int new_difference = calculateDifferenceBetweenDates(calculateNextBirthday(current->birthday, today),
                                                                 today);
            if (new_difference < difference) {
                closest = calculateNextBirthday(current->birthday, today);
                difference = calculateDifferenceBetweenDates(closest, today);
            }
            current = current->next;
        }
        return closest;
    }

    static Date *calculateNextBirthday(Date *birthday, Date *today) {
        Date *next_birthday = new Date;
        next_birthday->day = birthday->day;
        next_birthday->month = birthday->month;
        if (today->month > birthday->month) {
            next_birthday->year = today->year + 1;
            return next_birthday;
        }

        if (today->month < birthday->month) {
            next_birthday->year = today->year;
            return next_birthday;
        }

        if (today->month == birthday->month) {
            if (today->day > birthday->day) {
                next_birthday->year = today->year + 1;
                return next_birthday;
            }
            if (today->day < birthday->day) {
                next_birthday->year = today->year;
                return next_birthday;
            }
            if (today->day == birthday->day) {
                next_birthday->year = today->year;
                return next_birthday;
            }
        }
        return nullptr;
    }

    static unsigned int abs(int v) {
        return (v < 0) ? -v : v;
    }

    static int calculateDifferenceBetweenDates(Date *from, Date *to) {
        static int increment[12] = {1, -2, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1};

        int daysInc = 0;
        if (to->day - from->day < 0) {
            int month = to->month - 2; // -1 from zero, -1 previous month.
            if (month < 0)
                month = 11; // Previous month is December.
            daysInc = increment[month];
            //if ((month == 1) && (to->year % 4 == 0))
                //daysInc++; // Increment days for leap year.
        }

        int total1 = from->year * 360 + from->month * 30 + from->day;
        int total2 = to->year * 360 + to->month * 30 + to->day;
        int diff = total2 - total1;
        int years = diff / 360;
        int months = (diff - years * 360) / 30;
        int days = diff - years * 360 - months * 30 + daysInc;

        if (from->day == 1 && to->day == 31) {
            months--;
            days = 30;
        }
        return abs(diff);
    }

    bool checkIfNotDuplicate(Worker *worker) const {
        Worker *current = head;
        while (current) {
            if (Priority_Worker_Results_List::compareNames(current->name, worker->name)) {
                if (Priority_Worker_Results_List::compareNames(current->surname, worker->surname)) {
                    if (current->birthday->year == worker->birthday->year) {
                        if (current->birthday->month == worker->birthday->month) {
                            if (current->birthday->day == worker->birthday->day) {
                                return false;
                            }
                        }
                    }
                }
            }
            current = current->next;
        }
        return true;
    }

    void printWorkerWithClosestBirthday(Date *date, std::fstream &output) const {
        Worker *current = head;
        bool are_multiple = false;
        int worker_count = 0;
        // figure out if there are multiple results
        while (current) {
            if (current->birthday->month == date->month && current->birthday->day == date->day) {
                worker_count++;
            }
            current = current->next;
        }

        if (worker_count > 1) {
            are_multiple = true;
        }

        if (!are_multiple) {
            current = head;
            while (current) {
                if (current->birthday->month == date->month && current->birthday->day == date->day) {
                    output << Date::calculateAge(date->year, current->birthday->year) << ' ' <<
                           current->name << ' ' << current->surname << '\n';
                }
                current = current->next;
            }
        }

        if (are_multiple) {
            Priority_Worker_Results_List result_list;
            current = head;
            while (current) {
                if (current->birthday->month == date->month && current->birthday->day == date->day) {
                    result_list.insert(current, date);
                }
                current = current->next;
            }
            result_list.printPriorityResult(output);
        }
    }
};

int main() {
    std::fstream anniversary_in, anniversary_out;
    anniversary_in.open("anniversary.in", std::ios::in);
    anniversary_out.open("anniversary.out", std::ios::out);
    Worker_List workerList;
    char action;
    while (anniversary_in >> action) {
        if (action == 'P') {
            int nameLength = 0;
            char name[31];
            for (char &j : name) {
                j = '\0';
            }
            char space1;
            anniversary_in.get(space1);
            char name_temp;
            anniversary_in.get(name_temp);
            while (true) {
                if (name_temp == ' ') {
                    break;
                }
                name[nameLength] = name_temp;
                nameLength++;
                anniversary_in.get(name_temp);
            }

            char surname[31];
            for (char &k : surname) {
                k = '\0';
            }
            char surname_temp;
            anniversary_in.get(surname_temp);
            int surnameLength = 0;
            while (true) {
                if (surname_temp == ' ') {
                    break;
                }
                surname[surnameLength] = surname_temp;
                surnameLength++;
                anniversary_in.get(surname_temp);
            }

            int date, month, year;
            Date *worker_date = new Date;
            anniversary_in >> date;
            worker_date->day = date;
            char dot = '.';
            anniversary_in >> dot;
            anniversary_in >> month;
            worker_date->month = month;
            anniversary_in >> dot;
            anniversary_in >> year;
            worker_date->year = year;

            workerList.addWorker(name, nameLength, surname, surnameLength, worker_date);
        }
        if (action == 'D') {
            int date, month, year;
            Date *today = new Date;
            anniversary_in >> date;
            today->day = date;
            char dot = '.';
            anniversary_in >> dot;
            anniversary_in >> month;
            today->month = month;
            anniversary_in >> dot;
            anniversary_in >> year;
            today->year = year;

            Date *closest_date = workerList.findClosestAnniversary(today);
            Date::printDate(closest_date, anniversary_out);
            anniversary_out << '\n';
            workerList.printWorkerWithClosestBirthday(closest_date, anniversary_out);
        }
    }
    anniversary_in.close();
    anniversary_out.close();
    return 0;
}