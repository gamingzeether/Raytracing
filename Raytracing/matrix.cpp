#include "matrix.h"

matrix matrix::add(const matrix& other) {
	if (this->rows != other.rows || this->columns != other.columns) {
		throw;
	}

	matrix new_matrix = matrix(rows, columns);
	for (int i = 0; i < rows * columns; i++) {
		new_matrix.values[i] = this->values[i] + other.values[i];
	}
	return new_matrix;
}

matrix matrix::scalar_multiply(float scalar) {
	matrix new_matrix = matrix(rows, columns);
	for (int i = 0; i < rows * columns; i++) {
		new_matrix.values[i] = this->values[i] * scalar;
	}
	return new_matrix;
}

matrix matrix::transpose() {
	matrix new_matrix = matrix(rows, columns);
	for (int r = 0; r < this->rows; r++) {
		for (int c = 0; c < this->columns; c++) {
			new_matrix.set(r, c, this->get(r, c));
		}
	}
	return new_matrix;
}

const matrix matrix::matrix_multiply(const matrix& other) {
	if (this->columns != other.rows) {
		throw;
	}

	matrix new_matrix = matrix(this->rows, other.columns);
	for (int r = 0; r < new_matrix.rows; r++) {
		for (int c = 0; c < new_matrix.columns; c++) {
			float sum = 0;
			for (int i = 0; i < this->columns; i++) {
				float a = this->get(r, i);
				float b = other.get(i, c);
				sum += a * b;
			}
			new_matrix.set(r, c, sum);
		}
	}
	return new_matrix;
}
