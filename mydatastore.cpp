#include <iostream>
#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for (size_t i = 0; i < products_.size(); i++) {
        delete products_[i];
    }
    for (map<string, User*>::iterator item = users_.begin(); item != users_.end(); ++item) {
        delete item->second;
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.push_back(p);

    set<string> keys = p->keywords();
    for (set<string>::iterator item = keys.begin(); item != keys.end(); ++item) {
        keywordMap_[*item].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    users_[convToLower(u->getName())] = u;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type)
{
    vector<Product*> hits;
    if (terms.size() == 0) {
        return hits;
    }

    set<Product*> result = keywordMap_[convToLower(terms[0])];

    for (size_t i = 1; i < terms.size(); i++) {
        set<Product*> matches = keywordMap_[convToLower(terms[i])];
        if (type == 0) {
            result = setIntersection(result, matches);
        }
        else {
            result = setUnion(result, matches);
        }
    }

    for (set<Product*>::iterator item = result.begin(); item != result.end(); ++item) {
        hits.push_back(*item);
    }
    return hits;
}

void MyDataStore::dump(std::ostream& ofile)
{
    ofile << "<products>" << endl;
    for (size_t i = 0; i < products_.size(); i++) {
        products_[i]->dump(ofile);
    }
    ofile << "</products>" << endl;

    ofile << "<users>" << endl;
    for (map<string, User*>::iterator item = users_.begin(); item != users_.end(); ++item) {
        item->second->dump(ofile);
    }
    ofile << "</users>" << endl;
}

bool MyDataStore::addToCart(std::string username, Product* p)
{
    username = convToLower(username);
    if (users_.find(username) == users_.end()) {
        return false;
    }
    carts_[username].push_back(p);
    return true;
}

bool MyDataStore::viewCart(std::string username)
{
    username = convToLower(username);
    if (users_.find(username) == users_.end()) {
        return false;
    }

    vector<Product*>& cart = carts_[username];
    for (size_t i = 0; i < cart.size(); i++) {
        cout << "Item " << i + 1 << endl;
        cout << cart[i]->displayString() << endl;
        cout << endl;
    }
    return true;
}

bool MyDataStore::buyCart(std::string username)
{
    username = convToLower(username);
    map<string, User*>::iterator found = users_.find(username);
    if (found == users_.end()) {
        return false;
    }

    User* user = found->second;
    vector<Product*>& cart = carts_[username];
    vector<Product*> remaining;

    for (size_t i = 0; i < cart.size(); i++) {
        Product* p = cart[i];
        if (p->getQty() > 0 && user->getBalance() >= p->getPrice()) {
            p->subtractQty(1);
            user->deductAmount(p->getPrice());
        }
        else {
            remaining.push_back(p);
        }
    }

    cart = remaining;
    return true;
}
