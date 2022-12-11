/*
 * hashmap.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ALGO_SEARCH_HASHMAP_H_
#define ALGO_SEARCH_HASHMAP_H_

#include "atto/core/core.hpp"
#include <list>

#define HASHMAP_T_DECL template<typename Key, typename Value, typename Hash>
#define HASHMAP_C_DECL HashMap<Key, Value, Hash>

namespace algo {

/** ---- HashMap class --------------------------------------------------------
 *
 * class HashMap
 *
 * HashMap represents a set of keys organised in an array of chains.
 * The index of the chain where the key is stored is computed using
 * a hash function.
 *
 * During lookup, the key is hashed and the resulting hash indicates
 * where the corresponding value is stored.
 *
 * Collision resolution is handled by building, for each of the array
 * indices, a linked list of the keys with the same hash code.
 *
 * The main idea is to choose the array size to be sufficiently large
 * that the lists are short to enable efficient search through a two
 * step process: hash the key to find the index of the list in the array
 * that could contain the key, then sequentially search through the list
 * for the key.
 *
 *  API HashMap<Key,Value,Hash>:
 *      HashMap(compare)           create an empty hash table
 *      ~HashMap()                 destroy the hash table and all keys
 *
 *  public:
 *      size_t size(void)          return the number of keys in the hash table.
 *      bool is_empty(void)         is the hash table empty?
 *      void clear(void)           remove all keys from the hash table.
 *
 *      void load(void)            return the average number of keys per chain.
 *
 *      void put(key, val)         insert a given key into the hash table.
 *      const Value get(key)       return the value associated with the key.
 *      bool contains(key)         does the hash table contain the given key?
 *      void erase(key)            remove the key from the hash table.
 *
 *      std::vector<hashmap_pair_type> keys(void)
 *                                  return a vector of all key-value pairs
 *
 *  private:
 *      iterator find(chain, key)  return an iterator to the key in the chain
 *      void resize(capacity)      resize the hash map to a new chain capacity
 *
 * @see
 * Algorithms 4th Edition by Robert Sedgewick, Kevin Wayne.
 */
template<
    typename Key,
    typename Value,
    typename Hash = std::hash<Key>>
class HashMap {
    /*
     * HashMap typedef declarations.
     *
     * A hash table is a vector of linked lists holding a set of keys.
     * The index of the list containing a key is given by the key hash.
     */
     typedef std::pair<Key,Value>               hashmap_pair_type;
     typedef std::list<hashmap_pair_type>       hashmap_chain_type;
     typedef std::vector<hashmap_chain_type>    hashmap_type;
    /*
     * HashMap chain capacity range as a power of two:
     *  min capacity = 2^3 = 8 chains
     *  max capacity = 2^32 = 4294967296 chains
     */
    static const size_t hashmap_min_bits_ = 3;
    static const size_t hashmap_max_bits_ = 31;
    static const size_t hashmap_min_chains_ = 1 << hashmap_min_bits_;
    static const size_t hashmap_max_chains_ = 1 << hashmap_max_bits_;
    /*
     * HashMap minimum and maximum chain loads.
     *
     * Chain load is defined as the chain average length.
     * Assuming the hash function to uniformly distribute the
     * keys among the available chains, the map load is the
     * average number of keys per chain:
     *
     *  load = size / chains.
     *
     * At key insertion, if 1) the number of chains is smaller than
     * its maximum capacity and 2) the average chain size is larger
     * than or equal to the map maximum load:
     *
     *  num_chains < hashmap_max_chains
     *  &&
     *  num_keys >= hashmap_max_load * num_chains,
     *
     * then resize the map chain capacity to twice its current
     * number of chains and rehash the keys into the new map.
     *
     * At key deletion, if 1) the number of chains is larger than
     * its minimum capacity and 2) the average chain size is smaller
     * than or equal to the map minimum load:
     *
     * num_chains > hashmap_min_chains
     * &&
     * num_keys <= hashmap_min_load_ * num_chains,
     *
     * then the map chain capacity resized to half its current
     * number of chains and again rehash the keys into the new map.
     */
    static const size_t hashmap_min_load_ = 1;
    static const size_t hashmap_max_load_ = 4;

public:
    /* --- HashMap public API member functions --------------------------------
     *
     * Return the number of keys in the hash table.
     */
    size_t size(void) const;
    /*
     * Is the hash table empty?
     */
    bool is_empty(void) const;
    /*
     * Remove all keys from the hash table.
     */
    void clear(void);
    /*
     * Return the average number of keys per unit chain.
     */
    double load(void) const;
    /*
     * Insert a given key-value pair into the hash table.
     * If the symbol table already contains the specified key,
     * overwrite the old value with the new associated value.
     */
    void put(const Key &key, const Value &val);
    /*
     * Return the value associated with the given key.
     */
    const Value &get(const Key &key);
    /*
     * Does the hash table contain the given key?
     */
    bool contains(const Key &key);
    /*
     * Remove the key and associated value from the hash table.
     */
    void erase(const Key &key);
    /*
     * Return an iterable vector of all key-value pairs
     * and associated values in the hash table.
     */
    std::vector<hashmap_pair_type> keys(void) const;

