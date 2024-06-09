#include "big.h"

const int maxdigits = 1e6;

struct big::node {
    short data;
    node *flw, *prev;
    node (short data = -1, node *flw = nullptr, node *prev = nullptr): data(data), flw(flw), prev(prev) {}
};

big::big(): negative(false), frontdigit(nullptr), backdigit(nullptr), length(0) {}
big::big(long long x) : big() {
    if (x == 0) {
        push_front(0);
        return;
    }
    if (x < 0) {
        negative = true;
        x = -x;
    }
    while (x > 0) {
        push_front(x%10);
        x /= 10;
    }
}
big::big(const big &from): big() {
    negative = from.negative;
    node *curr = from.frontdigit;
    while (curr != nullptr) {
        push_back(curr->data);
        curr = curr->flw;
    }
}
big::~big() {
    node* curr = frontdigit;
    while (curr != nullptr) {
        node* temp = curr;
        curr = curr->flw;
        delete temp;
    }
}

std::istream& operator>>(std::istream &is, big &target) {
    char ch;
    while (!isdigit(ch = is.get()) && ch != '-');
    if (ch == '-') {
        target.negative = true;
        ch = is.get();
    }
    do
        target.push_back(ch-'0');
    while (isdigit(ch = is.get()));
    return is;
}

std::ostream& operator<<(std::ostream &os, const big &target) {
    if (target.negative)
        os << '-';
    big::node *curr = target.frontdigit;
    while (curr != nullptr) {
        os << curr->data;
        curr = curr->flw;
    }
    return os;
}

big& big::operator=(const big &from) {
    if (this == &from)
        return *this;
    
    while (length > 0)
        pop_front();

    negative = from.negative;
    node *curr = from.frontdigit;
    while (curr != nullptr) {
        push_back(curr->data);
        curr = curr->flw;
    }

    return *this;
}

bool big::operator<(const big &comp) const {
    if (negative != comp.negative)
        return negative > comp.negative;
    if (size() != comp.size())
        return negative ^ (size() < comp.size());
    node *b1 = front(), *b2 = comp.front();
    while (b1 != nullptr) {
        if (b1->data != b2->data)
            return negative ^ (b1->data < b2->data);
        b1 = b1->flw;
        b2 = b2->flw;
    }
    return false;
}

bool big::operator==(const big &comp) const {
    if (negative != comp.negative)
        return false;
    if (size() != comp.size())
        return false;
    node *b1 = front(), *b2 = comp.front();
    while (b1 != nullptr) {
        if (b1->data != b2->data)
            return false;
        b1 = b1->flw;
        b2 = b2->flw;
    }
    return true;
}

bool big::operator!=(const big &comp) const {
    return !(*this == comp);
}

bool big::operator<=(const big &comp) const {
    return *this < comp || *this == comp;
}

bool big::operator>(const big &comp) const {
    return comp < *this;
}

bool big::operator>=(const big &comp) const {
    return *this > comp || *this == comp;
}

big big::operator+(const big &add) const {
    big res;
    if (negative != add.negative) {
        big a = this->abs();
        big b = add.abs();
        if (a > b)
            std::swap(a, b);
        res = a - b;
        res.negative = this->negative ^ (this->abs() < add.abs());
        return res;
    }
    res.negative = negative;
    node *b1 = back(), *b2 = add.back();
    int t = 0;
    while (b1 != nullptr && b2 != nullptr) {
        t += b1->data + b2->data;
        res.push_front(t%10);
        t /= 10;
        b1 = b1->prev;
        b2 = b2->prev;
    }
    while (b1 != nullptr) {
        t += b1->data;
        res.push_front(t%10);
        t /= 10;
        b1 = b1->prev;
    }
    while (b2 != nullptr) {
        t += b2->data;
        res.push_front(t%10);
        t /= 10;
        b2 = b2->prev;
    }
    if (t > 0)
        res.push_front(t);
    return res;
}

big& big::operator+=(const big &add) {
    *this = *this + add;
    return *this;
}

big& big::operator++() {
    *this = *this + 1;
    return *this;
}

big big::operator++(int) {
    big temp(*this);
    *this = *this + 1;
    return temp;
}

big big::operator-() const {
    big res(*this);
    res.negative = !res.negative;
    return res;
}

big big::operator-(const big &sub) const {
    big from(*this), to = sub, res;
    if (from.negative != to.negative) {
        res = from + (-to);
        return res;
    }
    res.negative = from.negative ^ (from.abs() < to.abs());
    if (from < to)
        std::swap(from, to);
    node *b1 = from.back(), *b2 = to.back();
    int t = 0;
    while (b1 != nullptr && b2 != nullptr) {
        int digit = b1->data - b2->data - t;
        if (digit < 0) {
            digit += 10;
            t = 1;
        }
        res.push_front(digit%10);
        b1 = b1->prev;
        b2 = b2->prev;
    }
    while (b1 != nullptr) {
        int digit = b1->data - t;
        if (digit < 0) {
            digit += 10;
            t = 1;
        }
        res.push_front(digit%10);
        b1 = b1->prev;
    }
    while (b2 != nullptr) {
        int digit = b2->data - t;
        if (digit < 0) {
            digit += 10;
            t = 1;
        }
        res.push_front(digit%10);
        b2 = b2->prev;
    }
    return res.trim();
}

