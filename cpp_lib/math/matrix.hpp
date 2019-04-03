#ifndef MATRIX_H
#define MATRIX_H

/**
    Matrix class

    Author: Marcus Östling

    TODO:
    Rename Matris -> Matrix (regex)
    Implement faster matrix multiplication algorithm
*/

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <type_traits>
#include <math.h>

template <class T>
class Matris {
private:
    size_t m_rows;
    size_t m_cols;
    size_t m_capacity;
    T * m_vec;

    bool dimCheck(const Matris<T> &rhs) {
        if(this->cols() != rhs.cols() || this->rows() != rhs.rows()) {
            throw std::invalid_argument("Must be the same dimension");
        }
        return true;
    };
protected:
public:
    static_assert(std::is_move_constructible<T>::value, "Type must be moveConstructible");
    static_assert(std::is_move_assignable<T>::value, "Type must be moveAssignable");
    
    // Constructors
    Matris() : Matris(0) {};
    explicit Matris(const int n) : m_rows(n), m_cols(n), m_capacity(n*n), m_vec(nullptr) {
        if(n != 0) {
            m_vec = new T[n*n];
            for (int i = 0; i < n*n; ++i) {
                m_vec[i] = T();
            }
        }
    };
    Matris(const Matris &copy) : m_rows(copy.m_rows), m_cols(copy.m_cols), 
                                 m_capacity(copy.m_capacity) {
        m_vec = new T[m_capacity];
        for (size_t i = 0; i < copy.m_capacity; ++i){
            m_vec[i] = copy.m_vec[i];
        }
    }
    Matris(Matris &&other) : m_rows(other.m_rows), m_cols(other.m_cols), 
                             m_capacity(other.m_capacity), m_vec(other.m_vec) {
        other.m_rows = 0;
        other.m_cols = 0;
        other.m_capacity = 0;
        other.m_vec = nullptr;
    };
    Matris(std::initializer_list<T> init_list) {
        size_t sroot = std::sqrt(init_list.size());
        if(sroot*sroot != init_list.size())
            throw std::out_of_range("Size of the init_list must be an even square root.");
        
        m_rows = sroot;
        m_cols = sroot;
        m_capacity = sroot*sroot;
        m_vec = new T[sroot*sroot];

        size_t count = 0;
        for (auto it = init_list.begin(); it < init_list.end(); ++it) {
            this->m_vec[count] = *it;
            count++;
        }
    };
    virtual ~Matris() {
        delete[] this->m_vec;
    }

    //Iterator
    typedef T* iterator;
    iterator begin() {return m_vec;};
    iterator end() {return &(this->m_vec[m_capacity]);};
    typedef const T* const_iterator;
    const_iterator begin() const {return m_vec;};
    const_iterator end() const {return &(this->m_vec[m_capacity]);};

    // Accessors
    size_t cols() const {return this->m_cols; };
    size_t rows() const {return this->m_rows; };
    
    // Operators
    T& operator()(const size_t row, const size_t col) {
        if(row < this->rows() && col < this->cols())
            return m_vec[row*this->cols() + col];
        throw std::out_of_range("Element out of range");
    };
    const T& operator()(const size_t row, const size_t col) const {
        if(row < this->rows() && col < this->cols())
            return m_vec[row*this->cols() + col];
        throw std::out_of_range("Element out of range");
    };

    bool operator!=(Matris<T> &other) {
        if(*this == other)
            return false;
        return true;
    };
    bool operator==(Matris<T> &other) {
        if(this->cols() == other.cols() &&
           this->rows() == other.rows() &&
           this->m_capacity == other.m_capacity) {
            for (size_t i = 0; i < this->m_capacity; ++i) {
                if(this->m_vec[i] != other.m_vec[i])
                    return false;
            }
            return true;
        }
        return false;
    };
    