    /* --- HashMap debug API member functions ---------------------------------
     *
     * Print the load distribution of all chains in the table
     */
    void print_chainloads(void) const;

    /** ---- HashMap special member functions ----------------------------------
     *
     * HashMap constructor/destructor.
     */
    explicit HashMap(const Hash &hash = Hash())
        : hash_(hash)
        , hashmap_(hashmap_min_chains_)
        , hashmap_num_keys_(0) {}
    ~HashMap() { clear(); }
    /*
     * HashMap copy constructor/assignment.
     */
    HashMap(const HashMap &other)
        : hash_(other.hash_)
        , hashmap_(hashmap_min_chains_)
        , hashmap_num_keys_(0) {
        for (auto &item : other.keys()) {
            put(item.first, item.second);   /* copy the other keys */
        }
    }
    HashMap &operator=(const HashMap &other) {
        if (this == &other)
            return *this;
        hash_ = other.hash_;                /* copy the hash function */
        clear();                            /* clear and copy the other keys */
        for (auto &item : other.keys()) {
            put(item.first, item.second);
        }
        return *this;
    }
    /*
     * HashMap move constructor/assignment.
     */
    HashMap(HashMap &&other)
        : hash_(std::move(other.hash_))
        , hashmap_(std::move(other.hashmap_))
        , hashmap_num_keys_(std::move(other.hashmap_num_keys_)) {}
    HashMap &operator=(HashMap &&other) {
        if (this == &other)
            return *this;
        clear();                            /* clear and move the other keys */
        hash_ = std::move(other.hash_);
        hashmap_ = std::move(other.hashmap_);
        hashmap_num_keys_ = std::move(other.hashmap_num_keys_);
        return *this;
    }

private:
    /** ---- HashMap private member functions ----------------------------------
     *
     * Return an iterator to the item in the hashmap containing the key.
     */
    typename hashmap_chain_type::iterator find(
        hashmap_chain_type &chain,
        const Key &key);
    /*
     * Resize the hash map to have the given number of chains,
     * and rehashing all the keys in the process.
     */
    void resize(size_t capacity);

    /** ---- HashMap private member variables ----------------------------------
     */
    Hash hash_;                         /* hash function */
    hashmap_type hashmap_;              /* hash table */
    size_t hashmap_num_keys_;           /* number of keys in the hash table */
}; /* class HashMap */

/** --- HashMap public API member functions -----------------------------------
 *
 * HashMap<Key,Value,Hash>::size
 *
 * Return the number of keys in the hash table.
 */
HASHMAP_T_DECL
inline
size_t HASHMAP_C_DECL::size(void) const
{
    return hashmap_num_keys_;
}

/**
 * HashMap<Key,Value,Hash>::is_empty
 *
 * Is the hash table empty?
 */
HASHMAP_T_DECL
inline
bool HASHMAP_C_DECL::is_empty(void) const
{
    return (size() == 0);
}

/**
 * HashMap<Key,Value,Hash>::clear
 *
 * Remove all keys from the hash table.
 */
HASHMAP_T_DECL
inline
void HASHMAP_C_DECL::clear(void)
{
    hashmap_.clear();
    hashmap_num_keys_ = 0;
}

/**
 * HashMap<Key,Value,Hash>::load
 *
 * Return the average number of keys per unit chain.
 */
HASHMAP_T_DECL
inline
double HASHMAP_C_DECL::load(void) const
{
    return static_cast<double>(size()) / static_cast<double>(hashmap_.size());
}

/**
 * HashMap<Key,Value,Hash>::put
 *
 * Insert a given key-value pair into the hash table.
 * If the symbol table already contains the specified key,
 * overwrite the old value with the new associated value.
 */
HASHMAP_T_DECL
inline
void HASHMAP_C_DECL::put(const Key &key, const Value &val)
{
    /*
     * Double the table size if its average chain length >= hashmap_max_load.
     */
    if (hashmap_.size() < hashmap_max_chains_ &&
        hashmap_num_keys_ >= (hashmap_max_load_*hashmap_.size())) {
        resize(2*hashmap_.size());
    }

    /*
     * Get the hash index for this key and find the entry in
     * the corresponding chain.
     *
     * If the entry doesn't exist, create a new one.
     * Otherwise, replace the value associated with the key.
     */
    size_t ix = hash_(key) % hashmap_.size();
    auto it = find(hashmap_[ix], key);
    if (it == hashmap_[ix].end()) {
        hashmap_[ix].emplace_back(key, val);    /* insert a new key entry */
        hashmap_num_keys_++;                    /* update key counter */
    } else {
        it->second = val;                       /* replace the value */
    }
}

