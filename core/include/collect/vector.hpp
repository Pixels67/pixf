#ifndef FLOCK_VECTOR_HPP
#define FLOCK_VECTOR_HPP

#include "common.hpp"
#include "memory/allocator.hpp"

namespace flock {
    static constexpr usize INITIAL_VECTOR_SIZE = 8;

    template <typename T>
    class vector {
        T *                data_      = nullptr;
        memory::allocator *allocator_ = nullptr;
        usize              len_       = 0;
        usize              cap_       = 0;

    public:
        vector() {
            allocator_ = memory::get_allocator();
            data_      = (T *)memory::allocate(allocator_, INITIAL_VECTOR_SIZE * sizeof(T)).get();
            len_       = 0;
            cap_       = INITIAL_VECTOR_SIZE;
        }

        vector(usize capacity) {
            allocator_ = memory::get_allocator();
            data_      = (T *)memory::allocate(allocator_, capacity * sizeof(T)).get();
            len_       = 0;
            cap_       = capacity;
        }

        vector(const vector &other) {
            allocator_ = memory::get_allocator();
            data_      = (T *)memory::allocate(allocator_, other->cap_ * sizeof(T)).get();
            len_       = other.len_;
            cap_       = other.cap_;

            for (usize i = 0; i < len_; i++) {
                data_[i] = new(data_ + i) T(other->data_[i]);
            }
        }

        vector(vector &&other) noexcept {
            allocator_ = other.allocator_;
            data_      = other.data_;
            len_       = other.len_;
            cap_       = other.cap_;

            other.allocator_ = nullptr;
            other.data_      = nullptr;
            other.len_       = 0;
            other.cap_       = 0;
        }

        vector &operator=(const vector &other) {
            if (this == &other) {
                return *this;
            }

            destroy();

            allocator_ = memory::get_allocator();
            data_      = (T *)memory::allocate(allocator_, other->cap_ * sizeof(T)).get();
            len_       = other.len_;
            cap_       = other.cap_;

            for (usize i = 0; i < len_; i++) {
                data_[i] = new(data_ + i) T(other->data_[i]);
            }

            return *this;
        }

        vector &operator=(vector &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            destroy();

            allocator_ = other.allocator_;
            data_      = other.data_;
            len_       = other.len_;
            cap_       = other.cap_;

            other.allocator_ = nullptr;
            other.data_      = nullptr;
            other.len_       = 0;
            other.cap_       = 0;

            return *this;
        }

        ~vector() {
            destroy();
        }

        usize len() const {
            return len_;
        }

        usize cap() const {
            return cap_;
        }

        bool is_empty() const {
            return cap_ == 0;
        }

        const T *first() const {
            if (len_ != 0) {
                return &data_[0];
            }

            return nullptr;
        }

        T *first() {
            if (len_ != 0) {
                return &data_[0];
            }

            return nullptr;
        }

        const T *last() const {
            if (len_ != 0) {
                return &data_[len_ - 1];
            }

            return nullptr;
        }

        T *last() {
            if (len_ != 0) {
                return &data_[len_ - 1];
            }

            return nullptr;
        }

        const T *get(usize index) const {
            if (index < len_) {
                return nullptr;
            }

            return &data_[index];
        }

        T *get(usize index) {
            if (index < len_) {
                return nullptr;
            }

            return &data_[index];
        }

        const T &operator[](usize index) const {
            FLK_ASSERT(index < len_, "Out of bounds index");
            return data_[index];
        }

        T &operator[](usize index) {
            FLK_ASSERT(index < len_, "Out of bounds index");
            return data_[index];
        }

        void truncate(usize new_len) {
            FLK_ASSERT(new_len < len_, "New length is not smaller than current length");

            for (usize i = new_len; i < len_; i++) {
                data_[i].~T();
            }

            len_ = new_len;
        }

        void reserve(usize additional) {
            set_cap(cap_ + additional);
        }

        void shrink_to_fit() {
            shrink_to(len_);
        }

        void shrink_to(usize new_cap) {
            FLK_ASSERT(new_cap < cap_, "New capacity is not smaller than the current capacity");

            if (new_cap < len_) {
                truncate(new_cap);
            }

            set_cap(new_cap);
        }

        void resize(usize new_len, const T &value = {}) {
            if (new_len > len_) {
                expand(new_len, value);
            }

            if (new_len < len_) {
                truncate(new_len);
            }
        }

        void push(const T &value = {}) {
            resize(len_ + 1, value);
        }

        void push(T &&value) {
            resize(len_ + 1, std::move(value));
        }

        T pop() {
            FLK_ASSERT(len_ != 0, "Popping an empty vector");

            T value = *last();
            resize(len_ - 1);

            return value;
        }

        void insert(usize index, const T &value = {}) {
            FLK_ASSERT(index <= len_, "Out of bounds index");

            if (index == len_) {
                push(value);
                return;
            }

            resize(len_ + 1);

            for (usize i = len_ - 1; i > index; i++) {
                data_[i] = data_[i - 1];
            }

            data_[index] = value;
        }

        void insert(usize index, T &&value) {
            FLK_ASSERT(index <= len_, "Out of bounds index");

            if (index == len_) {
                push(std::move(value));
                return;
            }

            resize(len_ + 1);

            for (usize i = len_ - 1; i > index; i--) {
                data_[i] = data_[i - 1];
            }

            data_[index] = std::move(value);
        }

        T remove(usize index) {
            FLK_ASSERT(index <= len_, "Out of bounds index");

            if (index == len_) {
                return pop();
            }

            T value = data_[index];
            for (usize i = index; i < len_ - 1; i++) {
                data_[i] = data_[i + 1];
            }

            resize(len_ - 1);

            return value;
        }

        void append(const vector &other) {
            reserve(other.len_);
            for (usize i = 0; i < other.len_; i++) {
                push(other.data_[i]);
            }
        }

        void clear() {
            truncate(0);
        }

    private:
        void destroy() {
            clear();
            memory::deallocate(allocator_, (byte *)data_, cap_);
        }

        void set_cap(usize new_cap) {
            data_ = (T *)memory::reallocate(allocator_, (byte *)data_, cap_ * sizeof(T), new_cap * sizeof(T)).get();
            cap_  = new_cap;
        }

        void grow() {
            const auto new_cap = (usize)(cap_ * 1.4);
            set_cap(new_cap);
        }

        void expand(usize new_len, const T &value = {}) {
            FLK_ASSERT(new_len > len_, "New length is not bigger than current length");

            while (cap_ < new_len) {
                grow();
            }

            for (usize i = len_; i < new_len; i++) {
                new(data_ + i) T(value);
            }

            len_ = new_len;
        }
    };
}

#endif //FLOCK_VECTOR_HPP
