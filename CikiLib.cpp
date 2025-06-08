#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>
using namespace std;

const int MAX_BOOK = 100;
const int MAX_MEMBER = 100;
const int MAX_RACK = 5;

struct Book {
    int id;
    string title;
    string author;
    int rack;
    bool isBorrowed;
};

struct Member {
    int id;
    string name;
};

struct StackNode {
    int bookID;
    StackNode* next;
};

struct MemberNode {
    Member data;
    MemberNode* left;
    MemberNode* right;
};

MemberNode* insertMember(MemberNode* root, Member newMember) {
    if (!root) return new MemberNode{newMember, nullptr, nullptr};
    if (newMember.id < root->data.id)
        root->left = insertMember(root->left, newMember);
    else
        root->right = insertMember(root->right, newMember);
    return root;
}

void inorderMembers(MemberNode* root) {
    if (root) {
        inorderMembers(root->left);
        cout << left << setw(6) << root->data.id << "| "
             << setw(25) << root->data.name << endl;
        inorderMembers(root->right);
    }

}

StackNode* returnStack = nullptr;

void pushReturnStack(int bookID) {
    StackNode* node = new StackNode{bookID, returnStack};
    returnStack = node;
}

int borrowQueue[MAX_BOOK];
int frontQ = 0, rearQ = 0;

void enqueueBorrow(int bookID) {
    if (rearQ < MAX_BOOK)
        borrowQueue[rearQ++] = bookID;
    else
        cout << "Queue Full!\n";
}

Book books[MAX_BOOK];
int bookCount = 0;
int memberIDCounter = 1;
int bookIDCounter = 1;
MemberNode* memberRoot = nullptr;

int rackIndexes[MAX_RACK][MAX_BOOK];
int rackCounts[MAX_RACK] = {0};

bool backToMenu() {
    char choice;
    while (true) {
        for (int i = 0; i < 3; ++i) {
        cout << " ";
        this_thread::sleep_for(chrono::milliseconds(500));
    }
        cout << "\n====================================\n";
        cout << "= Back to menu? (y/n): ";
        cin >> choice;
        if (choice == 'n' || choice == 'N') {
            cout << "\n====================================\n";
            cout << "= Thank you for using CikiLib!\n";
            cout << "= Stay literated :)\n";
            cout << "= Exiting";
            for (int i = 0; i < 4; ++i) {
                cout << ".";
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            cout << endl;
            exit(0);

        } else if (choice == 'y' || choice == 'Y') {
            return true;
        } else {
            cout << "= Invalid input. Please enter 'y' or 'n'.\n";
        }
    }
}

void addMember() {
    system("cls");
    Member m;
    cout << "====================================\n";
    cout << "|             Add Members          |\n";
    cout << "====================================\n";
    bool valid = false;
    do {
        cout << "\n= Please enter member name: ";
        cin.ignore();
        getline(cin, m.name);

        valid = true;
        for (char c : m.name) {
            if (!isalpha(c) && c != ' ') {
                valid = false;
                break;
            }
        }

        if (!valid) {
            cout << "\n= Oops!\n";
            cout << "= Invalid input. Please input only letters:).\n";
        }
    } while (!valid);

    m.id = memberIDCounter++;
    memberRoot = insertMember(memberRoot, m);
    cout << "\n====================================\n";

    cout << "= Member added successfully!\n";
    cout << "= Name: " << m.name << endl;
    cout << "= ID  : " << m.id << endl;
    backToMenu();
}

void showAllMembers() {
    system("cls");
    cout << "====================================\n";
    cout << "|           CikiLib Members        |\n";
    cout << "====================================\n";
    if (memberRoot == nullptr) {
        cout << "\n= Empty data..\n";
        cout << "= There's nothing to see here-yet\n";
    } else {
        cout << endl;
        cout << string(33, '-') << endl;
        cout << left << setw(6) << "ID" << "| " << setw(25) << "Name" << endl;
        cout << string(33, '-') << endl;
        inorderMembers(memberRoot);
    }
    backToMenu();
}
void addBook() {
    system("cls");
    cout << "====================================\n";
    cout << "|          Add Collections         |\n";
    cout << "====================================\n";
    if (bookCount >= MAX_BOOK) {
        cout << "= Sorry, book limit reached!\n";
        backToMenu();
        return;
    }

    Book b;
    cout << "= Please enter book data:\n";

    cin.ignore();
    cout << "= Book title: ";
    getline(cin, b.title);

    bool valid = false;
    do {
        cout << "= Author    : ";
        getline(cin, b.author);

        valid = true;
        for (char c : b.author) {
            if (!isalpha(c) && c != ' ') {
                valid = false;
                break;
            }
        }

        if (!valid) {
            cout << "\n= Oops! Invalid input\n";
            cout << "= Author name must contain letters only.\n";
        }
    } while (!valid);

    do {
        cout << "= Rack (0-" << MAX_RACK - 1 << "): ";
        cin >> b.rack;

        if (cin.fail() || b.rack < 0 || b.rack >= MAX_RACK) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n= Oops! Invalid input\n";
            cout << "= Please input a valid rack number between 0 and " << MAX_RACK - 1 << ".\n";
            this_thread::sleep_for(chrono::seconds(1));

        } else {
            break;
        }
    } while (true);

    b.id = bookIDCounter++;
    b.isBorrowed = false;
    books[bookCount] = b;
    rackIndexes[b.rack][rackCounts[b.rack]++] = bookCount;
    ++bookCount;

    cout << "====================================\n";
    cout << "\n= Book added successfully!\n";
    cout << "= Title: " << b.title << endl;
    cout << "= ID   : " << b.id << endl;
    cout << "= Rack : " << b.rack << endl;
    backToMenu();
}



