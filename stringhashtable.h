#include <vector>

#define COMMAND_TABLE_SIZE 256

template<class T>
class stringhashtable {
	private:
		vector< pair<string, T> > buckets[COMMAND_TABLE_SIZE];

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
		bool add(string key, T value) {
			if (!contains(key)) {
				buckets[hash(key)&(COMMAND_TABLE_SIZE-1)].push_back(pair<string, T>(key,value));
				return true;
			} else {
				return false;
			}
		}

		bool remove(string key) {
			int h = hash(key)&(COMMAND_TABLE_SIZE-1);
			typename vector< pair<string, T> >::iterator i;
			for (i = buckets[h].begin(); i != buckets[h].end(); i++) {
				if (i->first == key) {
					buckets.remove(i);
					return true;
				}
			}
			return false;
		}

		bool contains(string key) {
			int h = hash(key)&(COMMAND_TABLE_SIZE-1);
			typename vector< pair<string, T> >::iterator i;
			for (i = buckets[h].begin(); i != buckets[h].end(); i++) {
				if (i->first == key) return true;
			}
			return false;
		}

		T get(string key) {
			int h = hash(key)&(COMMAND_TABLE_SIZE-1);
			typename vector< pair<string, T> >::iterator i;
			for (i = buckets[h].begin(); i != buckets[h].end(); i++) {
				if (i->first == key) return i->second;
			}
			return T();
		}
};

