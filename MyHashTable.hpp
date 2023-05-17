#ifndef MY_HASH_TABLE_HPP
#define MY_HASH_TABLE_HPP

/**
 * TODO: Complete this class!
 */

/// Your welcome
#include <cmath>
#include <forward_list>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

///	Begin our class namespace
namespace CPSC131::MyHashTable
{
  //
  template <class VTYPE>
  class MyHashTable
  {
    //
  public:
    //
    static constexpr size_t DEFAULT_CAPACITY = 1024;
    static constexpr size_t ULLONG_WRAP_AT = 4294967295;

    /**
     * Constructor
     *
     * Probably easiest to do the following:
     *
     *   1. Call on setCapacity() to set the table's capacity
     */
    MyHashTable(size_t capacity = MyHashTable::DEFAULT_CAPACITY)
    {
      setCapacity(capacity);
    }

    /**
     * Copy constructor
     */
    MyHashTable(const MyHashTable &other)
    {
      capacity_ = other.capacity_;
      size_ = other.size_;
      n_collisions_ = other.n_collisions_;
      table_ = new std::forward_list<std::pair<std::string, VTYPE>>[capacity_];

      for (size_t i = 0; i < capacity_; ++i)
      {
        table_[i] = other.table_[i];
      }
    }

    /**
     * Destructor
     *
     * Deallocate the table (array of std::forward_list 's), and set the table to
     * nullptr Don't double-deallocate if the table is already a null pointer
     */
    ~MyHashTable()
    {
      if (table_ != nullptr)
      {
        delete[] table_;
      }
    }

    /**
     * Should return the capacity_ of this hash table
     * Meaning, the total number of rows in this hash table
     * Doesn't discern between used/unused/collided rows
     */
    size_t capacity() const { return capacity_; }

    /**
     * Should return the number of items currently in the hash table
     */
    size_t size() const { return size_; }

    /**
     * Return true if the table is empty, false otherwise
     */
    bool empty() { return size_ == 0; }

    /**
     * Should return the number of collisions currently present in the hash table
     */
    size_t n_collisions() const { return n_collisions_; }

    /**
     * Set the capacity for this table.
     * Changes the total number of hash table rows available.
     *
     * Existing entries should survive the change,
     * 	so you probably want to do the following:
     *
     *  1. Keep the existing table array pointer in a temporary pointer
     *
     *  2. Re-initialize 'this' to contain a new empty table of the right capacity
     *
     *  3. The temporary pointer should now contain the OLD data (old table),
     *     while 'this' contains the brand new table of the new size.
     *
     *  4. Iterate through all key/value pairs in the old data (temp pointer).
     *
     *  5. For each key/value pair in the old data, call this->add() to insert
     *     properly into the new table.
     *
     *  6. Deallocate the old table by calling `delete []` on the temporary
     * pointer.
     *
     * Don't forget to check whether the current table is a nullptr before you
     *  decide to do iterations!
     */
    void setCapacity(size_t c)
    {
      if (c > MyHashTable::ULLONG_WRAP_AT)
      {
        throw std::invalid_argument(
            "capacity must be less than or equal to 4294967295");
      }
      auto old_table = table_;
      size_t old_capacity = capacity_;
      // size_t old_size = size_;

      capacity_ = c;
      size_ = 0;
      n_collisions_ = 0;
      table_ = new std::forward_list<std::pair<std::string, VTYPE>>[c];

      for (size_t i = 0; i < old_capacity; ++i)
      {
        for (const auto &kv_pair : old_table[i])
        {
          this->add(kv_pair.first, kv_pair.second);
        }
      }

      if (old_table != nullptr)
      {
        delete[] old_table;
      }
    }

    ///	Your welcome
    unsigned long long int hash(std::string key) const
    {
      return this->midSquareHash(key);
      return this->myCustomHashFunction1(key);
      return this->myCustomHashFunction2(key);
      return this->myCustomHashFunction3(key);
      return this->myCustomHashFunction4(key);
    }

    /**
     * Implement the "modified mid square" hash function,
     * as described in the instructions.
     */
    unsigned int midSquareHash(const std::string &key) const
    {
      unsigned long long int sum = 1;

      for (size_t i = 0; i < key.size(); ++i)
      {
        sum = (sum * (unsigned long long int)key[i]) % ULLONG_WRAP_AT;
      }

      unsigned long long int squared_sum = sum * sum;

      std::string squared_sum_str = std::to_string(squared_sum);

      size_t target_length = squared_sum_str.size() / 2;
      size_t start_index = std::floor(target_length / 2);

      std::string middle_digits =
          squared_sum_str.substr(start_index, target_length);

      unsigned long long int hashCode = std::stoull(middle_digits);

      return hashCode % capacity_;
    }

    /**
     * Implement your own custom hash functions to compete on the Gradescope
     * Leaderboard! Four stubs are provided to make experimentation a little
     * easier. Only the best hash function will count towards the leaderboard.
     */

    static constexpr unsigned long long int fnv_prime = 1099511628211ull;
    static constexpr unsigned long long int fnv_offset_basis = 14695981039346656037ull;

    unsigned long long int myCustomHashFunction1(const std::string &key) const
    {
      unsigned long long int hash = 0;
      size_t index = 0;
      for (char c : key)
      {
        hash = (hash * 41) ^ (c + (c << (index % 4)) + (c >> (index % 3)));
        index++;
      }
      return hash % capacity_;
    }

