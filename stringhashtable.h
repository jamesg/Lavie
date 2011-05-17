#ifndef STRINGHASHTABLE_H
#define STRINGHASHTABLE_H
#include <vector>
#include <string>

#define TABLE_SIZE 256

template<class T>
class stringhashtable {
	private:
		vector< pair<string, T> > buckets[TABLE_SIZE];
		int size;

		int hash(string s) {
			int h = 0;
			string::iterator i;
			for (i = s.begin(); i != s.end(); i++) {
				h += *i;
				h += (h << 10);
				h ^= (h << 6);
			}
			h += (h << 3);
			h ^= (h << 11);
			h += (h << 15);
			return h;
		}
	public:
		bool insert(string key, T value) {
			if (!contains(key)) {
				buckets[hash(key)&(TABLE_SIZE-1)].push_back(pair<string, T>(key,value));
				return true;
			} else {
				return false;
			}
		}

		bool remove(string key) {
			int h = hash(key)&(TABLE_SIZE-1);
			typename vector< pair<string, T> >::iterator i;
			for (i = buckets[h].begin(); i != buckets[h].end(); i++) {
				if (i->first == key) {
					buckets[h].erase(i);
					return true;
				}
			}
			return false;
		}

		bool contains(string key) {
			typename vector< pair<string, T> >::iterator i;
			int h = hash(key)&(TABLE_SIZE-1);
			for (i = buckets[h].begin(); i != buckets[h].end(); i++) {
				if (i->first == key) {
					return true;
				}
			}
			return false;
		}

		T get(string key) {
			int h = hash(key)&(TABLE_SIZE-1);
			typename vector< pair<string, T> >::iterator i;
			for (i = buckets[h].begin(); i != buckets[h].end(); i++) {
				if (i->first == key) return i->second;
			}
			return T();
		}
};

#endif
