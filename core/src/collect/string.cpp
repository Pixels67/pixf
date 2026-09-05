#include "collect/string.hpp"

#include <cstring>

namespace flock {
    string string::create() {
        string str{};
        return str;
    }

    string string::with_cap(usize capacity) {
        string str{};
        str.vec_ = vector<char>::with_cap(capacity);

        return str;
    }

    string string::from(const char *str) {
        const usize str_len = strlen(str);

        string out_str{};
        out_str.vec_ = vector<char>::with_cap(str_len);

        for (usize i = 0; i < str_len; i++) {
            out_str.vec_.push(str[i]);
        }

        return out_str;
    }

    usize string::len() const {
        return vec_.len();
    }

    usize string::cap() const {
        return vec_.cap();
    }

    bool string::is_empty() const {
        return vec_.is_empty();
    }

    const char *string::first() const {
        return vec_.first();
    }

    char *string::first() {
        return vec_.first();
    }

    const char *string::last() const {
        return vec_.last();
    }

    char *string::last() {
        return vec_.last();
    }

    const char *string::get(usize index) const {
        return vec_.get(index);
    }

    char *string::get(usize index) {
        return vec_.get(index);
    }

    const char &string::operator[](usize index) const {
        return vec_[index];
    }

    char &string::operator[](usize index) {
        return vec_[index];
    }

    void string::reserve(usize additional) {
        return vec_.reserve(additional);
    }

    void string::shrink_to_fit() {
        return vec_.shrink_to_fit();
    }

    void string::shrink_to(usize new_cap) {
        return vec_.shrink_to(new_cap);
    }

    void string::resize(usize new_len, char value) {
        return vec_.resize(new_len, value);
    }

    void string::push(char value) {
        return vec_.push(value);
    }

    char string::pop() {
        return vec_.pop();
    }

    void string::insert(usize index, char value) {
        return vec_.insert(index, value);
    }

    void string::insert_str(usize index, const string &str) {
        FLK_ASSERT(index <= vec_.len(), "Out of bounds index");

        if (index == vec_.len()) {
            append(str);
            return;
        }

        resize(vec_.len() + str.len());

        for (usize i = vec_.len() - 1; i > index + str.len() - 1; i--) {
            vec_[i] = vec_[i - str.len()];
        }

        for (usize i = index; i < index + str.len(); i++) {
            vec_[i] = str[i - index];
        }
    }

    char string::remove(usize index) {
        return vec_.remove(index);
    }

    void string::removen(usize index, usize num) {
        return vec_.removen(index, num);
    }

    void string::append(const string &other) {
        return vec_.append(other.vec_);
    }

    bool string::starts_with(const string &str) const {
        if (str.len() > len()) {
            return false;
        }

        for (usize i = 0; i < str.len(); i++) {
            if ((*this)[i] != str[i]) {
                return false;
            }
        }

        return true;
    }

    bool string::ends_with(const string &str) const {
        if (str.len() > len()) {
            return false;
        }

        for (usize i = len() - str.len(), j = 0; i < len(); i++, j++) {
            if ((*this)[i] != str[j]) {
                return false;
            }
        }

        return true;
    }

    bool string::contains(const string &str) const {
        return find(str).has_value();
    }

    maybe<usize> string::find(const string &str) const {
        if (str.len() > len()) {
            return {};
        }

        const char *ptr = vec_.first();
        for (usize i = 0; i <= len() - str.len(); i++) {
            bool equal = true;
            for (usize j = 0; j < str.len(); j++) {
                if (ptr[j] != str[j]) {
                    equal = false;
                    break;
                }
            }

            if (equal) {
                return i;
            }

            ptr++;
        }

        return {};
    }

    void string::trim() {
        trim_end();
        trim_start();
    }

    void string::trim_start() {
        if (vec_.is_empty()) {
            return;
        }

        while (!vec_.is_empty() && *vec_.first() == ' ') {
            vec_.remove(0);
        }
    }

    void string::trim_end() {
        if (vec_.is_empty()) {
            return;
        }

        while (!vec_.is_empty() && *vec_.last() == ' ') {
            vec_.resize(vec_.len() - 1);
        }
    }

    void string::make_uppercase() {
        for (usize i = 0; i < vec_.len(); i++) {
            if (vec_[i] >= 'a' && vec_[i] <= 'z') {
                vec_[i] -= 'a' - 'A';
            }
        }
    }

    void string::make_lowercase() {
        for (usize i = 0; i < vec_.len(); i++) {
            if (vec_[i] >= 'A' && vec_[i] <= 'Z') {
                vec_[i] += 'a' - 'A';
            }
        }
    }

    void string::replace(const string &str, const string &replace) {
        return replacen(str, replace, ~0u);
    }

    void string::replacen(const string &str, const string &replace, usize num) {
        auto  maybe = find(str);
        usize i     = 0;
        while (maybe.has_value() && i < num) {
            removen(maybe.get(), str.len());
            insert_str(maybe.get(), replace);
            maybe = find(str);

            i++;
        }
    }

    void string::clear() {
        return vec_.clear();
    }
}