void showAllBooks() {
    system("cls");
    cout << "===========================================\n";
    cout << "|            CikiLib Collections          |\n";
    cout << "===========================================\n";
    if (bookCount == 0) {
        cout << "\n= Empty data..\n";
        cout << "= There's nothing to see here-yet.\n";

    } else {
        cout << "\n= Our Collections:\n";
        cout << string(44, '-') << endl;
        cout << left << setw(6) << "ID" << setw(10) << "Title"
             << setw(10) << "Author" << setw(10) << "Rack"
             << setw(10) << "Status" << endl;
        cout << string(44, '-') << endl;

        for (int i = 0; i < bookCount; ++i) {
            Book b = books[i];
            cout << left << setw(6) << b.id << setw(10) << b.title
                 << setw(10) << b.author << setw(10) << b.rack
                 << setw(10) << (b.isBorrowed ? "Borrowed" : "Available") << endl;
        }
    }
    backToMenu();
}

void borrowBook() {
    system("cls");
    cout << "====================================\n";
    cout << "|           Borrowing Book         |\n";
    cout << "====================================\n";

    int id;
    char choice;
    cout << "= Please enter book ID to borrow: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "= Invalid input.\n";
        cout << "= Book ID must be a number.\n";
        cout << "= Try again? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            borrowBook();  // rekursif jika ingin coba lagi
            return;
        } else if (choice == 'n' || choice == 'N') {
            backToMenu();
            return;
        }
        return;
    }

    for (int i = 0; i < bookCount; ++i) {
        if (books[i].id == id && !books[i].isBorrowed) {
            books[i].isBorrowed = true;
            enqueueBorrow(id);
            cout << "= \"" << books[i].title << "\" is borrowed successfully!\n";
            backToMenu();
            return;
        }
    }

    cout << "= Book ID not found or already borrowed.\n";
    cout << "= Try again? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        borrowBook();
    } else {
        backToMenu();
    }
}


