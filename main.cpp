#include <iostream>
using namespace std;

template<typename data_type>
struct LinkedNode {
    data_type data;
    LinkedNode *next;

    LinkedNode() {
        next = nullptr;
    }

    LinkedNode(data_type data, LinkedNode<data_type> *next = nullptr) : data(data), next(next) {}
};

template<typename data_type>
struct DoubleLinkedNode {
    data_type data;
    DoubleLinkedNode *next;
    DoubleLinkedNode *prev;

    DoubleLinkedNode() {
        next = nullptr;
        prev = nullptr;
    }
    DoubleLinkedNode(data_type data, DoubleLinkedNode<data_type> *next = nullptr, DoubleLinkedNode<data_type> *prev = nullptr) : data(data), next(next), prev(prev) {}
};

template<typename data_type>
struct LinkedList {
    LinkedNode<data_type> *head;
    LinkedNode<data_type> *tail;

    LinkedList() {
        head = nullptr;
        tail = nullptr;
    }
    void push_front(data_type data) {
        LinkedNode<data_type> *new_node = new LinkedNode<data_type>(data, head);
        if (head == nullptr) {
            tail = new_node;
        }
        head = new_node;
    }

    void push_back(data_type data) {
        if (tail == nullptr) {
            head = tail = new LinkedNode<data_type>(data);
        } else {
            tail->next = new LinkedNode<data_type>(data);
            tail = tail->next;
        }
    }
    void insert(LinkedNode<data_type> *node, data_type data) {
        LinkedNode<data_type> *new_node = new LinkedNode<data_type>(data, node->next);
        if (node == tail) {
            tail = new_node;
        }
        node->next = new_node;
    }

    void insert(int k, data_type data) {
        if (k == 0) {
            push_front(data);
        }
        else {
            LinkedNode<data_type> *current = head;
            for (int i = 0; i <= k - 1; ++i) {
                current = current->next;
            }
            insert(current, data);
        }
    }

    void pop_front() {
        LinkedNode<data_type> *current = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete current;
    }

    void erase(LinkedNode<data_type> *node) {
        if (node -> next) {
            LinkedNode<data_type> *current = node->next;
            if (node->next == tail) {
                tail = node;
            }
            node -> next = node -> next -> next;
            delete current;
        }
    }

    void erase(int k) {
        if (k == 0) {
            pop_front();
        }
        else {
            LinkedNode<data_type> *current = head;
            for (int i = 0; i < k - 1; ++i) {
                current = current->next;
            }
            erase(current);
        }
    }

    void print() {
        LinkedNode<data_type> *current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

template<typename data_type>
struct DoubleLinkedList {
    DoubleLinkedNode<data_type> *head;
    DoubleLinkedNode<data_type> *tail;
    DoubleLinkedList() {
        head = nullptr;
        tail = nullptr;
    }
    void insert(DoubleLinkedNode<data_type> *node, data_type data) {
        DoubleLinkedNode<data_type> *new_node = new DoubleLinkedNode<data_type>(data, node->next, node);
        if (node == tail) {
            tail = new_node;
        }
        node->next = new_node;
    }

    void insert(int k, data_type data) {
        if (k == 0) {
            push_front(data);
        }
        else {
            DoubleLinkedNode<data_type> *current = head;
            for (int i = 0; i <= k - 1; ++i) {
                current = current->next;
            }
            insert(current, data);
        }
    }
    
    void print() {
        DoubleLinkedNode<data_type> *current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
    void push_front(data_type data) {
        DoubleLinkedNode<data_type> *new_node = new DoubleLinkedNode<data_type>(data, head);
        if (head == nullptr) {
            tail = new_node;
        } else {
            head->prev = new_node;
        }
        head = new_node;
    }
    void erase(DoubleLinkedNode<data_type> *node) {
        if (node == head) {
            if (head == tail) {
                delete node;
                head = tail = nullptr;
            }
            else {
                head = head->next;
                head->prev = nullptr;
                delete node;
            }
        } else if (node == tail) {
            tail = node->prev;
            tail->next = nullptr;
            delete node;
        } else {
            DoubleLinkedNode<data_type> *predecesor = node->prev;
            DoubleLinkedNode<data_type> *sucesor = node->next;
            predecesor->next = node->next;
            sucesor->prev = node->prev;
            delete node;
        }
    }
    void delete_first() {
        DoubleLinkedNode<data_type> *current = head;
        while (current != nullptr) {
            if (current->data == head->data) {
                erase(current);
                break;
            }
            current = current->next;
        }
    }
    void pop_front() {
        DoubleLinkedNode<data_type> *current = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete current;
    }
    void pop_back() {
        DoubleLinkedNode<data_type> *current = tail;
        tail = tail->prev;
        if (tail == nullptr) {
            head = nullptr;
        } else {
            tail->next = nullptr;
        }
        delete current;
    }
};

struct CircularLinkedList {

};

int main() {
    DoubleLinkedList<int> L;
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        string op;
        cin >> op;
        if (op[0] == 'i') {
            int k, x;
            cin >> k >> x;
            L.insert(k, x);
        }
        else if (op.size() == 6){
            int x;
            cin >> x;
            L.delete_first();
        }
        else if (op.size() == 10) {}
    }
    return 0;
}