    unsigned long long int myCustomHashFunction2(const std::string &key) const
    {
      unsigned long long int hash = 0;
      for (size_t i = 0; i < key.size(); ++i)
      {
        hash = (hash * 89) + (key[i] * (key.size() - i));
      }
      return hash % capacity_;
    }

    unsigned long long int myCustomHashFunction3(const std::string &key) const
    {
      unsigned long long int hash = 0;
      size_t index = 0;
      for (char c : key)
      {
        hash = (hash * 113) ^ (c * (1 << (index % 7)));
        index++;
      }
      return hash % capacity_;
    }

    unsigned long long int myCustomHashFunction4(std::string key) const
    {
      unsigned long long int hash = fnv_offset_basis;
      size_t index = 1;
      for (char c : key)
      {
        hash = (hash ^ (c * (6 * index * index + 1))) * fnv_prime;
        index++;
      }
      return hash % capacity_;
    }

    /**
     * Return true if a key exists in the table, or false otherwise.
     */
    bool exists(std::string key) const
    {
      size_t index = hash(key) % capacity_;
      for (const auto &kv_pair : table_[index])
      {
        if (kv_pair.first == key)
        {
          return true;
        }
      }
      return false;
    }

    /**
     * Add a key/value pair to this table.
     * If the key already exists, throw a runtime_error.
     */
    void add(std::string key, VTYPE value)
    {
      size_t index = hash(key) % capacity_;
      auto &bucket = table_[index];

      for (const auto &kv_pair : bucket)
      {
        if (kv_pair.first == key)
        {
          throw std::runtime_error("Key already exists");
        }
      }

      if (!bucket.empty())
      {
        ++n_collisions_;
      }

      bucket.emplace_front(key, value);
      ++size_;
    }

    /**
     * Should return a reference to the VTYPE value associated to the provided
     * key. If the key/value pair isn't in the table, a runtime_error should be
     * thrown.
     */
    VTYPE &get(std::string key) const
    {
      size_t index = hash(key) % capacity_;
      auto &bucket = table_[index];

      for (auto &kv_pair : bucket)
      {
        if (kv_pair.first == key)
        {
          return kv_pair.second;
        }
      }
      throw std::runtime_error(
          "Cannot get value for key because it doesn't exist: " + key);
    }

    /**
     * In a single std::forward_list, return every key currently present in the
     * table
     *
     * If no keys exist (empty table), just return an empty std::forward_list.
     *
     * It's okay that when sorted == false, std::forward_list::push_front will
     *  return your keys in backwards-order, as there is no guarantee
     *  of properly-ordered keys anyway.
     *
     * However, if the caller passes sorted=true, you should sort the keys
     * 	as a last step before returning them.
     * 	Try using the built-in method std::forward_list::sort
     */
    std::forward_list<std::string> getAllKeys(bool sorted = false) const
    {
      std::forward_list<std::string> keys;
      for (size_t i = 0; i < capacity_; ++i)
      {
        for (const auto &kv_pair : table_[i])
        {
          keys.push_front(kv_pair.first);
        }
      }

      if (sorted)
      {
        keys.sort();
      }

      return keys;
    }

    /**
     * Remove a key/value pair that corresponds to the provided key.
     * If no such key exists, throw a runtime_error.
     */
    void remove(std::string key)
    {
      size_t index = hash(key) % capacity_;
      auto &bucket = table_[index];

      auto prev_it = bucket.before_begin();
      auto it = bucket.begin();

      while (it != bucket.end())
      {
        if (it->first == key)
        {
          bucket.erase_after(prev_it);
          --size_;
          if (!bucket.empty())
          {
            --n_collisions_;
          }
          return;
        }
        prev_it = it;
        ++it;
      }
      throw std::runtime_error(
          "Cannot remove value for key because it doesn't exist: " + key);
    }

    /**
     * Remove all entries in this table
     * Iterate over each table row and call clear on the row's list
     */
    void clear()
    {
      for (size_t i = 0; i < capacity_; ++i)
      {
        table_[i].clear();
      }
      size_ = 0;
      n_collisions_ = 0;
    }

    /**
     * Assignment Operator
     */
    MyHashTable<VTYPE> &operator=(const MyHashTable<VTYPE> &other)
    {
      capacity_ = other.capacity_;
      table_ = other.table_;
      size_ = other.size_;
      n_collisions_ = other.n_collisions_;
      return *this;
    }

    //
  private:
    /**
     * Keeps track of the capacity of this hash table
     * (total number of rows in the table, whether used or not)
     * (does not care about collisions)
     */
    size_t capacity_ = 0;

    /**
     * Keeps track of the total number of items currently in
     * the hash table
     */
    size_t size_ = 0;

    /**
     * Keeps track of the number of collisions currently present in the table
     */
    size_t n_collisions_ = 0;

    /**
     * The actual hash table.
     * We'll make this a fixed-sized array,
     *   but dynamically allocated so we can resize whenever we need.
     *
     * Try to imagine each table row is actually a complete forward_list.
     * In a perfect situation where a table has no collisions, each list
     *   will simply contain one item.
     *
     * One "item" in this case refers to an std::pair<std::string, VTYPE>,
     *   where the first part of the pair is the key and the second
     *   is the value (of type VTYPE).
     */
    std::forward_list<std::pair<std::string, VTYPE>> *table_ = nullptr;
  };
} // namespace CPSC131::MyHashTable

#endif