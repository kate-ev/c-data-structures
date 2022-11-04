#include <fstream>

struct Reviewer {
    char name[100]{};
    long int nickname[50]{};
    struct Reviewer *next{};

    Reviewer() {
        for (char &j : name) {
            j = '\0';
        }
        for (long &i : nickname) {
            i = 0;
        }
    }
};

class ReviewerList {
public:
    Reviewer *head, *tail;
public:
    ReviewerList() {
        head = nullptr;
        tail = nullptr;
    }

    void addReviewer(const char *name, int length) {
        auto *current = new Reviewer;
        for (int i = 0; i < length; i++) {
            current->name[i] = name[i];
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

    bool addReviewerNicknames(char *searchName, int insertCount, const long int *nicknames) const {
        Reviewer *current = head;
        while (current) {
            if (compareNames(current->name, searchName)) {
                int countNicks = getCount(current->nickname);
                if (countNicks > 50 || (insertCount + countNicks) > 50) return false;
                for (int i = 0; i < insertCount; i++) {
                    current->nickname[countNicks + i] = nicknames[i];
                }
                break;
            }
            current = current->next;
        }
        return true;
    }

    bool findReviewerByName(char *searchName) const {
        Reviewer *current = head;
        char thisName[100] = {'\0'};
        while (current) {
            for (int i = 0; current->name[i] != '\0'; i++) {
                thisName[i] = current->name[i];
            }
            if (compareNames(searchName, thisName)) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool checkIfCanAddNicknames(char *searchName, int insertCount, const long int *nicknames) const {
        Reviewer *current = head;
        while (current) {
            int count = getCount(current->nickname);
            if (compareNames(current->name, searchName)) {
                if (count > 50 || (insertCount + count) > 50) return false;
            }
            for (int i = 0; i < insertCount; i++) {
                for (int j = 0; j < count; j++) {
                    if (current->nickname[j] == nicknames[i]) return false;
                }
            }
            current = current->next;
        }
        return true;
    }

    bool skipNonUniqueNickname(char *searchName, long int nickname) const {
        Reviewer *current = head;
        while (current) {
            if (compareNames(current->name, searchName)) {
                int count = getCount(current->nickname);
                for (int i = 0; i < count; i++) {
                    if (current->nickname[i] == nickname) return true;
                }
            }
            current = current->next;
        }
        return false;
    }

    Reviewer *findReviewerByNickname(long int nickname) const {
        Reviewer *current = head;
        while (current) {
            int count = getCount(current->nickname);
            for (int i = 0; i < count; i++) {
                if (current->nickname[i] == nickname) return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    static bool compareNames(const char name1[], const char name2[]) {
        int i = 0;
        while (name1[i] != '\0' || name2[i] != '\0') {
            if (name1[i] != name2[i]) {
                return false;
            }
            i++;
        }
        return true;
    }

    static int getCount(const long int *elements) {
        int count = 0;
        while (elements[count] != 0) count++;
        return count;
    }

    bool deleteReviewer(long int nickname) {
        Reviewer *temp = head;
        Reviewer *previous = nullptr;

        if (temp != nullptr) {
            int count = getCount(temp->nickname);
            for (int i = 0; i < count; i++) {
                if (temp->nickname[i] == nickname) {
                    head = temp->next;
                    delete temp;
                    return true;
                }
            }
        }
        while (temp != nullptr) {
            int count = getCount(temp->nickname);
            for (int i = 0; i < count; i++) {
                if (temp->nickname[i] == nickname) {
                    previous->next = temp->next;
                    delete temp;
                    return true;
                }
            }
            previous = temp;
            temp = temp->next;
        }
        return false;
    };
};

int main() {
    std::fstream review_in, review_out;
    review_in.open("reviewers.in", std::ios::in);
    review_out.open("reviewers.out", std::ios::out);
    auto *reviewerList = new ReviewerList();
    bool data = false;
    char type;
    while (review_in >> type) {
        if (review_in.peek() != '\n')  {
            if (type == 'I') {
                if (data) {
                    review_out << "\n";
                }
                char name[100];
                for (char &j : name) {
                    j = '\0';
                }
                int nameLength = 0;
                char space1;
                review_in.get(space1);
                char temp;
                review_in.get(temp);
                while (true) {
                    if (temp == ' ') {
                        break;
                    }
                    name[nameLength] = temp;
                    nameLength++;
                    review_in.get(temp);
                }
                bool addAnother = reviewerList->findReviewerByName(name);
                if (!addAnother) {
                    reviewerList->addReviewer(name, nameLength);
                }
                int nicknameCount;
                review_in >> nicknameCount;
                long int nicknames[nicknameCount];
                long int nickname;
                int extract = 0;
                int pos = 0;
                for (int k = 0; k < nicknameCount; k++) {
                    review_in >> nickname;
                    if (!reviewerList->skipNonUniqueNickname(name, nickname)) {
                        nicknames[pos] = nickname;
                        pos++;
                    } else {
                        extract++;
                    }
                }
                int nickCountToSend = nicknameCount - extract;
                long int nicksToSend[nickCountToSend];
                for (int m = 0; m < nickCountToSend; m++) {
                    nicksToSend[m] = nicknames[m];
                }
                if (reviewerList->checkIfCanAddNicknames(name, nickCountToSend, nicksToSend)) {
                    reviewerList->addReviewerNicknames(name, nickCountToSend, nicksToSend);
                    review_out << "ok";
                } else {
                    review_out << "no";
                }
                data = true;
                continue;
            }
            if (type == 'L') {
                if (data) {
                    review_out << "\n";
                }
                long int nickname;
                review_in >> nickname;
                Reviewer *temp = reviewerList->findReviewerByNickname(nickname);
                if (temp != nullptr) {
                    review_out << temp->name;
                } else {
                    review_out << "no";
                }
                data = true;
                continue;
            }
            if (type == 'D') {
                if (data) {
                    review_out << "\n";
                }
                long int nickname;
                review_in >> nickname;
                bool success = reviewerList->deleteReviewer(nickname);
                if (success) {
                    review_out << "ok";
                }
                else {
                    review_out << "no";
                }
                data = true;
                continue;
            }
        }
    }
    review_in.close();
    review_out.close();
    return 0;
}