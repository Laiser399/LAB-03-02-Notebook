#pragma once
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>

#include "TempHashFunction.h"

//#include <iostream>

#define HT_DEFAULT_MULTI 1777
#define HT_DEFAULT_SIZE 32
#define HT_DEFAULT_MAX_DEPTH 5

#define ABS(a) ((a) < 0 ? -(a) : (a))
// TODO max depth




// ключ - значение
template<typename key_t, typename value_t>
class HashTable
{
private:
    std::vector<std::vector<std::pair<key_t, value_t> > > table; // хеш таблица
	int currDepth, maxDepth, hashMulti; // глубина, максимально допустима€ глубина, хэш множитель
	bool canRebuild;

	int hashFunc(key_t &key); // хэш функци€
	void rebuild();
public:
	HashTable();
	HashTable(int size, int maxDepth = HT_DEFAULT_MAX_DEPTH, 
		int hashMulti = HT_DEFAULT_MULTI);
	HashTable(HashTable<key_t, value_t> &another);
	HashTable<key_t, value_t> & operator =(HashTable<key_t, value_t> &another);

	bool insert(key_t key, value_t &value);
	std::pair<key_t, value_t> find(key_t key, bool &ok);
	bool changeValue(key_t key, value_t &newVal);

    bool erase(key_t key);
    void clear();

    std::vector<std::pair<key_t, value_t> > getAllPairs();
    std::vector<value_t> getAllValues();

	friend std::ostream & operator <<(std::ostream &out, HashTable<key_t, value_t> &ob);


	~HashTable() = default;
};


//-------------|
//   private   |
//-------------|
template<typename key_t, typename value_t>
int HashTable<key_t, value_t>::hashFunc(key_t &key) {
	return ABS(tempHashFunc<key_t>(key, hashMulti)) % table.size();
}


template<typename key_t, typename value_t>
void HashTable<key_t, value_t>::rebuild() {
	HashTable<key_t, value_t> newHashTable(this->table.size() * 2, this->maxDepth, this->hashMulti);
	newHashTable.canRebuild = false;

	for (auto it = table.begin(), it_last = table.end();
		it != it_last; ++it)
	{
		for (auto in_it = (*it).begin(), in_it_last = (*it).end();
			in_it != in_it_last; ++in_it)
		{
			newHashTable.insert((*in_it).first, (*in_it).second);
		}
	}

	newHashTable.canRebuild = true;
	*this = newHashTable;
}


//------------|
//   public   |
//------------|

template<typename key_t, typename value_t>
HashTable<key_t, value_t>::HashTable() {
	table.resize(HT_DEFAULT_SIZE);
	currDepth = 0;
	maxDepth = HT_DEFAULT_MAX_DEPTH;
	hashMulti = HT_DEFAULT_MULTI;
	canRebuild = true;
}

template<typename key_t, typename value_t>
HashTable<key_t, value_t>::HashTable(int size, int maxDepth, int hashMulti) {
	if (size <= 0)
		throw std::string("HashTable: constructor: received size <= 0!");
	if (maxDepth < 1)
		throw std::string("HashTable: constructor: received maxDepth < 1!");
	if (hashMulti == 0)
		throw std::string("HashTable: constructor: received hashMulti == 1!");

	table.resize(size);
	currDepth = 0;
	this->maxDepth = maxDepth;
	this->hashMulti = hashMulti;
	this->canRebuild = true;
}

template<typename key_t, typename value_t>
HashTable<key_t, value_t>::HashTable(HashTable<key_t, value_t> &another) {
	this->table.resize(another.table.size());
	this->currDepth = another.currDepth;
	this->maxDepth = another.maxDepth;
	this->hashMulti = another.hashMulti;
	this->canRebuild = another.canRebuild;

	// цикл по таблице
	for (auto it = table.begin(), an_it = another.table.begin(); 
		(it != table.end()) && (an_it != another.table.end()); ++it, ++an_it)
	{
		// цикл по массиву в €чейке таблицы
		for (auto inside_it = (*an_it).begin(); inside_it != (*an_it).end(); ++inside_it) {
			(*it).push_back(*inside_it);
		}
	}
}



