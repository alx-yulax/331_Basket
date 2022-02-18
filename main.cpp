#include <iostream>
#include <map>
#include <exception>

class NullItems : public std::exception {
public:
    const char *what() const noexcept override {
        return "item is NULL";//"does not exist in stock";
    }
};

class NotEnough : public std::exception {
    std::string text;
public:
    NotEnough(std::string inCode, int inRemaining, int inQuantity)
    {
        text = "Not enough! Code: " + inCode + "  remaining: "
               + std::to_string(inRemaining) + " quantity: "
               + std::to_string(inQuantity);
    };

    const char *what() const noexcept override {
        return text.c_str();
    }
};

void check_arguments(std::string code, int quantity) {
    if (code.empty()) {
        throw std::invalid_argument("code is empty");
    }
    if (quantity <= 0) {
        throw std::invalid_argument("quantity <= 0");
    }
}


class Store {
    std::map<std::string, int> warehouse;
public:
    void add(std::string code, int quantity) {
        warehouse[code] += quantity;
    }

    void remove(std::string code, int quantity) {
        int remaining = getRemainingItems(code);
        if (remaining >= quantity) {
            warehouse[code] -= quantity;
        } else {
            throw NotEnough(code, remaining, quantity);
        }
    }

    int getRemainingItems(std::string code) {
        if (auto pos = warehouse.find(code); pos != warehouse.end()) {
            return pos->second;
        }
        throw NullItems();
    }

    void showRemaining() {
        std::cout << "Remaining in store:" << std::endl;
        for (const auto&[key, value]: warehouse) {
            std::cout << '[' << key << "] = " << value << "; " << std::endl;
        }
    }
};

class Basket {
    Store &store;
    std::map<std::string, int> basket;
public:
    Basket(Store &inStore) : store(inStore) {}

    void add(std::string code, int quantity) {
        store.remove(code, quantity);
        basket[code] += quantity;
        std::cout << "basket " << basket[code] << " code: " << code << "  quantity: " << quantity;
    }

    void remove(std::string code, int quantity) {
        int remaining = getRemainingItems(code);
        if (remaining >= quantity) {
            basket[code] -= quantity;
            store.add(code, quantity);
        } else {
            throw NotEnough(code, remaining, quantity);
        }
    }

    int getRemainingItems(std::string code) {
        if (auto pos = basket.find(code); pos != basket.end()) {
            return pos->second;
        }
        throw NullItems();
    }

    void showRemaining() {
        std::cout << "Remaining in basket:" << std::endl;
        for (const auto&[key, value]: basket) {
            std::cout << '[' << key << "] = " << value << "; " << std::endl;
        }
    }
};


int main() {
    Store ikea;

    std::cout << "Warehouse filling." << std::endl;
    while (true) {
        std::string code;
        int quantity;
        std::cout << "Input code (\"end\" to stop): ";
        std::cin >> code;
        if (code == "end") {
            break;
        }
        std::cout << "Input quantity: ";
        std::cin >> quantity;
        try {
            check_arguments(code, quantity);
            ikea.add(code, quantity);
        } catch (std::invalid_argument &x) {
            std::cout << "invalid argument:" << x.what() << std::endl;
            std::cin.clear();
        }
    }

    ikea.showRemaining();
    Basket myBasket(ikea);

    std::string command;
    while (true) {
        std::string code;
        int quantity;
        std::cout << "Input command (add, remove or end), code and quantity: ";
        std::cin.clear();
        std::cin >> command;
        if (command == "end") break;
        std::cin >> code >> quantity;
        if (command == "add") {
            try {
                check_arguments(code, quantity);
                myBasket.add(code, quantity);
            } catch (NullItems &x) {
                std::cout << "store: " << x.what() << std::endl;
            } catch (NotEnough &x) {
                std::cout << "store: " << x.what() << std::endl;
            } catch (...) {
                std::cout << "store??????" << std::endl;
            }

        } else if (command == "remove") {
            try {
                check_arguments(code, quantity);
                myBasket.remove(code, quantity);
            } catch (NullItems &x) {
                std::cout << "basket: " << x.what() << std::endl;
            } catch (NotEnough &x) {
                std::cout << "basket: " << x.what() << std::endl;
            } catch (...) {
                std::cout << "basket??????" << std::endl;
            }

        }

        myBasket.showRemaining();
        ikea.showRemaining();
    }

    myBasket.showRemaining();
    ikea.showRemaining();

    return 0;
}