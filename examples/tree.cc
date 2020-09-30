#include <bits/stdint-intn.h>
#include <iostream>

#include "libalgo/interval_tree.hh"

int main() {
    auto x = libalgo::IntervalTree<int64_t, int>(0, (1ll<<60));
    int64_t big_num = 1ll<<59;
    x.add(10, 10, big_num);
    auto t1 = x.query(2, 10);
    auto t2 = x.query(2, 15);
    auto t3 = x.query(big_num - 1, big_num * 2);
    auto t4 = x.query(big_num, big_num * 2);
    std::cout << t1 << " " << t2 << std::endl;
    std::cout << t3 << " " << t4 << std::endl;
    std::cout << "SIZE: " << x.size() << std::endl;
    return 0;
}