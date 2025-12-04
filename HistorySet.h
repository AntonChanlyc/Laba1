#pragma once
#include <string>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <stack>
#include <memory>
#include <sstream>

enum BucketState { BU_EMPTY, BU_FILLED, BU_DELETED };

template<typename Key, typename Value>
class HistorySet {
private:
    struct Bucket {
        Key key;
        Value value;
        BucketState state = BU_EMPTY;
    };

    Bucket* table_;
    size_t capacity_;
    size_t size_;
    static constexpr double LOAD_FACTOR = 0.6;
    static constexpr size_t INITIAL_CAP = 16;

    size_t hashKey(const Key& key) const {
        return std::hash<Key>()(key) % capacity_;
    }

    void rehash() {
        size_t oldCap = capacity_;
        Bucket* oldTable = table_;
        capacity_ *= 2;
        table_ = new Bucket[capacity_];
        size_t oldSize = size_;
        size_ = 0;
        for (size_t i = 0; i < oldCap; ++i) {
            if (oldTable[i].state == BU_FILLED) {
                add(oldTable[i].key, oldTable[i].value);
            }
        }
        delete[] oldTable;
    }

public:
    HistorySet() : capacity_(INITIAL_CAP), size_(0) {
        table_ = new Bucket[capacity_];
    }

    HistorySet(const HistorySet& other)
        : capacity_(other.capacity_), size_(other.size_) {
        table_ = new Bucket[capacity_];
        for (size_t i = 0; i < capacity_; ++i) table_[i] = other.table_[i];
    }

    HistorySet& operator=(const HistorySet& other) {
        if (this == &other) return *this;
        delete[] table_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        table_ = new Bucket[capacity_];
        for (size_t i = 0; i < capacity_; ++i) table_[i] = other.table_[i];
        return *this;
    }

    ~HistorySet() {
        delete[] table_;
    }

    
    void add(const Key& key, const Value& value) {
        if ((double)size_ / capacity_ > LOAD_FACTOR) rehash();
        size_t idx = hashKey(key);
        size_t start = idx;
        while (table_[idx].state == BU_FILLED) {
            if (table_[idx].key == key) throw std::runtime_error("Duplicate key");
            idx = (idx + 1) % capacity_;
            if (idx == start) throw std::runtime_error("Hash table full");
        }
        table_[idx].key = key;
        table_[idx].value = value;
        table_[idx].state = BU_FILLED;
        ++size_;
    }

    HistorySet& operator<<(const std::pair<Key, Value>& kv) {
        add(kv.first, kv.second);
        return *this;
    }

    bool remove(const Key& key) {
        size_t idx = hashKey(key);
        size_t start = idx;
        while (table_[idx].state != BU_EMPTY) {
            if (table_[idx].state == BU_FILLED && table_[idx].key == key) {
                table_[idx].state = BU_DELETED;
                --size_;
                return true;
            }
            idx = (idx + 1) % capacity_;
            if (idx == start) break;
        }
        return false;
    }

    void clear() {
        delete[] table_;
        capacity_ = INITIAL_CAP;
        table_ = new Bucket[capacity_];
        size_ = 0;
    }

    size_t size() const { return size_; }

    bool contains(const Key& key) const {
        size_t idx = hashKey(key);
        size_t start = idx;
        while (table_[idx].state != BU_EMPTY) {
            if (table_[idx].state == BU_FILLED && table_[idx].key == key) return true;
            idx = (idx + 1) % capacity_;
            if (idx == start) break;
        }
        return false;
    }

    Value& operator[](const Key& key) {
        size_t idx = hashKey(key);
        size_t start = idx;
        while (table_[idx].state != BU_EMPTY) {
            if (table_[idx].state == BU_FILLED && table_[idx].key == key) return table_[idx].value;
            idx = (idx + 1) % capacity_;
            if (idx == start) break;
        }
        throw std::runtime_error("Key not found");
    }

    bool operator==(const HistorySet& other) const {
        if (size_ != other.size_) return false;
        for (size_t i = 0; i < capacity_; ++i) {
            if (table_[i].state == BU_FILLED) {
                if (!other.contains(table_[i].key)) return false;
            }
        }
        return true;
    }

    HistorySet operator&&(const HistorySet& other) const {
        HistorySet res;
        for (size_t i = 0; i < capacity_; ++i) {
            if (table_[i].state == BU_FILLED) {
                if (other.contains(table_[i].key)) res.add(table_[i].key, table_[i].value);
            }
        }
        return res;
    }

    std::vector<Key> keys() const {
        std::vector<Key> out;
        out.reserve(size_);
        for (size_t i = 0; i < capacity_; ++i) {
            if (table_[i].state == BU_FILLED) out.push_back(table_[i].key);
        }
        return out;
    }

    
    void save(const std::string& path) const {
        std::ofstream out(path);
        if (!out) throw std::runtime_error("Cannot open file for save");
        for (size_t i = 0; i < capacity_; ++i) {
            if (table_[i].state == BU_FILLED) {
                out << table_[i].key << "\n";
                
                Value copy = table_[i].value;
                out << copy.size() << "\n";
                
                while (!copy.empty()) {
                    std::ostringstream ss;
                    
                    ss << *copy.top();
                    out << ss.str() << "\n";
                    copy.pop();
                }
            }
        }
        out.close();
    }

    
    void load(const std::string& path) {
        clear();
        std::ifstream in(path);
        if (!in) throw std::runtime_error("Cannot open file for load");
        std::string key;
        while (std::getline(in, key)) {
            if (key.empty()) continue;
            std::string nline;
            if (!std::getline(in, nline)) break;
            size_t n = 0;
            try { n = std::stoul(nline); } catch(...) { n = 0; }
            
            for (size_t i = 0; i < n; ++i) {
                std::string dummy;
                std::getline(in, dummy);
            }
            
            Value v;
            add(key, v);
        }
        in.close();
    }
};
