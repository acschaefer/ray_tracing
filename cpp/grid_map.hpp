#ifndef GRID_MAP_H_
#define GRID_MAP_H_ GRID_MAP_H_

#include <algorithm>
#include <array>
#include <exception>
#include <sstream>
#include <vector>


template <int N>
class GridMap {
private:
    std::array<int, N> shape;
    std::array<double, N> size;
    std::array<int, N> offset;
    int elements;
    std::vector<int> hits;
    std::vector<int> misses;

public:
    GridMap(
                std::array<int, N> const & shape, 
                std::array<double, N> const & size) 
            : shape(shape), size(size) {
        if (N <= 0) {
            throw std::invalid_argument(
                "Dimensionality of space must be positive.");
        }

        if (*std::min_element(this->shape.begin(), this->shape.end()) < 1 
                || *std::min_element(
                        this->size.begin(), this->size.end()) <= 0.0) {
            throw std::invalid_argument(
                "Input arguments \"shape\" and \"size\" must be positive.");
        }

        this->elements = 1;
        for (int i = N - 1; i >= 0; --i) {
            this->offset[i] = this->elements;
            this->elements *= this->shape[i];
        }

        this->hits.assign(this->elements, 0);
        this->misses.assign(this->elements, 0);
    }

protected:
    int linear_index(std::array<int, N> const & index) const {
        int linear_index = 0;
        for (int i = 0; i < N; ++i) {
            linear_index += index[i] * this->offset[i];
        }
        return linear_index;
    }

public:
    int & get_hit(std::array<int, N> const & index) {
        return this->hits[this->linear_index(index)];
    }

    int & get_miss(std::array<int, N> const & index) {
        return this->misses[this->linear_index(index)];
    }

    std::vector<int> const & get_hits() const {
        return this->hits;
    }

    std::vector<int> const & get_misses() const {
        return this->misses;
    }

    GridMap & set_hits(std::vector<int> const & hits) {
        if (this->elements != (int)hits.size()) {
            std::stringstream msg;
            msg << "Input argument \"hits\" must have " << this->elements 
                << " elements.";
            throw std::invalid_argument(msg.str());
        }
        this->hits = hits;
        return *this;
    }

    GridMap & set_misses(std::vector<int> const & misses) {
        if (this->elements != (int)hits.size()) {
            std::stringstream msg;
            msg << "Input argument \"misses\" must have " << this->elements
                << " elements.";
            throw std::invalid_argument(msg.str());
        }
        this->misses = misses;
        return *this;
    }

    std::array<int, N> get_shape() const {
        return this->shape;
    }

    std::array<double, N> get_size() const {
        return this->size;
    }

    bool operator==(GridMap const & map) const {
        if (this->shape != map.shape) {
            return false;
        }

        if (this->size != map.size) {
            return false;
        }

        if (this->misses != map.misses || this->hits != map.hits) {
            return false;
        }

        return true;
    }

    void operator+=(GridMap const & map) {
        if (this->shape != map.shape || this->size != map.size) {
            throw std::invalid_argument(
                "Both maps must have the same shapes and sizes.");
        }

        for (int i = 0; i < this->elements; ++i) {
            this->hits[i] += map.hits[i];
            this->misses[i] += map.misses[i];
        }
    }
};


#endif
