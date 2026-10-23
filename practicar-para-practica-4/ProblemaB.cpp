#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>

using namespace std;

class ColaDoblePrioridad {
public:
    void llega(int prioridad) {
        pedidos_.insert({prioridad, 0});
    }

    optional<int> atiende_menor() {
        if (pedidos_.empty())
            return nullopt;
        auto it = pedidos_.begin();
        int p = it->first;
        pedidos_.erase(it);
        return p;
    }

    optional<int> atiende_mayor() {
        if (pedidos_.empty())
            return nullopt;
        auto it = prev(pedidos_.end());
        int p = it->first;
        pedidos_.erase(it);
        return p;
    }

private:
    multimap<int, int> pedidos_;
};

int main() {
    int q;
    cin >> q;
    ColaDoblePrioridad cola;
    while (q--) {
        int tipo;
        cin >> tipo;
        if (tipo == 1) {
            int x;
            cin >> x;
            cola.llega(x);
        } else {
            optional<int> atendido =
                (tipo == 2) ? cola.atiende_menor() : cola.atiende_mayor();
            if (atendido)
                cout << *atendido << '\n';
            else
                cout << "VACIA" << '\n';
        }
    }
    return 0;
}
