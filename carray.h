#ifndef CARRAY_H
#define CARRAY_H

#include <cstddef>
#include <utility>
#include <stdexcept>
#define NDEBUG
#include <cassert>

template <typename TData>
class CArray
{
public: // Interface
  typedef CArray<TData> my_type;

  // Конструктор по умолчанию
  CArray()
    : m_size(0)
    , m_capacity(0)
    , m_buffer(nullptr)
  {}

  // Копирующий конструктор
  CArray(
    const my_type &_array
  )
    : m_size(0)
    , m_capacity(0)
    , m_buffer(nullptr)
  {
    my_type tmp_array;
    tmp_array.reserve(_array.size());
    tmp_array.__unsafe_push_back(_array.begin(), _array.size());

    swap(tmp_array);
  }

  // Если пределён, нетривиальный конструтор копирования нужно пререопределить оператор =
  my_type &operator=(
    const my_type &other
    )
  {
    if (this != &other)
    {
      my_type(other).swap(*this);
      return *this;
    }
  }

  // Деструктор
  ~CArray()
  {
    if (m_buffer)
    {
      clear();
      operator delete(m_buffer);
      m_buffer = nullptr;
    }
  }

  // Добавить элемент в конец массива
  void push_back(
    const TData &_value
  )
  {
    if (m_capacity > m_size)
    {
      __unsafe_push_back(_value);
    }
    else
    {
      my_type tmp_array;
      tmp_array.reserve(m_size + 1);
      tmp_array.__unsafe_push_back(begin(), size());
      tmp_array.__unsafe_push_back(_value);
      swap(tmp_array);
    }
  }

  // Добавить элемент в массив по заданному индексу
  void insert(
    unsigned int _index,
    const TData &_value
  )
  {
    if (_index > m_size)
      throw std::out_of_range("_index should be less then array size");

    if (_index == size())
      push_back(_value);

    if (m_capacity > m_size)
    {
      __unsafe_push_back(m_buffer[m_size - 1]);

      for (size_t i = m_size - 2; i != _index; --i)
        m_buffer[i] = m_buffer[i - 1];

      m_buffer[_index] = _value;
    }
    else
    {
      my_type tmp_array;
      tmp_array.reserve(m_size + 1);
      tmp_array.__unsafe_push_back(begin(), _index);
      tmp_array.__unsafe_push_back(_value);
      tmp_array.__unsafe_push_back(begin() + _index, size() - _index);
      swap(tmp_array);
    }
  }

  // Удалить элемент массива по заданному индексу
  void erase(
    unsigned int _index
  )
  {
    if (_index >= m_size)
      throw std::out_of_range("_index should be less then array size");

    for (size_t i = _index + 1; i < m_size; ++i)
      m_buffer[i - 1] = m_buffer[i];

    --m_size;
    m_buffer[m_size].~TData();
  }

  // Очистить массив
  void clear()
  {
    while (m_size != 0)
    {
      m_buffer[m_size - 1].~TData();
      --m_size;
    }
  }

  // Получить размер массива
  unsigned int size() const
  {
    return m_size;
  }

  // Получить размер буффера
  unsigned int capacity() const
  {
    return m_capacity;
  }

  // Получить элемент массива по заданному индексу
  TData &operator[](
    unsigned int _index
    )
  {
    return m_buffer[_index];
  }

  // Получить элемент константного массива по заданному индексу
  const TData &operator[](
    unsigned int _index
    ) const
  {
    return m_buffer[_index];
  }

  void swap(
    my_type &_array
  )
  {
    if (this != &_array)
    {
      std::swap(m_buffer, _array.m_buffer);
      std::swap(m_capacity, _array.m_capacity);
      std::swap(m_size, _array.m_size);
    }
  }

  // При необходимости выделяет память под массив размера _size
  void reserve(
    unsigned int _capacity
  )
  {
    if (!m_buffer) {
      size_t capacity = __calculate_new_size(_capacity);
      m_buffer = static_cast<TData *> (operator new(sizeof(TData) * capacity));
      m_capacity = capacity;
      return;
    }

    if (m_capacity < _capacity)
    {
      my_type tmp_array;
      tmp_array.reserve(_capacity);
      tmp_array.__unsafe_push_back(begin(), size());
      swap(tmp_array);
    }
  }

  TData *begin()
  {
    return m_buffer;
  }

  const TData *begin() const
  {
    return m_buffer;
  }

  TData *end()
  {
    return m_buffer + m_size;
  }

  const TData *end() const
  {
    return m_buffer + m_size;
  }

protected: // Attributes
  size_t m_size;
  size_t m_capacity;
  TData *m_buffer;

private:
  // рассчитывает размер буфера под массив размера _size равный степени 2.
  size_t __calculate_new_size(size_t _size) const
  {
    size_t size = 1;
    while (size < _size)
      size <<= 1;
    return size;
  }

  // добавляет в конец буфера count элементов подряд начиная с begin не проверяя размер!
  void __unsafe_push_back(
    const TData *_begin,
    size_t       _count
  )
  {
    assert(m_capacity - m_size >= _count);
    for (size_t i = 0; i < _count; ++i)
    {
      new (m_buffer + m_size) TData(_begin[i]);
      ++m_size;
    }
  }

  // добавляет в конец буфера _value не проверяя размер!
  void __unsafe_push_back(
    const TData &_value
  )
  {
    assert(m_capacity > m_size);
    new(m_buffer + m_size) TData(_value);
    ++m_size;
  }
};

#endif // CARRAY_H
