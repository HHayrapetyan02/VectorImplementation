#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

class VectorOutOfRange : public std::out_of_range {
 public:
  VectorOutOfRange() : std::out_of_range("VectorViewOutOfRange") {
  }
};

template <class T>
class Vector {
  T* ptr_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  template <class G>
  friend bool operator<(const Vector<G>&, const Vector<G>&);
  template <class G>
  friend bool operator>(const Vector<G>&, const Vector<G>&);
  template <class G>
  friend bool operator<=(const Vector<G>&, const Vector<G>&);
  template <class G>
  friend bool operator>=(const Vector<G>&, const Vector<G>&);
  template <class G>
  friend bool operator==(const Vector<G>&, const Vector<G>&);
  template <class G>
  friend bool operator!=(const Vector<G>&, const Vector<G>&);

  Vector() = default;

  explicit Vector(const size_t& size) : size_(size), capacity_(size) {
    ptr_ = size != 0 ? new T[size]{} : nullptr;
  }

  Vector(const size_t& size, const T& value) : size_(size), capacity_(size) {
    if (size_ != 0) {
      ptr_ = new T[size];
      try {
        std::fill(ptr_, ptr_ + size_, value);
      } catch (...) {
        delete[] ptr_;
        throw;
      }
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) : size_(std::distance(first, last)), capacity_(size_) {
    if (size_ != 0) {
      ptr_ = new T[std::distance(first, last)];
      try {
        for (size_t i = 0; i < size_; ++i) {
          *(ptr_ + i) = *(first);
          ++first;
        }
      } catch (...) {
        delete[] ptr_;
        throw;
      }
    }
  }

  Vector(const std::initializer_list<T>& init) : size_(init.size()), capacity_(init.size()) {
    if (size_ != 0) {
      ptr_ = new T[init.size()];
      try {
        T* temp = ptr_;
        for (const auto& elem : init) {
          *temp = elem;
          ++temp;
        }
      } catch (...) {
        delete[] ptr_;
        throw;
      }
    }
  }

  Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_) {
    if (other.capacity_ != 0) {
      ptr_ = new T[other.capacity_];
      try {
        for (size_t i = 0; i < other.size_; ++i) {
          *(ptr_ + i) = *(other.ptr_ + i);
        }
      } catch (...) {
        delete[] ptr_;
        throw;
      }
    }
  }

