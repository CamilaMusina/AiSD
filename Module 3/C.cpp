#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <string>

int M = pow(2, 31) - 1;

class BitArray {
private:
	std::vector<char> bitarr;
	int size;
public:
	BitArray() { }
	BitArray(int size) {
		this->size = size;
		(size % 8 == 0) ? bitarr.resize(size / 8) : bitarr.resize(size / 8 + 1);
		for (auto bit : bitarr)
			bit = 0;
	}

	void bit_set(int x, int y) {
		if (!(bitarr[x] & 1 << y))
			bitarr[x] ^= (1 << y);
	}

	bool bit_find(int x, int y) {
		if (!(bitarr[x] & 1 << y)) 
			return false;
		return true;
	}

	void print() {
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(NULL);

		int bitsize = (size % 8 == 0) ? size / 8 - 1 : size / 8;
		for (int i = 0; i < bitsize; i++)
			for (int j = 0; j < 8; j++)
				(bitarr[i] & 1 << j) ? std::cout << 1 : std::cout << 0;

		char last = bitarr[bitsize];
		int count = size % 8;
		if (count == 0) count = 8;
		int q = last & 1 << 2;
		for (int i = 0; i < count; i++)
			(last & 1 << i) ? std::cout << 1 : std::cout << 0;
		std::cout << "\n";
	}
};


std::vector<int> setprime(int n) {
	std::vector<int> primes;
	primes.reserve(n);
	for (int i = 2; primes.size() < n; i++) {
		bool flag = true;
		for (auto p : primes)
			if (i % p == 0)
				flag = false;
		if (flag)
			primes.push_back(i);
	}
	return primes;
}

class BloomFilter {
private:
	std::vector<int> primes;
	int size;
	int hash;
	BitArray bitarr;

public:
	BloomFilter() {}
	BloomFilter(int N, double P) {

		size = floor(-N * log2(P) / log(2) + 0.5);
		hash = floor(-log2(P) + 0.5);
		primes = setprime(hash + 1);
		bitarr = BitArray(size);

	}

	void add(unsigned long long k) {
		for (int i = 0; i < hash; i++) {
			int prime = primes[i];
			int hesh = ((k % M) * (i + 1) % M + prime % M) % M % size;

			int x = hesh / 8;
			int y = hesh % 8;
			bitarr.bit_set(x, y);
		}
	}

	bool search(unsigned long long k) {
		for (int i = 0; i < hash; i++) {		
			int prime = primes[i];
			int hesh = ((k % M) * (i + 1) % M + prime % M) % M % size;

			int x = hesh / 8;
			int y = hesh % 8;
			bool flag = true;
			flag = bitarr.bit_find(x, y);
			if (!flag)
				return flag;
		}
		return true;
	}

	/*void print() {
		bitarr.print();
	} */

	auto getbit() {
		return &bitarr;
	}

	int getsize() {
		return this->size;
	}

	int gethash() {
		return this->hash;
	}
};

int main() {
	BloomFilter bf;
	std::string line;
	bool start = true;
	bool flag = false;

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	while (std::cin >> line) {
		if (line.empty())
			continue;
		if (line.size() == 3 && start && line[0] == 's') {
			int m;
			float P;
			std::cin >> m >> P;
			if (m == 0 || P <= 0 || P >= 1 || floor(-log2(P) + 0.5) == 0) {
				std::cout << "error\n";
				continue;
			}
			bf = BloomFilter(m, P);
			std::cout << bf.getsize() << " " << bf.gethash() << "\n";
			start = false;
			flag = true;
		}
		else if (line.size() == 3 && !start && line[0] == 's') {
			int m;
			float P;
			std::cin >> m >> P;
			std::cout << "error\n";
		}
		else if (line.size() == 3 && flag) {
			unsigned long long k;
			std::cin >> k;
			bf.add(k);
		}
		else if (line.size() == 6 && flag) {
			unsigned long long k;
			std::cin >> k;
			std::cout << (bf.search(k) ? "1\n" : "0\n");
		}
		else if ((line.size() == 3 || line.size() == 6) && !flag) {
			unsigned long long k;
			std::cin >> k;
			std::cout << "error\n";
		}
		else if (line.size() == 5 && flag) {
			bf.getbit()->print();
		}
		else
			std::cout << "error\n";
	}
}
