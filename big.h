#ifndef BIG_H
#define BIG_H

#include <iostream>
#include <vector>

class big {
    private:
        bool negative;
        struct node;
        node *frontdigit, *backdigit;
        size_t length;
        big& trim();

        node* front() const;
        node* back() const;

        void push_front(short data);
        void push_back(short data);
        void pop_front();
        void pop_back();

    public:
        big();
        big(long long x);
        big(const big &from);
        ~big();

        friend std::istream& operator>>(std::istream &is, big &target);
        friend std::ostream& operator<<(std::ostream &os, const big &target);

        big& operator=(const big &from);
        bool operator<(const big &comp) const;
        bool operator==(const big &comp) const;
        bool operator!=(const big &comp) const;
        bool operator<=(const big &comp) const;
        bool operator>(const big &comp) const;
        bool operator>=(const big &comp) const;
        
        big operator+(const big &add) const;
        big& operator+=(const big &add);
        big& operator++();
        big operator++(int);
        big operator-() const;
        big operator-(const big &sub) const;
        big& operator-=(const big &add);
        big& operator--();
        big operator--(int);
        big operator*(const big &mult) const;
        big& operator*=(const big &add);
        big operator/(const big &div) const;
        big& operator/=(const big &add);
        big operator%(const big &mod) const;
        big& operator%=(const big &add);

        size_t size() const;

        big abs() const;
        big gcd(const big &other) const;
        
        using decomp_t = std::vector<std::pair<big, int>>;
        friend std::ostream& operator<<(std::ostream &os, const decomp_t &t);
        decomp_t primefact() const;
};

#endif