void returnBook() {
    system("cls");
    cout << "====================================\n";
    cout << "|            Book Returns          |\n";
    cout << "====================================\n";

    int id;
    cout << "\n= Please enter book ID to return: ";
    cin >> id;

    for (int i = 0; i < bookCount; ++i) {
        if (books[i].id == id && books[i].isBorrowed) {
            books[i].isBorrowed = false;
            pushReturnStack(id);
            cout << "= " << books[i].title << " returned successfully!\n";
            backToMenu();
            return;
        }
    }

    char choice;
    while (true) {
        cout << "= Invalid book ID or already returned.\n";
        cout << "= Try again? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            returnBook();
            return;
        } else if (choice == 'n' || choice == 'N') {
            backToMenu();
            return;
        } else {
            cout << "= Invalid input.\n";
        }
    }
}

void showBooksInRack() {
    system("cls");
    cout << "====================================\n";
    cout << "|           Books Storage           |\n";
    cout << "====================================\n";

    int rack;
    cout << "\n= Please enter rack number (0-" << MAX_RACK - 1 << "): ";
    cin >> rack;

    if (rack < 0 || rack >= MAX_RACK) {
        char choice;
        while (true) {
            cout << "====================================\n";
            cout << "= Invalid rack.\n= Check another rack? (y/n): ";
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                showBooksInRack();
                return;
            } else if (choice == 'n' || choice == 'N') {
                backToMenu();
                return;
            } else {
                cout << "= Invalid input.\n";
            }
        }
    }

    if (rackCounts[rack] == 0) {
        char choice;
        cout << "\n= Empty rack...\n";
        cout << "= Check another rack? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            showBooksInRack();
        } else {
            backToMenu();
        }
        return;
    }

    cout << string(36, '-') << endl;
    cout << left << setw(6) << "ID"
         << setw(10) << "Title"
         << setw(10) << "Author"
         << setw(10) << "Status" << endl;
    cout << string(36, '-') << endl;

    for (int i = 0; i < rackCounts[rack]; ++i) {
        Book b = books[rackIndexes[rack][i]];
        cout << left << setw(6)  << b.id
                     << setw(10) << b.title
                     << setw(10) << b.author
                     << setw(10) << (b.isBorrowed ? "Borrowed" : "Available") << endl;
    }
    backToMenu();
}

void menu() {
    int choice;
    do {
        system("cls");
        cout << "====================================\n";
        cout << "|         * CIKINI LIBRARY *       |\n";
        cout << "|            MAIN MENU             |\n";
        cout << "====================================\n";
        cout << "| 1 | Add Member                   |\n";
        cout << "| 2 | Add Book                     |\n";
        cout << "| 3 | View CikiLib Collections     |\n";
        cout << "| 4 | View CikiLib Members         |\n";
        cout << "| 5 | Borrow Book                  |\n";
        cout << "| 6 | Return Book                  |\n";
        cout << "| 7 | Collections Storage          |\n";
        cout << "| 0 | Exit                         |\n";
        cout << "------------------------------------\n";
        cin >> choice;

        switch (choice) {
            case 1: addMember(); break;
            case 2: addBook(); break;
            case 3: showAllBooks(); break;
            case 4: showAllMembers(); break;
            case 5: borrowBook(); break;
            case 6: returnBook(); break;
            case 7: showBooksInRack(); break;
            case 0:
                cout << "= Exiting";
                for (int i = 0; i < 4; ++i) {
                    cout << ".";
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
                cout << "\n= Thank you for using CikiLib!\n";
                break;
            default: cout << "= Invalid choice.\n"; this_thread::sleep_for(chrono::seconds(1)); break;
        }
    } while (choice != 0);
}

int main() {
    cout << "=====================================\n";
    cout << "           Tugas Pertemuan 13        \n";
    cout << "=====================================\n";
    cout << "= Nama     : Alia Hamzah\n";
    cout << "= NPM      : 241063117006\n";
    cout << "= Kelas    : 2B IF-2025\n";
    cout << "=====================================\n";
    for (int i = 0; i < 3; ++i) {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(500));

    } system("cls");

    cout << "=====================================\n";
    cout << ":: Welcome to Cikini Library!\n";
    cout << ":: Nice to see you again\n";
    cout << ":: Loading";
    for (int i = 0; i < 4; ++i) {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << endl;
    menu();
    return 0;
}