    void operator=(Matris<T> &&rhs) {
        if(*this != rhs) {
            delete[] this->m_vec;
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_capacity = rhs.m_capacity;
            this->m_vec = rhs.m_vec;
            rhs.m_rows = 0;
            rhs.m_cols = 0;
            rhs.m_capacity = 0;
            rhs.m_vec = new T[0];
        }
    };
    void operator=(Matris<T> &rhs) {
        if(*this != rhs) {
            delete[] this->m_vec;
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_capacity = rhs.m_capacity;
            this->m_vec = new T[m_capacity];
            for (size_t i = 0; i < this->m_capacity; ++i) {
                this->m_vec[i] = rhs.m_vec[i];
            }
            
        }
    };

    void operator+=(const T &rhs) {
        for (size_t i = 0; i < this->m_capacity; ++i) {
            m_vec[i] += rhs;
        }
    };
    void operator-=(const T &rhs) {
        for (size_t i = 0; i < m_capacity; ++i) {
            m_vec[i] -= rhs;
        }
    };
    void operator*=(const T &rhs) {
        for (size_t i = 0; i < m_capacity; ++i) {
            m_vec[i] *= rhs;
        }
    };

    void operator+=(const Matris<T> &rhs) {
        this->dimCheck(rhs);
        for (size_t i = 0; i < this->m_capacity; ++i){
            this->m_vec[i] += rhs.m_vec[i];
        }
    };
    void operator-=(const Matris<T> &rhs) {
        dimCheck(rhs);
        for (size_t i = 0; i < m_capacity; ++i){
            m_vec[i] -= rhs.m_vec[i];
        }
    };
    void operator*=(const Matris<T> &rhs) {
        if(this->cols() != rhs.rows())
            throw std::invalid_argument("This cols must equal thats rows.");
        
        Matris<T> tmp(*this);
        this->m_cols = rhs.cols();
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0;j < this->cols(); ++j) {
                T tmp_sum = 0;
                for (size_t k = 0; k < rhs.rows(); ++k) {
                    tmp_sum += tmp(i,k)*rhs(k,j);   
                }
                (*this)(i,j) = tmp_sum;
            }
        }
        
    };

    friend Matris<T> operator+(const Matris<T> &lhs, const Matris<T> &rhs) {
        Matris<T> m(lhs);
        m += rhs;
        return m;
    };
    friend Matris<T> operator-(const Matris<T> &lhs, const Matris<T> &rhs) {
        Matris<T> m(lhs);
        m -= rhs;
        return m;
    }
    friend Matris<T> operator*(const Matris<T> &lhs, const Matris<T> &rhs) {
        Matris<T> m(lhs);
        m *= rhs;
        return m;
    };
    
    friend std::ostream& operator<<(std::ostream& os, const Matris<T> &m) {
        os << m.rows() << " " << m.cols() << std::endl;
        for (unsigned int i = 0; i < m.rows(); ++i) {
            for (unsigned int j = 0; j < m.cols(); ++j) {
                os << m(i,j) << " ";
            }
            if(i+1 != m.rows())
                os << std::endl;
        }
        return os;
    }; 
    friend std::istream& operator>>(std::istream& is, Matris<T> &m) {
        is >> m.m_rows >> m.m_cols;
        m.m_capacity = m.cols() * m.rows();
        delete[] m.m_vec;
        m.m_vec = new T[m.m_capacity];
        for (auto it = m.begin(); it != m.end(); ++it) {
            is >> *it;
        }
        return is;
    }; 

    // Methods
    void reset() {
        for (auto it = (*this).begin(); it != (*this).end(); it++) {
            *it = T();
        }
    };
    Matris<T> identity(const unsigned int dim) const {
        Matris<T> tmp(dim);
        for (size_t i = 0; i < tmp.cols(); ++i) {
            tmp(i,i) = 1;
        }
        return tmp;
    };
    
    void insert_row(const unsigned int before_row) {
        if (0 > before_row || before_row > this->rows()-1 || this->rows() == 0)
            throw std::out_of_range("Row out of range");

        size_t new_cap = this->m_capacity+this->cols();
        T* new_vec = new T[new_cap];
        size_t offset = 0;
        for (size_t i = 0; i < new_cap; ++i) {
            if(i >= this->cols() * before_row
               && i < this->cols()*before_row + this->cols()) {
                new_vec[i] = 0;
                offset++;
            } else {
                new_vec[i] = this->m_vec[i-offset];
            }
        }
        delete[] this->m_vec;
        this->m_capacity = new_cap;
        this->m_vec = new_vec;
        this->m_rows++;
    }
    void append_row(const unsigned int after_row) {
        if (0 > after_row || after_row > this->rows()-1 || this->rows() == 0)
            throw std::out_of_range("Row out of range");

        size_t new_cap = this->m_capacity+this->cols();
        T* new_vec = new T[new_cap];
        size_t offset = 0;
        for (size_t i = 0; i < new_cap; ++i) {
            if(i >= this->cols() * (after_row+1)
               && i < this->cols() * (after_row+1) + this->cols()) {
                new_vec[i] = 0;
                offset++;
            } else {
                new_vec[i] = this->m_vec[i-offset];
            }
        }
        delete[] this->m_vec;
        this->m_capacity = new_cap;
        this->m_vec = new_vec;
        this->m_rows++;
    };
    void remove_row(const unsigned int row) {
        if (0 > row || row > this->rows()-1 || this->rows() == 0)
            throw std::out_of_range("Row out of range");

        size_t new_cap = this->m_capacity-this->cols();
        T* new_vec = new T[new_cap];
        size_t offset = 0;
        for (size_t i = 0; i < new_cap; ++i) {
            if(i == this->cols() * row)
                offset = this->cols();
            new_vec[i] = this->m_vec[i+offset];
        }
        delete[] this->m_vec;
        this->m_capacity = new_cap;
        this->m_vec = new_vec;
        this->m_rows--;
    };

    void insert_column(const unsigned int before_col) {
        if (0 > before_col || before_col > this->cols()-1 || this->cols() == 0)
            throw std::out_of_range("Column out of range");

        size_t new_cap = this->m_capacity+this->rows();
        T* new_vec = new T[new_cap];
        size_t offset = 0;
        for (size_t i = 0; i < new_cap; ++i) {
            if(i%(this->cols()+1) == before_col) {
                new_vec[i] = 0;
                offset++;
            } else {
                new_vec[i] = this->m_vec[i-offset];
            }
        }
        delete[] this->m_vec;
        this->m_capacity = new_cap;
        this->m_vec = new_vec;
        this->m_cols++;
    };
    void append_column(const unsigned int after_col) {
        if (0 > after_col || after_col > this->cols()-1 || this->cols()==0)
            throw std::out_of_range("Column out of range");

        size_t new_cap = this->m_capacity+this->rows();
        T* new_vec = new T[new_cap];
        size_t offset = 0;
        for (size_t i = 0; i < new_cap; ++i) {
            if(i%(this->cols()+1) == after_col+1) {
                new_vec[i] = 0;
                offset++;
            } else {
                new_vec[i] = this->m_vec[i-offset];
            }
        }
        delete[] this->m_vec;
        this->m_capacity = new_cap;
        this->m_vec = new_vec;
        this->m_cols++;
    };
    void remove_column(const unsigned int col) {
        if (0 > col || col > this->cols()-1 || this->cols() == 0)
            throw std::out_of_range("Column out of range");

        size_t new_cap = this->m_capacity-this->rows();
        T* new_vec = new T[new_cap];
        size_t offset = 0;
        for (size_t i = 0; i < new_cap; ++i) {
            if((i+offset)%(this->cols()) == col) {
                offset++;
            }
            new_vec[i] = this->m_vec[i+offset];
        }
        delete[] this->m_vec;
        this->m_capacity = new_cap;
        this->m_vec = new_vec;
        this->m_cols--;
    };
    
    Matris& transpose() {
        Matris<T> tmp(*this);
        size_t old_cols = this->cols();
        this->m_cols = this->m_rows;
        this->m_rows = old_cols;
        for(size_t i = 0; i < this->rows(); ++i) {
            for(size_t j = 0; j < this->cols(); ++j) {
                (*this)(i,j) = tmp(j,i);
            }
        }
        return *this;
    };

    void swap(Matris<T> &m1, Matris <T> &m2) {
        Matris<T> tmp(std::move(m1));
        m1 = std::move(m2);
        m2 = std::move(tmp);
    }
    
};

#endif
