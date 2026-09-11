#include <string>
#include <vector>

[[nodiscard]] bool is_balanced(const std::string& s) {
    std::vector<char> st;
    st.reserve(s.size());

    for (char c : s) {
        switch (c) {
            case '(':
            case '[':
            case '{':
                st.push_back(c);
                break;

            case ')':
                if (st.empty() || st.back() != '(') return false;
                st.pop_back();
            break;
            case ']':
                if (st.empty() || st.back() != '[') return false;
                st.pop_back();
            break;
            case '}':
                if (st.empty() || st.back() != '{') return false;
                st.pop_back();
                break;

            default:
                return false;
        }
    }
    return st.empty();
}

[[nodiscard]] std::string solve_one(const std::string& bracket_string) {
    return is_balanced(bracket_string) ? "YES" : "NO";
}

[[nodiscard]] std::vector<std::string> solve_all(const std::vector<std::string>& inputs) {
    std::vector<std::string> out;
    out.reserve(inputs.size());
    for (const auto& s : inputs) {
        out.push_back(solve_one(s));
    }
    return out;
}

#include <iostream>

int main2() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<std::string> inputs;
    inputs.reserve(n);
    for (int i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;
        inputs.push_back(std::move(s));
    }

    for (const auto& ans : solve_all(inputs)) {
        std::cout << ans << '\n';
    }
   return 0;
}


using u64 = unsigned long long;
using i64 = long long;



i64 contarRepeticiones(const std::vector<i64>& a, const i64 x) {
    i64 n = a.size();

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

#include <algorithm>

std::vector<i64> contarQueries(std::vector<i64> a, const std::vector<i64>& queries) {
    std::ranges::sort(a); // C++23

    std::vector<i64> ans;
    ans.reserve(queries.size());

    for (i64 x : queries) {
        ans.push_back(contarRepeticiones(a, x));
    }

    return ans;
}


#include <sstream>

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
