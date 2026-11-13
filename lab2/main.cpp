#include <iostream>



//Односвязный линейный связный список
template <typename T>
class LinkedList {
private:

    struct Node {
        T value_{};
        Node *next_{nullptr};
    };

    Node *head_{nullptr};
    Node *tail_{nullptr};
    std::size_t size_{0};

    void check_not_empty() const {
        if (size_ == 0) { throw std::out_of_range("List is empty"); }
    }

    static void check_range(const T &min_value, const T &max_value) {
        if (min_value > max_value) { throw std::logic_error("the lower bounder greater than higher bounder"); }
    }

    void Swap(LinkedList &other) noexcept
    {
        std::swap(head_, other.tail_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

public:
    //Конструктор по умолчанию
    LinkedList() = default;

    //Конструктор копирования
    LinkedList(const LinkedList &other) {

    }
    //Конструктор, заполняющий список случайными значениями согласно заданию. С настройкой сида

    //Деструктор
    ~LinkedList() {  }

    //Операция присваивания. 
    
    // Добавление элемента в конец списка — push_tail,
    void push_tail(const T &value) {
        Node *new_node = new Node{value, nullptr}; //агрегатную инициализацию (списочную инициализацию)
        if (size_ == 0) {
            head_ = tail_ = new_node;
        } else {
            tail_->next_ = new_node;
            tail_ = new_node;
        }
        return;
    }
    // Добавление другого списка LinkedList в конец списка — push_tail 	(перегруженный метод).

    // Добавление элемента в начало списка — push_head.
    void push_head(const T &value) {
        Node *new_node = new Node{value, nullptr}; //агрегатная инициализацию (списочную инициализацию)
        if (size_ == 0) {
            head_ = tail_ = new_node;
        } else {
            new_node->next_ = head_;
            head_ = new_node;
        }
    }
    // Добавление списка LinkedList в начало списка — push_head (перегруженный метод).

    // Удаление элемента из начала списка — pop_heads.
    void pop_heads() {
        check_not_empty();

        if (size_ == 1) {
            delete head_;
            head_ = tail_ = nullptr;
        } else {
            Node *ptr = head_->next_;
            delete head_;
            head_ = ptr;
        }
        
        --size_;
    }
    // Удаление элемента из конца списка — pop_tails.
    void pop_tails() {
        Node *ptr = head_->next_;
        delete head_;
        head_ = ptr;
    }
    // Удаление всех элементов Node с информационным полем, равным переданному — delete_node. 	

    // Операция доступа по индексу (в двух вариантах для чтения/записи). В случае варианта с циклическим списком брать элемент по модулю(т.е. для любого индекса есть соответствующий элемент списка).

    // Операция вывода списка на печать (перегруженный).

};


//Реализуйте функцию print_reverse() вывода списка на печать в обратном порядке.
template <typename T>
bool print_reverse() {

}

int main() {

    return 0;
}