/**
 * HashMap<Key,Value,Hash>::get
 *
 * Return the value associated with the given key.
 */
HASHMAP_T_DECL
inline
const Value &HASHMAP_C_DECL::get(const Key &key)
{
    size_t ix = hash_(key) % hashmap_.size();
    auto it = find(hashmap_[ix], key);
    core_assert(it != hashmap_[ix].end(), "nonexistent key");
    return it->second;
}

/**
 * HashMap<Key,Value,Hash>::contains
 *
 * Does the hash table contain the given key?
 */
HASHMAP_T_DECL
inline
bool HASHMAP_C_DECL::contains(const Key &key)
{
    size_t ix = hash_(key) % hashmap_.size();
    return (find(hashmap_[ix], key) != hashmap_[ix].cend());
}

/**
 * HashMap<Key,Value,Hash>::erase
 *
 * Remove the key and associated value from the hash table.
 */
HASHMAP_T_DECL
inline
void HASHMAP_C_DECL::erase(const Key &key)
{
    core_assert(!is_empty(), "symbol table underflow");

    /*
     * Get the hash index for this key and find the entry
     * in the corresponding chain.
     *
     * If the entry doesn't exist, do nothing. Otherwise,
     * remove the entry from the map.
     */
    size_t ix = hash_(key) % hashmap_.size();
    auto it = find(hashmap_[ix], key);
    if (it != hashmap_[ix].end()) {
        it = hashmap_[ix].erase(it);        /* remove the key entry */
        hashmap_num_keys_--;                /* update key counter */
    }

    /*
     * Halve the table size if its average chain length <= hashmap_min_load.
     */
    if (hashmap_.size() > hashmap_min_chains_ &&
        hashmap_num_keys_ <= (hashmap_min_load_*hashmap_.size())) {
        resize(hashmap_.size()/2);
    }
}

/**
 * HashMap<Key,Value,Hash>::keys
 *
 * Return an iterable vector of all key-value pairs in the hash table.
 */
HASHMAP_T_DECL
inline
std::vector<typename HASHMAP_C_DECL::hashmap_pair_type>
HASHMAP_C_DECL::keys(void) const
{
    std::vector<hashmap_pair_type> items;
    for (auto &chain : hashmap_) {
        for (auto &item : chain) {
            items.emplace_back(item);
        }
    }
    return items;
}


/** --- HashMap debug API member functions ------------------------------------
 *
 * Print the load distribution of all chains in the table
 */
HASHMAP_T_DECL
inline
void HASHMAP_C_DECL::print_chainloads(void) const
{
    for (size_t i = 0; i < hashmap_.size(); ++i) {
        std::cout << hashmap_[i].size() << " ";
    }
    std::cout << "\n";
}

/** ---- HashMap private member functions -------------------------------------
 *
 * HashMap<Key,Value,Hash>::find
 *
 * Return an iterator to the item in the hashmap containing the key.
 */
HASHMAP_T_DECL
inline
typename HASHMAP_C_DECL::hashmap_chain_type::iterator HASHMAP_C_DECL::find(
    hashmap_chain_type &chain,
    const Key &key)
{
#if 1
    typename hashmap_chain_type::iterator it = chain.begin();
    while (it != chain.end()) {
        if (it->first == key) {
            break;
        }
        ++it;
    }
    return it;
#else
    return std::find_if(
            chain.begin(),
            chain.end(),
            [&] (const std::pair<Key,Value> &item) -> bool
                { return item.first == key; });
#endif
}

/**
 * HashMap<Key,Value,Hash>::resize
 *
 * Resize the hash map to have the given number of chains,
 * and rehashing all the keys in the process.
 */
HASHMAP_T_DECL
inline
void HASHMAP_C_DECL::resize(size_t capacity)
{
    /*
     * Create a copy of the hashmap with the new capacity.
     */
    hashmap_type copy_hashmap(capacity);
    for (auto &chain : hashmap_) {
        for (auto &item : chain) {
            size_t ix = hash_(item.first) % copy_hashmap.size();
            copy_hashmap[ix].emplace_back(item.first, item.second);
        }
    }
    hashmap_.swap(copy_hashmap);
}

} /* namespace algo */

#undef HASHMAP_T_DECL
#undef HASHMAP_C_DECL

#endif /* ALGO_SEARCH_HASHMAP_H_ */