big& big::operator-=(const big &add) {
    *this = *this - add;
    return *this;
}

big& big::operator--() {
    *this = *this - 1;
    return *this;
}

big big::operator--(int) {
    big temp(*this);
    *this = *this - 1;
    return temp;
}

big big::operator*(const big &mult) const {
    std::vector<big> aux(10);
    node *b1 = back();
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < i; j++)
            aux[i].push_front(0);
        int t = 0;
        node *b2 = mult.back();
        while (b2 != nullptr) {
            t += b1->data*b2->data;
            aux[i].push_front(t%10);
            t /= 10;
            b2 = b2->prev;
        }
        if (t > 0)
            aux[i].push_front(t);
        b1 = b1->prev;
    }
    big res = 0;
    for (const big &e: aux)
        res += e;
    res.negative = this->negative ^ mult.negative;
    return res.trim();
}

big& big::operator*=(const big &add) {
    *this = *this * add;
    return *this;
}

big big::operator/(const big &div) const {
    if (div == 0)
        throw std::runtime_error("Division by zero");

    big curr, cop;
    big quotient;
    node *b = front();
    while (b != nullptr) {
        curr.push_back(b->data);
        curr.trim();
        short x = 0;
        cop = curr;
        while (div <= curr) {
            curr -= div;
            x++;
        }
        quotient.push_back(x);
        curr = cop - div*x;
        b = b->flw;
    }
    quotient.negative = this->negative ^ div.negative;
    return quotient.trim();
}

big& big::operator/=(const big &add) {
    *this = *this / add;
    return *this;
}

big big::operator%(const big &mod) const {
    if (mod == 0)
        throw std::runtime_error("Modulo by zero");

    big curr, cop;
    node *b = front();
    while (b != nullptr) {
        curr.push_back(b->data);
        curr.trim();
        cop = curr;
        short x = 0;
        while (mod <= curr) {
            curr -= mod;
            x++;
        }
        curr = cop - mod*x;
        b = b->flw;
    }
    curr.negative = this->negative;
    return curr;
}

big& big::operator%=(const big &add) {
    *this = *this % add;
    return *this;
}

size_t big::size() const {
    return length;
}

big::node* big::front() const {
    if (frontdigit == nullptr)
        return nullptr;
    node *curr = frontdigit;
    while (curr->prev != nullptr)
        curr = curr->prev;
    return curr;
}

big::node* big::back() const {
    if (backdigit == nullptr)
        return nullptr; 
    big::node *curr = backdigit;
    while (curr->flw != nullptr)
        curr = curr->flw;
    return curr;
}

void big::push_front(short data) {
    node *newnode = new node(data, frontdigit, nullptr);
    if (frontdigit != nullptr) 
        frontdigit->prev = newnode;
    frontdigit = newnode;
    if (length == 0) 
        backdigit = newnode;
    length++;
}

void big::push_back(short data) {
    node *newnode = new node(data, nullptr, backdigit);
    if (backdigit != nullptr) 
        backdigit->flw = newnode;
    backdigit = newnode;
    if (length == 0) 
        frontdigit = newnode;
    length++;
}

void big::pop_front() {
    if (length == 0) 
        return;
    node *temp = frontdigit;
    frontdigit = frontdigit->flw;
    if (frontdigit != nullptr) 
        frontdigit->prev = nullptr;
    delete temp;
    length--;
    if (length == 0) 
        backdigit = nullptr;
}

void big::pop_back() {
    if (length == 0) 
        return;
    node *temp = backdigit;
    backdigit = backdigit->prev;
    if (backdigit) 
        backdigit->flw = nullptr;
    delete temp;
    length--;
    if (length == 0) 
        frontdigit = nullptr;
}

big& big::trim() {
    while (length > 1 && frontdigit->data == 0)
        pop_front();
    return *this;
}

std::ostream& operator<<(std::ostream &os, const big::decomp_t &t) {
    for (auto e: t)
        os << e.first << ' ' << e.second << '\n';
    return os;
}

big::decomp_t big::primefact() const {
    decomp_t decomp;
    big d = 2, curr(this->abs());
    while (d <= curr) {
        if (curr % d == 0) {
            int e = 0;
            do {
                curr /= d;
                e++;
            } while (curr % d == 0);
            decomp.push_back(std::make_pair(d, e));
        }
        d++;
    }
    return decomp;
}

big big::abs() const {
    big res(*this);
    res.negative = false;
    return res;
}

big big::gcd(const big &other) const {
    big a(this->abs()), b(other.abs()), c;
    while (b > 0) {
        c = b;
        b = a%b;
        a = c;
    }
    return a;
}