
#ifndef HASH_MAP_H
#define HASH_MAP_


#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <iterator>
#include <stdexcept>

template <typename K, typename V>
struct HashMap {
    std::vector<std::list<std::pair<K, V>>> table;
    std::hash<K> hash;

    HashMap(size_t capacity = 100) : table(capacity) {}

    void put(const K& key, const V& value) {
        size_t hashValue = hash(key) % table.size();
        for (auto& kv : table[hashValue]) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        table[hashValue].emplace_back(key, value);
    }

    bool get(const K& key, V& value) const {
        size_t hashValue = hash(key) % table.size();
        for (const auto& kv : table[hashValue]) {
            if (kv.first == key) {
                value = kv.second;
                return true;
            }
        }
        return false;
    }

    bool remove(const K& key) {
        size_t hashValue = hash(key) % table.size();
        auto& cell = table[hashValue];
        for (auto it = cell.begin(); it != cell.end(); ++it) {
            if (it->first == key) {
                cell.erase(it);
                return true;
            }
        }
        return false;
    }

    bool containsKey(const K& key) const {
        size_t hashValue = hash(key) % table.size();
        for (const auto& kv : table[hashValue]) {
            if (kv.first == key) {
                return true;
            }
        }
        return false;
    }

    void push_back(const K& key, const V& value) {
        put(key, value);
    }

    void append(const HashMap& other) {
        for (const auto& cell : other.table) {
            for (const auto& kv : cell) {
                put(kv.first, kv.second);
            }
        }
    }

    size_t size() const {
        size_t count = 0;
        for (const auto& cell : table) {
            count += cell.size();
        }
        return count;
    }

    bool empty() const {
        return size() == 0;
    }

    void clear() {
        for (auto& cell : table) {
            cell.clear();
        }
    }

    // Copy assignment operator
    HashMap& operator=(const HashMap& other) {
        if (this == &other) return *this;  // Handle self-assignment

        table = other.table;
        hash = other.hash;
        return *this;
    }

    // Equality operator
    bool operator==(const HashMap& other) const {
        if (table.size() != other.table.size()) return false;

        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i].size() != other.table[i].size()) return false;

            auto it1 = table[i].begin();
            auto it2 = other.table[i].begin();
            while (it1 != table[i].end()) {
                if (*it1 != *it2) return false;
                ++it1;
                ++it2;
            }
        }
        return true;
    }

    bool operator!=(const HashMap& other) const {
        return !(*this == other);
    }

    // Subscript operator
    V& operator[](const K& key) {
        size_t hashValue = hash(key) % table.size();
        for (auto& kv : table[hashValue]) {
            if (kv.first == key) {
                return kv.second;
            }
        }
        // If key doesn't exist, insert a new element with default value
        table[hashValue].emplace_back(key, V());
        return table[hashValue].back().second;
    }

    // Const subscript operator
    const V& operator[](const K& key) const {
        size_t hashValue = hash(key) % table.size();
        for (const auto& kv : table[hashValue]) {
            if (kv.first == key) {
                return kv.second;
            }
        }
        throw std::out_of_range("Key not found");
    }

    // Iterator for HashMap
    struct iterator {
        using outer_iterator = typename std::vector<std::list<std::pair<K, V>>>::iterator;
        using inner_iterator = typename std::list<std::pair<K, V>>::iterator;

        outer_iterator outer;
        outer_iterator outer_end;
        inner_iterator inner;

        iterator(outer_iterator outer, outer_iterator outer_end, inner_iterator inner)
            : outer(outer), outer_end(outer_end), inner(inner) {}

        std::pair<K, V>& operator*() {
            return *inner;
        }

        iterator& operator++() {
            ++inner;
            if (inner == outer->end()) {
                do {
                    ++outer;
                } while (outer != outer_end && outer->empty());
                if (outer != outer_end) {
                    inner = outer->begin();
                }
            }
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return outer != other.outer || (outer == other.outer && inner != other.inner);
        }
    };

    iterator begin() {
        for (auto it = table.begin(); it != table.end(); ++it) {
            if (!it->empty()) {
                return iterator(it, table.end(), it->begin());
            }
        }
        return end();
    }

    iterator end() {
        return iterator(table.end(), table.end(), {});
    }

    // Const iterator for HashMap
    struct const_iterator {
        using outer_iterator = typename std::vector<std::list<std::pair<K, V>>>::const_iterator;
        using inner_iterator = typename std::list<std::pair<K, V>>::const_iterator;

        outer_iterator outer;
        outer_iterator outer_end;
        inner_iterator inner;

        const_iterator(outer_iterator outer, outer_iterator outer_end, inner_iterator inner)
            : outer(outer), outer_end(outer_end), inner(inner) {}

        const std::pair<K, V>& operator*() const {
            return *inner;
        }

        const_iterator& operator++() {
            ++inner;
            if (inner == outer->end()) {
                do {
                    ++outer;
                } while (outer != outer_end && outer->empty());
                if (outer != outer_end) {
                    inner = outer->begin();
                }
            }
            return *this;
        }

        bool operator!=(const const_iterator& other) const {
            return outer != other.outer || (outer == other.outer && inner != other.inner);
        }
    };

    const_iterator begin() const {
        for (auto it = table.begin(); it != table.end(); ++it) {
            if (!it->empty()) {
                return const_iterator(it, table.end(), it->begin());
            }
        }
        return end();
    }

    const_iterator end() const {
        return const_iterator(table.end(), table.end(), {});
    }

    // Find method
    iterator find(const K& key) {
        size_t hashValue = hash(key) % table.size();
        for (auto it = table[hashValue].begin(); it != table[hashValue].end(); ++it) {
            if (it->first == key) {
                return iterator(table.begin() + hashValue, table.end(), it);
            }
        }
        return end();
    }

    const_iterator find(const K& key) const {
        size_t hashValue = hash(key) % table.size();
        for (auto it = table[hashValue].begin(); it != table[hashValue].end(); ++it) {
            if (it->first == key) {
                return const_iterator(table.begin() + hashValue, table.end(), it);
            }
        }
        return end();
    }

    // Insert method
    void insert(const std::pair<K, V>& kv) {
        put(kv.first, kv.second);
    }

    // Erase method
    void erase(iterator it) {
        if (it.outer == table.end()) return;
        size_t hashValue = hash(it->first) % table.size();
        table[hashValue].erase(it.inner);
    }

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const HashMap& map) {
        for (const auto& cell : map.table) {
            for (const auto& kv : cell) {
                os << "{" << kv.first << ": " << kv.second << "} ";
            }
        }
        return os;
    }
};


#endif // HASH_MAP_H
