#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

class Matrix {
	unsigned rows;
	unsigned cols;
	std::vector<double> values;
	
public:
	Matrix(unsigned rows_, unsigned cols_) : rows(rows_), cols(cols_), values(rows_ * cols_) {
	}
	Matrix(Matrix const & other)
		: rows(other.rows), cols(other.cols), values(other.values.begin(), other.values.end()) {
	}
	const Matrix& operator=(Matrix const& other) {
		rows = other.rows;
		cols = other.cols;
		values = other.values;
		return *this;
	}

	unsigned getNumberOfRows() const {
		return rows;
	}
	unsigned getNumberOfColumns() const {
		return cols;
	}
	double get(unsigned row, unsigned col) const {
		return values[row * cols + col];
	}
	void set(unsigned row, unsigned col, double value) {
		values[row * cols + col] = value;
	}
	std::vector<double> multiply(std::vector<double> const& vec) const {
		if (vec.size() != cols) {
			throw "matrix second dimension does not match vector dimension";
		}
		std::vector<double> result(rows);
		for (unsigned r = 0; r < rows; r++) {
			result[r] = 0;
			for (unsigned c = 0; c < cols; c++) {
				result[r] += get(r, c) * vec[c];
			}
		}
		return result;
	}
	template<class Distribution, class Generator>
	void randomize(Distribution& d, Generator& gen) {
		for (double& v: values) {
			v = d(gen);
		}
	}
};

std::vector<double> operator-(std::vector<double> const& a, std::vector<double> const& b) {
	if (a.size() != b.size()) {
		throw "vector dimensions do not match";
	}
	std::vector<double> c(a.size());
	for (unsigned i = 0; i < c.size(); i++) {
		c[i] = a[i] - b[i];
	}
	return c;
}
std::vector<double> operator/(std::vector<double> a, double b) {
	for (double& v: a) {
		v = v / b;
	}
	return a;
}
std::vector<double> squared(std::vector<double> a) {
	for (double& v: a) {
		v = v * v;
	}
	return a;
}
#endif
