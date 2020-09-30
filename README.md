# libalgo

Currently:
  - IntervalTree - works fast even on huge ranges like [1, sizeof(long long)))
  - Set - implemented on a splay tree. Because of that it has "caching" built-in to the architecture. Items that are accessed frequently can be accessed really fast. It supports an additional operation shift(value >= 0, x). It adds value to all elements in the set greater or equal to x.
  - SuffixTree - Implementation of Ukkonens algorithm for linear, online construction of suffix trees.

