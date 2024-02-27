#include "vector.h"

#include <iterator>
#include <algorithm>
#include <numeric>
#include <cmath>    


namespace linalg {

    Vector::Vector(std::size_t n) : data_(n) {}

    Vector::Vector(std::size_t n, float val) : data_(n, val) {}

    Vector::Vector(std::initializer_list<float> list) : data_(list) {}

    auto Vector::operator=(float val) -> Vector& {
        assign(val);
        return *this;
    }

    auto Vector::assign(float val) -> void {
        std::fill(begin(), end(), val);
    }

    auto Vector::assign(Vector v) -> void {
        //data_ = v.data_;
        // This should be better 
        data_ = std::move(v.data_);
    }

    auto Vector::size() const -> std::size_t {
        return data_.size();
    }

    auto Vector::begin() -> iterator {
        return data_.begin();
    }

    auto Vector::end() -> iterator {
        return data_.end();
    }

    auto Vector::begin() const -> const_iterator {
        return data_.cbegin();
    }

    auto Vector::end() const -> const_iterator {
        return data_.cend();
    }

    auto Vector::cbegin() const -> const_iterator {
        return data_.cbegin();
    }

    auto Vector::cend() const -> const_iterator {
        return data_.cend();
    }

    auto Vector::operator[](int idx) -> float & {
        if (std::abs(idx) >= size())
            throw std::out_of_range("Index out of range");
        if (idx < 0)
            idx += static_cast<int>(size());

        return data_[idx];
    }

    auto Vector::operator[](int idx) const -> const float & {
        if (std::abs(idx) >= size())
            throw std::out_of_range("Index out of range");
        if (idx < 0) 
            idx += static_cast<int>(size());

        return data_[idx];
    }

    auto Vector::coeff(int idx) -> float & {
    if (idx < 0 || idx >= static_cast<int>(size()))
        throw std::out_of_range("Index out of range");

    return data_[idx];
    }

    auto Vector::coeff(int idx) const -> const float & {
        if (idx < 0 || idx >= static_cast<int>(size())) 
            throw std::out_of_range("Index out of range");

        return data_[idx];
    }

    auto Vector::operator+=(float val) -> Vector& {
        for (auto& x : *this)
            x += val;
        return *this;
    }

    auto Vector::operator-=(float val) -> Vector& {
        for (auto& x : *this)
            x -= val;
        return *this;
    }

    auto Vector::operator*=(float val) -> Vector& {
        for (auto& x : *this)
            x *= val;
        return *this;
    }

    auto Vector::operator/=(float val) -> Vector& {
        for (auto& x : *this)
            x /= val;
        return *this;
    }

    auto Vector::operator+=(const Vector &y) -> Vector & {
        if (size() != y.size())
            throw std::invalid_argument("Vectors must have the same size");

        for (unsigned int i = 0; i < size(); ++i)
            data_[i] += y[i];

        return *this;
    }

    auto Vector::operator-=(const Vector &y) -> Vector & {
        if (size() != y.size())
            throw std::invalid_argument("Vectors must have the same size");

        for (unsigned int i = 0; i < size(); ++i)
            data_[i] -= y[i];

        return *this;
    }

    auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
        ostr << "[ ";
        std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
        ostr << "]";

        return ostr;
    }


    auto min(const Vector& x) -> float {
        if (x.size() == 0)
            throw std::invalid_argument("Vector must not be empty");
        
        return *std::min_element(x.begin(), x.end());
    }

    auto max(const Vector &x) -> float {
        if (x.size() == 0)
            throw std::invalid_argument("Vector must not be empty");

        return *std::max_element(x.begin(), x.end());
    }

    auto argmin(const Vector &x) -> std::size_t {
        if (x.size() == 0)
            throw std::invalid_argument("Vector must not be empty");

        return std::distance(x.begin(), std::min_element(x.begin(), x.end()));
    }

    auto argmax(const Vector &x) -> std::size_t {
        if (x.size() == 0)
            throw std::invalid_argument("Vector must not be empty");

        return std::distance(x.begin(), std::max_element(x.begin(), x.end()));
    }

    auto non_zeros(const Vector &x) -> std::size_t {
        return std::count_if(x.begin(), x.end(), [](float element){ return element != 0; });
    }

    auto sum(const Vector &x) -> float {
        return std::accumulate(x.begin(), x.end(), 0.0f);
    }

    auto prod(const Vector &x) -> float {
        return std::accumulate(x.begin(), x.end(), 1.0f, std::multiplies<float>());
    }

    auto dot(const Vector &x, const Vector &y) -> float {
        if (x.size() != y.size())
            throw std::invalid_argument("Vectors must have the same size");

        return std::inner_product(x.begin(), x.end(), y.begin(), 0.0f);
    }

    auto norm(const Vector &x) -> float {
        return std::sqrt(dot(x, x));
    }

    auto normalize(Vector &x) -> void {
        float norm_x = norm(x);
        x /= norm_x;
    }

    auto normalized(const Vector &x) -> Vector {
        Vector v(x);
        normalize(v);
        return v;
    }

    auto floor(const Vector &x) -> Vector {
        Vector v(x);
        std::transform(v.begin(), v.end(), v.begin(), [](float element){ return std::floor(element); });
        return v;
    }

    auto ceil(const Vector &x) -> Vector {
        Vector v(x);
        std::transform(v.begin(), v.end(), v.begin(), [](float element){ return std::ceil(element); });
        return v;
    }

    auto operator+(const Vector &x) -> Vector {
        Vector v(x);
        return v;
    }

    auto operator-(const Vector &x) -> Vector {
        Vector v(x);
        std::transform(v.begin(), v.end(), v.begin(), [](float element){ return -element; });
        return v;
    }

    auto operator+(const Vector &x, const Vector &y) -> Vector {
        Vector v(x);
        v += y;
        return v;
    }

    auto operator-(const Vector &x, const Vector &y) -> Vector {
        Vector v(x);
        v -= y;
        return v;
    }

    auto operator+(const Vector &x, float val) -> Vector {
        Vector v(x);
        v += val;
        return v;
    }

    auto operator-(const Vector &x, float val) -> Vector {
        Vector v(x);
        v -= val;
        return v;
    }

    auto operator*(const Vector &x, float val) -> Vector {
        Vector v(x);
        v *= val;
        return v;
    }

    auto operator/(const Vector &x, float val) -> Vector {
        Vector v(x);
        v /= val;
        return v;
    }

    auto operator+(float val, const Vector &x) -> Vector {
        Vector v(x);
        v += val; // z = 42 + x is the same as z = x + 42
        return v;
    }

    auto operator-(float val, const Vector &x) -> Vector {
        Vector v(x);
        // z = 42 - x is not the same as z = x - 42, se I implemented this loop:
        for (auto& element : v)
            element = val - element;
        return v;
    }

    auto operator*(float val, const Vector &x) -> Vector {
        Vector v(x);
        v *= val;
        return v;
    }
}

/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */
