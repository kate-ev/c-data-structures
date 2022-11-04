#include <fstream>


struct Node {
long int departure;
int status;
long int id;
int mnr;
struct Node *l;
};

class PriorityQueue {
public:
    Node *f;
public:
    PriorityQueue() {
        f = NULL;
    }

void insert(int i, int p1, int p2, int mnr) {
    Node *t, *q;
    t = new Node;
    t->id = i;
    t->departure = p1;
    t->status = p2;
    t->mnr = mnr;
    if (f == NULL || p1 < f->departure) {
        t->l = f;
        f = t;
    } else {
        q = f;
        while (q->l != NULL) {
            if(q->l->departure < p1) {
                q = q->l;
                continue;
            }
            if(q->l->departure == p1 && q->l->status > p2) {
                q = q->l;
                continue;
            }
            if(q->l->departure == p1 && q->l->status == p2 && q->l->mnr < mnr) {
                q = q->l;
                continue;
            }
            break;
        }
        t->l = q->l;
        q->l = t;
    }
};

void print(std::fstream &output) {
    Node *ptr;
    ptr = f;
    if (f == NULL)
    output << " ";
    else {
        while(ptr != NULL) {
            output << ptr->id << " " << ptr->departure << std::endl;
            ptr = ptr->l;
        }
    }
};

    void del() {
        Node *t;
        t = f;
        f = f->l;
        free(t);
    }
};

int main() {
    std::fstream customsin, customsout;
    int p_count, n_count, p_time, n_time;

    customsin.open("customs.in", std::ios::in);
    customsout.open("customs.out", std::ios::out);

    customsin >> p_count;
    int * p_customs = new int[p_count];
    long int * p_free = new long int[p_count];

    customsin >> n_count;
    int * n_customs = new int[n_count];
    long int * n_free = new long int[n_count];

    customsin >> p_time;
    for (int i = 0; i < p_count; i++) {
        p_customs[i] = p_time;
    }
    for (int i = 0; i < p_count; i++) {
        p_free[i] = 0;
    }

    customsin >> n_time;
    for (int i = 0; i < n_count; i++) {
        n_customs[i] = n_time;
    }
    for (int i = 0; i < n_count; i++) {
        n_free[i] = 0;
    }

    PriorityQueue queueAll;

    bool data = false;

    while(customsin) {
        char letter;
        customsin >> letter;
        if (letter == 'T') {
            char type;
            customsin >> type;
            int id;
            customsin >> id;
            int new_time;
            customsin >> new_time;
            if(type == 'P') {
                p_customs[id - 1] = new_time;
            }
            if(type == 'N') {
                n_customs[id - 1] = new_time;
            }
            continue;
        }
        if (letter == 'X') {
            if(data == false) {
                customsout << "nothing";
            }
            break;
        }
        long int id;
        customsin >> id;
        if (letter == 'P') {
            long int departure_time;
            bool no_free_custom = true;
            int priority = 1;
            int customNr;
            for (int i = 0; i < p_count; i++) {
                if(p_free[i] <= id) {
                    departure_time = p_customs[i] + id;
                    customNr = i;
                    p_free[customNr] = departure_time;
                    no_free_custom = false;
                    break;
                }
            }
            if(no_free_custom) {
                int closest_available = p_free[0];
                for (int i = 0; i < p_count; i++) {
                    if (closest_available > p_free[i]) {
                        closest_available = p_free[i];
                    }
                }
                for (int i = 0; i < p_count; i++) {
                    if(p_free[i] == closest_available) {
                        customNr = i;
                        break;
                    }
                }
                departure_time = p_free[customNr] + p_customs[customNr];
                p_free[customNr] = departure_time;
            }
            queueAll.insert(id, departure_time, priority, customNr + 1);
            data = true;
        }
        if (letter == 'N') {
            long int departure_time;
            bool no_free_custom = true;
            int priority = 0;
            int customNr;
            for (int i = 0; i < n_count; i++) {
                if(n_free[i] <= id) {
                    departure_time = n_customs[i] + id;
                    customNr = i;
                    n_free[customNr] = departure_time;
                    no_free_custom = false;
                    break;
                }
            }
            if(no_free_custom) {
                int closest_available = n_free[0];
                for (int i = 0; i < n_count; i++) {
                    if (closest_available > n_free[i]) {
                        closest_available = n_free[i];
                    }
                }
                for (int i = 0; i < n_count; i++) {
                    if(n_free[i] == closest_available) {
                        customNr = i;
                        break;
                    }
                }
                departure_time = n_free[customNr] + n_customs[customNr];
                n_free[customNr] = departure_time;
            }
            queueAll.insert(id, departure_time, priority, customNr + 1);
            data = true;
        }
    }
    queueAll.print(customsout);
    customsin.close();
    customsout.close();
    delete[] n_customs;
    delete[] p_customs;
    delete[] p_free;
    delete[] n_free;

    return 0;
}
