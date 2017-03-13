#include <iostream>
#include <algorithm>
#include <string>

#include <cstdlib>
#include <ctime>

#include "carray.h"

template<class T>
void print_array(
  const CArray<T> &_array,
  const std::string &_header
)
{
  std::cout << "==========[" << _header << "]==========\n";
  for (size_t i = 0; i < _array.size(); ++i)
    std::cout << '[' << i << "]=" << _array[i] << '\n';
  std::cout << std::endl;
}

std::string generate_word()
{
  std::string word;
  size_t length = std::rand() % 5 + 3;
  while (length--) {
    word.push_back(char(std::rand() % ('z' - 'a' + 1) + 'a'));
  }
  return word; // C++11 => move
}

int main(int argc, char *argv[])
{
  std::srand((unsigned int)std::time(nullptr));
  CArray<int> array_int;

  // 1.1. добавление в цикле 20 случайных чисел в диапазоне от 0 до 100.
  for (size_t i = 0; i < 20; ++i)
    array_int.push_back(std::rand() % 101);
  print_array(array_int, "array generation");

  // 1.2. упорядочивание получившегося набора чисел по возрастанию.
  std::sort(array_int.begin(), array_int.end());
  print_array(array_int, "array sorting");

  // 1.3. удаление каждого 2 элемента.
  for (size_t i = 0; i < array_int.size(); ++i)
    array_int.erase(i);
  print_array(array_int, "erase elements from even positions");

  // 1.4. вставка 10 случайных чисел в диапазоне от 0 до 100 на случайные позиции.
  for (size_t i = 0; i < 10; ++i)
    array_int.insert((size_t)std::rand() % array_int.size(), std::rand() % 101);
  print_array(array_int, "random insert");

  // 1.5. очистка контейнера.
  array_int.clear();

  CArray<std::string> array_string;

  // 2.1. добавление в цикле 15 случайно выбранных слов, состоящих из латинских букв в нижнем регистре.
  for (int i = 0; i < 15; ++i)
    array_string.push_back(generate_word());
  print_array(array_string, "array generation");

  // 2.2. упорядочивание получившегося набора слов по возрастанию.
  std::sort(array_string.begin(), array_string.end());
  print_array(array_string, "array sorting");

  // 2.3. удаление каждого слова, включающего в себя любую из букв a, b, c, d, e.
  for (size_t i = 0; i < array_string.size(); ++i)
  {
    if (std::any_of(array_string[i].begin(),
      array_string[i].end(),
      [](char c) { return c >= 'a' && c <= 'e'; }))
    {
      array_string.erase(i--);
    }
  }
  print_array(array_string, "array filtering");

  // 2.4. вставка 3 новых случайно выбранных слов на случайные позиции.
  for (size_t i = 0; i < 3; ++i)
    array_string.insert((size_t)std::rand() % array_string.size(), generate_word());
  print_array(array_string, "random insert");

  return 0;
}