  Vector(Vector&& other) noexcept : ptr_(other.ptr_), size_(other.size_), capacity_(other.capacity_) {
    other.ptr_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ~Vector() {
    delete[] ptr_;
    size_ = 0;
    capacity_ = 0;
  }

  size_t Size() const noexcept {
    return size_;
  }

  bool Empty() const noexcept {
    return (size_ == 0);
  }

  size_t Capacity() const noexcept {
    return capacity_;
  }

  T& At(const size_t& i) {
    if (i < size_) {
      return ptr_[i];
    }
    throw VectorOutOfRange{};
  }

  const T& At(size_t i) const {
    if (i < size_) {
      return ptr_[i];
    }
    throw VectorOutOfRange{};
  }

  T& Front() {
    return ptr_[0];
  }

  const T& Front() const {
    return ptr_[0];
  }

  T& Back() {
    return ptr_[size_ - 1];
  }

  const T& Back() const {
    return ptr_[size_ - 1];
  }

  T* Data() {
    return ptr_;
  }

  const T* Data() const {
    return ptr_;
  }

  void Swap(Vector& other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(ptr_, other.ptr_);
  }

  void Resize(const size_t& new_size) {
    if (new_size > capacity_) {
      auto* temp = new T[new_size]{};
      for (size_t i = 0; i < size_; ++i) {
        temp[i] = std::move(ptr_[i]);
      }
      delete[] ptr_;
      ptr_ = temp;
      capacity_ = new_size;
    }
    size_ = new_size;
  }

  void Resize(const size_t& new_size, const T& value) {
    if (new_size > capacity_) {
      auto* temp = new T[new_size]{};
      try {
        for (size_t i = size_; i < new_size; ++i) {
          temp[i] = value;
        }
      } catch (...) {
        delete[] temp;
        throw;
      }
      for (size_t i = 0; i < size_; ++i) {
        temp[i] = std::move(ptr_[i]);
      }
      delete[] ptr_;
      ptr_ = temp;
      capacity_ = new_size;
      size_ = new_size;
    } else {
      try {
        for (size_t i = size_; i < new_size; ++i) {
          ptr_[i] = value;
        }
      } catch (...) {
        for (size_t i = size_; i < new_size; ++i) {
          ptr_[i].~T();
        }
        throw;
      }
      size_ = new_size;
    }
  }

  void Reserve(const size_t& new_capacity) {
    size_t max = std::max(capacity_, new_capacity);
    if (max != capacity_) {
      auto temp = new T[max];
      for (size_t i = 0; i < size_; ++i) {
        temp[i] = std::move(ptr_[i]);
      }
      delete[] ptr_;
      ptr_ = temp;
    }
    capacity_ = max;
  }

  void ShrinkToFit() {
    if (size_ != 0) {
      auto temp = new T[size_];
      for (size_t i = 0; i < size_; ++i) {
        temp[i] = std::move(ptr_[i]);
      }
      delete[] ptr_;
      ptr_ = temp;
      capacity_ = size_;
    } else {
      delete[] ptr_;
      ptr_ = nullptr;
      capacity_ = 0;
    }
  }

  void Clear() noexcept {
    size_ = 0;
  }

  void PushBack(const T& value) {
    if (size_ == capacity_) {
      size_t size = capacity_ == 0 ? 1 : capacity_ * 2;
      auto temp = new T[size]{};
      for (size_t i = 0; i < size_; ++i) {
        temp[i] = std::move(ptr_[i]);
      }
      try {
        temp[size_] = value;
      } catch (...) {
        delete[] temp;
        throw;
      }
      delete[] ptr_;
      ptr_ = temp;
      ++size_;
      capacity_ = size;
    } else {
      try {
        ptr_[size_] = value;
      } catch (...) {
        ptr_[size_].~T();
        throw;
      }
      ++size_;
    }
  }

  void PushBack(T&& value) {
    if (size_ == capacity_) {
      capacity_ = capacity_ == 0 ? 1 : capacity_ * 2;
      auto temp = new T[capacity_]{};
      for (size_t i = 0; i < size_; ++i) {
        temp[i] = std::move(ptr_[i]);
      }
      delete[] ptr_;
      temp[size_] = std::move(value);
      ++size_;
      ptr_ = temp;
    } else {
      ptr_[size_] = std::move(value);
      ++size_;
    }
  }

  void PopBack() noexcept {
    ptr_[size_ - 1].~T();
    --size_;
  }

  Iterator begin() {  // NOLINT
    return ptr_;
  }

  ConstIterator begin() const {  // NOLINT
    return ptr_;
  }

  ConstIterator cbegin() const {  // NOLINT
    return ptr_;
  }

  Iterator end() {  // NOLINT
    return ptr_ + size_;
  }

  ConstIterator end() const {  // NOLINT
    return ptr_ + size_;
  }

  ConstIterator cend() const {  // NOLINT
    return ptr_ + size_;
  }

  ReverseIterator rbegin() {  // NOLINT
    return ReverseIterator(ptr_ + size_);
  }

  ConstReverseIterator rbegin() const {  // NOLINT
    return ConstReverseIterator(ptr_ + size_);
  }

  ConstReverseIterator crbegin() const {  // NOLINT
    return ConstReverseIterator(ptr_ + size_);
  }

  ReverseIterator rend() {  // NOLINT
    return ReverseIterator(ptr_);
  }

  ConstReverseIterator rend() const {  // NOLINT
    return ConstReverseIterator(ptr_);
  }

  ConstReverseIterator crend() const {  // NOLINT
    return ConstReverseIterator(ptr_);
  }

  Vector& operator=(const Vector& other) {
    if (this != &other) {
      if (capacity_ < other.size_) {
        delete[] ptr_;
        try {
          ptr_ = new T[other.size_];
        } catch (...) {
          ptr_ = nullptr;
          size_ = 0;
          capacity_ = 0;
          throw;
        }
        capacity_ = other.size_;
      }
      size_ = other.size_;
      try {
        for (size_t i = 0; i < other.size_; ++i) {
          ptr_[i] = other.ptr_[i];
        }
      } catch (...) {
        size_ = 0;
        throw;
      }
    }
    return *this;
  }

  Vector& operator=(Vector&& other) noexcept {
    if (this != &other) {
      delete[] ptr_;
      ptr_ = other.ptr_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.ptr_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  T& operator[](const size_t& i) noexcept {
    return ptr_[i];
  }

  const T& operator[](const size_t& i) const noexcept {
    return ptr_[i];
  }
};

template <class T>
bool operator<(const Vector<T>& first, const Vector<T>& second) {
  size_t min = std::min(first.size_, second.size_);
  for (size_t i = 0; i < min; ++i) {
    if (first.ptr_[i] != second.ptr_[i]) {
      return first.ptr_[i] < second.ptr_[i];
    }
  }
  return first.size_ < second.size_;
}

template <class T>
bool operator>(const Vector<T>& first, const Vector<T>& second) {
  size_t min = std::min(first.size_, second.size_);
  for (size_t i = 0; i < min; ++i) {
    if (first.ptr_[i] != second.ptr_[i]) {
      return first.ptr_[i] > second.ptr_[i];
    }
  }
  return first.size_ > second.size_;
}

template <class T>
bool operator==(const Vector<T>& first, const Vector<T>& second) {
  if (first.size_ != second.size_) {
    return false;
  }
  for (size_t i = 0; i < first.size_; ++i) {
    if (first.ptr_[i] != second.ptr_[i]) {
      return false;
    }
  }
  return true;
}

template <class T>
bool operator!=(const Vector<T>& first, const Vector<T>& second) {
  return !(first == second);
}

template <class T>
bool operator<=(const Vector<T>& first, const Vector<T>& second) {
  return !(first > second);
}

template <class T>
bool operator>=(const Vector<T>& first, const Vector<T>& second) {
  return !(first < second);
}

#endif
