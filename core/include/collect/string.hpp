#ifndef FLOCK_STRING_HPP
#define FLOCK_STRING_HPP

#include "common.hpp"
#include "maybe.hpp"
#include "vector.hpp"

namespace flock {
    class FLK_API string {
        vector<char> vec_ = vector<char>::create();

    public:
        static string create();
        static string with_cap(usize capacity);
        static string from(const char *str);

        string(const string &other)     = default;
        string(string &&other) noexcept = default;

        string &operator=(const string &other)     = default;
        string &operator=(string &&other) noexcept = default;

        ~string() = default;

        usize len() const;
        usize cap() const;
        bool  is_empty() const;

        const char *first() const;
        char *      first();

        const char *last() const;
        char *      last();

        const char *get(usize index) const;
        char *      get(usize index);

        const char &operator[](usize index) const;
        char &      operator[](usize index);

        void reserve(usize additional);
        void shrink_to_fit();
        void shrink_to(usize new_cap);
        void resize(usize new_len, char value = {});

        void push(char value = {});
        char pop();
        void insert(usize index, char value = {});
        void insert_str(usize index, const string &str);
        char remove(usize index);
        void removen(usize index, usize num);
        void append(const string &other);

        bool         starts_with(const string &str) const;
        bool         ends_with(const string &str) const;
        bool         contains(const string &str) const;
        maybe<usize> find(const string &str) const;

        void trim();
        void trim_start();
        void trim_end();
        void make_uppercase();
        void make_lowercase();

        void replace(const string &str, const string &replace);
        void replacen(const string &str, const string &replace, usize num);

        void clear();

    private:
        string() = default;
    };
}

#endif //FLOCK_STRING_HPP
