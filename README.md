# VectorImplementation
# Vector
_Ранее в курсе был рассмотрен класс динамической строки std::string (см. задание String), которая увеличивает размер массива символов по мере необходимости и самостоятельно управляет выделяемой памятью. В этом задании мы рассмотрим класс std::vector, являющийся обобщением динамической строки на произвольный тип, то есть класс динамического массива._

# std::vector
Интерфейс и реализация std::vector<T> во многом похожи на std::string. Ключевым отличием является семантика работы метода reserve (и остальных методов, которые приводят к увеличению capacity). При резервировании памяти под большее число объектов, выделяется "сырая" (неинициализированная) память достаточная для хранения нужного числа дополнительных объектов. Пустые ячейки заполняются лениво по мере необходимости. То есть, если size < capacity, то это значит, что первые size * sizeof(T) байт реально заняты объектами, а последние (capacity - size) * sizeof(T) байт пусты - объектов там не создано (как такого добиться - см. задание Optional). Это нужно, например, для того, чтобы можно было создавать вектор из объектов, у которых нет конструктора по умолчанию (а как бы тогда нужно было инициализировать неиспользуемые ячейки?):

struct A {\
  int x;\
\
  A() = delete;\
  explicit A(int x_param) : x(x_param) {\
  }\
};\
\
std::vector<A> v;\
v.reserve(1000);  // объекты A не создаются! Выделяется "сырая" память размера 1000 * sizeof(A)\
for (int i = 0; i < 1000; ++i) {\
  v.push_back(A(i));\
  // а лучше v.emplace_back(i);\
}\
Подробности на лекциях, семинарах, в чатах, на заборе, а также в справочнике.

# Детали реализации
От вас требуется реализовать шаблонный класс Vector с единственным шаблонным параметром - типом хранящихся элементов. При реализации можно (и даже нужно) пользоваться обобщенными алгоритмами из STL (_std::copy, std::fill и т.п._), но нельзя использовать стандартные контейнеры. Будет проверяться корректность мультипликативной схемы расширения массива с коэффициентом 2. В базовой версии ручное управление временем жизни объектов не требуется (см. доп. задание). Класс должен поддерживать следующий функционал:

. Конструктор по умолчанию - создает пустой массив;
. Явный конструктор от числа - создает массив заданного размера заполненный объектами, сконструированными по умолчанию;
. Конструктор, принимающий size и value (именно в этом порядке) - создает массив длины size, заполненный элементами со значением value;
. Шаблонный конструктор, принимающий пару итераторов - создает копию переданного диапазона;
# Важно: объявление этого конструктора должно иметь вид

template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>\

Vector(Iterator first, Iterator last)\

Это делает конструктор доступным только в случае, когда на вход приходят два Forward итератора.

* Конструктор от std::initializer_list;
* Правило "пяти";
* Методы Size, Capacity, Empty;
* Константный и неконстантный оператор доступа по индексу []. Неконстантный должен позволять изменять полученный элемент;
* Константный и неконстантный метод доступа по индексу At. При выходе за границы массива должен бросать исключение std::out_of_range;
* Методы Front() и Back()
  - доступ к первому и последнему элементам (тоже по две версии).
* Метод Data()
  - возвращает указатель на начало массива.
* Метод Swap(other)
  - обменивает содержимое с другим массивом other;
* Метод Resize(new_size)
  - изменяет размер на new_size. Если вместимость не позволяет хранить столько элементов, то выделяется новый буфер с вместимостью new_size. Недостающие элементы конструируются по умолчанию.
* Метод Resize(new_size, value)
  - то же, что и Resize(new_size), но в случае new_size > size заполняет недостающие элементы значением value.
* Метод Reserve(new_cap)
  - изменяет вместимость на max(new_cap, текущая вместимость). Размер при этом не изменяется.
* Метод ShrinkToFit()
  - уменьшает capacity до size.
* Метод Clear()
  - устанавливает размер в 0, очищения выделенной памяти при этом НЕ происходит.
* Методы PushBack(const T&) и PushBack(T&&)
  - добавляет новый элемент в конец массива.
* Метод PopBack()
  - удаляет последний элемент.
* Операции сравнения (<, >, <=, >=, ==, !=), задающие лексикографический порядок.
Также реализуйте поддержку итераторов и методы для работы с ними: begin(), end(), cbegin(), cend(), rbegin(), rend(), crbegin(), crend(). begin()-end(), rbegin()-rend() должны иметь две версии, возвращающие константные и неконстантные итераторы.

Внутри класса Vector определите типы-члены ValueType, Pointer, ConstPointer, Reference, ConstReference, SizeType, Iterator, ConstIterator.
