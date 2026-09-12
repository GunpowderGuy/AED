#include <string>
#include <vector>

#include <iostream>

#include <algorithm>

#include <sstream>


using u64 = unsigned long long;
using i64 = long long;



i64 contarRepeticiones(const std::vector<i64>& a, const i64 x) {
    i64 n = a.size(); // paremetro a debe estar ordenado

    // primera posicion donde a[i] >= x
    int lo = 0, hi = n, first = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (a[mid] >= x) {
            first = mid;
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    // primera posicion donde a[i] > x
    lo = 0; hi = n;
    int last = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (a[mid] > x) {
            last = mid;
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return last - first;
}



/*
 s td::vector<i*64> contarQueries(const std::vector<i64>& a, const std::vector<i64>& queries) {
 std::unordered_map<i64, i64> freq;
 freq.reserve(a.size() * 2);

 for (i64 x : a) freq[x]++;

 std::vector<i64> ans;
 ans.reserve(queries.size());

 for (i64 x : queries) ans.push_back(freq[x]);

 return ans;
 }
 */



std::vector<i64> contarQueries(std::vector<i64> a, const std::vector<i64>& queries) {
    std::ranges::sort(a); // C++23

    std::vector<i64> ans;
    ans.reserve(queries.size());

    for (i64 x : queries) {
        ans.push_back(contarRepeticiones(a, x));
    }

    return ans;
}


std::string solve(const std::string& input) {
    std::istringstream in(input);

    int n, q;
    in >> n >> q;

    std::vector<i64> a(n);
    for (int i = 0; i < n; i++) in >> a[i];

    std::vector<i64> queries(q);
    for (int i = 0; i < q; i++) in >> queries[i];

    std::vector<i64> ans = contarQueries(a, queries);

    std::string out;
    for (i64 x : ans) {
        out += std::to_string(x);
        out += '\n';
    }

    return out;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string input((std::istreambuf_iterator<char>(std::cin)),
                      std::istreambuf_iterator<char>());

    const std::string output = solve(input);

    std::cout << output;
    return 0;
}