template<typename key_t, typename value_t>
HashTable<key_t, value_t> & HashTable<key_t, value_t>::operator =(HashTable<key_t, value_t> &another) {
	this->table.resize(another.table.size());
	this->currDepth = another.currDepth;
	this->maxDepth = another.maxDepth;
	this->hashMulti = another.hashMulti;

	// цикл по таблице
	for (auto it = table.begin(), an_it = another.table.begin();
		(it != table.end()) && (an_it != another.table.end()); ++it, ++an_it)
	{
		// очистка
		(*it).resize(0);
		// цикл по массиву в €чейке таблицы
		for (auto inside_it = (*an_it).begin(); inside_it != (*an_it).end(); ++inside_it) {
			(*it).push_back(*inside_it);
		}
	}
	return *this;
}

template<typename key_t, typename value_t>
bool HashTable<key_t, value_t>::insert(key_t key, value_t &value) {
	int index = hashFunc(key);
	// по парам в подмассиве
	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if ((*it).first == key) {
			return false;
		}
	}

	table[index].push_back(std::pair<key_t, value_t>(key, value));

	if (table[index].size() > currDepth)
		currDepth = table[index].size();

    while (currDepth > maxDepth)
        rebuild();

	return true;
}


template<typename key_t, typename value_t>
std::pair<key_t, value_t> HashTable<key_t, value_t>::find(key_t key, bool &ok) {
	int index = hashFunc(key);

	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if ((*it).first == key) {
			ok = true;
			return *it;
		}
	}

	ok = false;
    return std::pair<key_t, value_t>(key_t(), value_t());
}

template<typename key_t, typename value_t>
bool HashTable<key_t, value_t>::changeValue(key_t key, value_t &newVal) {
	int index = hashFunc(key);

	for (auto it = table[index].begin(), it_last = table[index].end(); it != it_last; ++it) {
		if ((*it).first == key) {
			(*it).second = newVal;
			return true;
		}
	}
	return false;
}

template<typename key_t, typename value_t>
bool HashTable<key_t, value_t>::erase(key_t key) {
    int index = hashFunc(key);
    // итератор на пару (ключ - значение)
    for (auto it_p = table[index].begin(), it_p_l = table[index].end();
         it_p != it_p_l; ++it_p)
    {
        if (it_p->first == key) {
            table[index].erase(it_p);
            return true;
        }
    }
    return false;
}

template<typename key_t, typename value_t>
void HashTable<key_t, value_t>::clear() {
    for (auto it = table.begin(), it_l = table.end();
         it != it_l; ++it)
    {
        it->resize(0);
    }
    currDepth = 0;
}

template<typename key_t, typename value_t>
std::vector<std::pair<key_t, value_t> > HashTable<key_t, value_t>::getAllPairs() {
    std::vector<std::pair<key_t, value_t> > result;

    // итератор на эл-т хеш таблицы (массив пар)
    for (auto it = table.begin(), it_l = table.end();
         it != it_l; ++it)
    {
        // итератор на пару (ключ - значение)
        for (auto it_p = it->begin(), it_p_l = it->end();
             it_p != it_p_l; ++it_p)
        {
            result.push_back(*it_p);
        }
    }
    return result;
}

template<typename key_t, typename value_t>
std::vector<value_t> HashTable<key_t, value_t>::getAllValues() {
    std::vector<value_t> result;
    // итератор на массив пар (ключ - значение)
    for (auto it = table.begin(), it_l = table.end();
         it != it_l; ++it)
    {
        // итератор на пару (ключ - значение)
        for (auto it_p = it->begin(), it_p_l = it->end();
             it_p != it_p_l; ++it_p)
        {
            result.push_back(it_p->second);
        }
    }
    return result;
}

template<typename key_t, typename value_t>
std::ostream & operator <<(std::ostream &out, HashTable<key_t, value_t> &ob) {
	for (auto it = ob.table.begin(), it_last = ob.table.end();
		it != it_last; ++it)
	{
		int index = it - ob.table.begin();
		for (auto in_it = (*it).begin(), in_it_last = (*it).end();
			in_it != in_it_last; ++in_it)
		{
			out << "index: " << index << ", key: " << (*in_it).first << ", value: " <<
                (*in_it).second << '\n';
		}
	}
    out << "depth: " << ob.currDepth << '\n';

	return out;
}


#endif // !HASHTABLE_H
