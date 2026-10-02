#include <sstream>
#include <iomanip>
#include "book.h"
#include "util.h"

using namespace std;

Book::Book(const std::string name, double price, int qty,
           const std::string isbn, const std::string author) :
    Product("book", name, price, qty),
    isbn_(isbn),
    author_(author)
{
}

Book::~Book()
{
}

std::set<std::string> Book::keywords() const
{
    set<string> keys = parseStringToWords(name_);
    set<string> authorKeys = parseStringToWords(author_);
    keys = setUnion(keys, authorKeys);
    keys.insert(convToLower(isbn_));
    return keys;
}

std::string Book::displayString() const
{
    stringstream ss;
    ss << name_ << "\n";
    ss << "Author: " << author_ << " ISBN: " << isbn_ << "\n";
    ss << fixed << setprecision(2) << price_ << " " << qty_ << " left.";
    return ss.str();
}

void Book::dump(std::ostream& os) const
{
    Product::dump(os);
    os << isbn_ << "\n" << author_ << endl;
}