#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits> // Required for numeric_limits

using namespace std;

// ===================================
// 1. BOOK CLASS (OOP)
// ===================================
class Book {
private:
    string title;
    string author;
    string isbn;
    int quantity;

public:
    // Constructor
    Book(string t = "", string a = "", string i = "", int q = 0)
        : title(t), author(a), isbn(i), quantity(q) {}

    // Getters (Accessors)
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    int getQuantity() const { return quantity; }

    // Display Method
    void displayBook() const {
        cout << "| Title: " << title 
             << " | Author: " << author 
             << " | ISBN: " << isbn 
             << " | Quantity: " << quantity << " |" << endl;
    }

    // Method to format data for saving to file
    string toFileString() const {
        // Use a delimiter (e.g., '|') that won't appear in the data
        return title + "|" + author + "|" + isbn + "|" + to_string(quantity);
    }
};

// ===================================
// 2. FILE HANDLING FUNCTIONS
// ===================================
const string FILENAME = "library_data.txt";

// Function to save all books to the file
void saveBooks(const vector<Book>& books) {
    ofstream outFile(FILENAME);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file for writing!" << endl;
        return;
    }

    for (const auto& book : books) {
        outFile << book.toFileString() << "\n";
    }
    outFile.close();
    cout << "✅ Library data saved successfully." << endl;
}

// Function to load all books from the file
vector<Book> loadBooks() {
    vector<Book> books;
    ifstream inFile(FILENAME);
    string line;

    if (!inFile.is_open()) {
        cout << "📚 Starting with a fresh library (file not found)." << endl;
        return books;
    }

    while (getline(inFile, line)) {
        stringstream ss(line);
        string segment;
        vector<string> segments;

        // Split the line by the delimiter '|'
        while (getline(ss, segment, '|')) {
            segments.push_back(segment);
        }

        // Ensure we have exactly 4 segments (Title, Author, ISBN, Quantity)
        if (segments.size() == 4) {
            try {
                // Convert string quantity to integer
                int quantity = stoi(segments[3]); 
                books.emplace_back(segments[0], segments[1], segments[2], quantity);
            } catch (const exception& e) {
                cerr << "Error loading book data: Invalid quantity format." << endl;
            }
        }
    }
    inFile.close();
    cout << "📖 " << books.size() << " books loaded from file." << endl;
    return books;
}

// ===================================
// 3. CORE LIBRARY FUNCTIONS
// ===================================
// Function to add a new book
void addBook(vector<Book>& books) {
    string title, author, isbn;
    int quantity;

    // Clear input buffer before reading strings
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "\n--- Add New Book ---\n";
    cout << "Enter Title: ";
    getline(cin, title);
    cout << "Enter Author: ";
    getline(cin, author);
    cout << "Enter ISBN: ";
    getline(cin, isbn);
    cout << "Enter Quantity: ";
    
    // Input validation for quantity
    while (!(cin >> quantity) || quantity < 1) {
        cout << "Invalid input. Please enter a positive number for Quantity: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Check if ISBN already exists
    for (const auto& book : books) {
        if (book.getISBN() == isbn) {
            cout << "\n⚠️ Book with ISBN " << isbn << " already exists. Use an Update function instead." << endl;
            return;
        }
    }
    
    books.emplace_back(title, author, isbn, quantity);
    cout << "\n➕ Book added successfully!" << endl;
}

// Function to display all books
void displayAllBooks(const vector<Book>& books) {
    if (books.empty()) {
        cout << "\n🗃️ The library is currently empty." << endl;
        return;
    }

    cout << "\n--- All Books in Library (" << books.size() << ") ---\n";
    for (const auto& book : books) {
        book.displayBook();
    }
    cout << "------------------------------------------\n";
}

// Function to search for a book
void searchBook(const vector<Book>& books) {
    if (books.empty()) {
        cout << "\n🗃️ The library is currently empty. Nothing to search." << endl;
        return;
    }
    
    int choice;
    string searchTerm;
    vector<Book> results;

    cout << "\n--- Search Book ---\n";
    cout << "Search by: 1. Title | 2. Author | 3. ISBN : ";
    if (!(cin >> choice)) {
        cout << "Invalid choice." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    
    // Clear buffer after reading int
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Enter search term: ";
    getline(cin, searchTerm);
    
    // Simple case-insensitive search (for strings)
    for (const auto& book : books) {
        if (choice == 1 && book.getTitle().find(searchTerm) != string::npos) {
            results.push_back(book);
        } else if (choice == 2 && book.getAuthor().find(searchTerm) != string::npos) {
            results.push_back(book);
        } else if (choice == 3 && book.getISBN() == searchTerm) {
            results.push_back(book);
        }
    }

    if (results.empty()) {
        cout << "\n❌ No books found matching your search term." << endl;
    } else {
        cout << "\n🔍 Found " << results.size() << " matching book(s):\n";
        for (const auto& book : results) {
            book.displayBook();
        }
    }
}

// ===================================
// 4. MAIN PROGRAM & MENU
// ===================================
void showMenu() {
    cout << "\n===== Library Management System (CLI) =====" << endl;
    cout << "1. Add New Book" << endl;
    cout << "2. Display All Books" << endl;
    cout << "3. Search Book" << endl;
    cout << "4. Save & Exit" << endl;
    cout << "Enter your choice (1-4): ";
}

int main() {
    // 1. Load data at startup
    vector<Book> library = loadBooks(); 
    int choice;

    while (true) {
        showMenu();

        // Input validation for menu choice
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addBook(library);
                break;
            case 2:
                displayAllBooks(library);
                break;
            case 3:
                searchBook(library);
                break;
            case 4:
                // 2. Save data before exiting
                saveBooks(library); 
                cout << "\nGoodbye! 👋" << endl;
                return 0; // Exit the program
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
                break;
        }
    }

    return 0;
}
