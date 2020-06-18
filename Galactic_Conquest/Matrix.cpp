
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include "Matrix.h"


Matrix Matrix::Identity()
	{ // Identity()
	// quick hack: diagonal elements are zero mod 5
	Matrix result;
	for (int i = 0; i < 16; i++)
		result.coords[i] = !(i%5);
	return result;
	} // Identity()
	
// returns a zero matrix
Matrix Matrix::Zero()
	{ // Zero()
	Matrix result;
	for (int i = 0; i < 16; i++)
		result.coords[i] = 0;
	return result;
	} // Zero()


// returns a specified element in the matrix
GLfloat &Matrix::Element(int row, int col)
	{ // Element()
	// column-major, so multiply column by height
	return coords[row + 4 * col];
	} // Element()
	
// Copy another Matrix & return self
Matrix &Matrix::operator = (Matrix otherMatrix)
	{ // copy
	for (int i = 0; i < 16; i++)
		coords[i] = otherMatrix.coords[i];
	return (*this);
	} // copy

// Add another Matrix
Matrix Matrix::operator + (Matrix otherMatrix)
	{ // addition
	Matrix result;
	for (int i = 0; i < 16; i++)
		result.coords[i] = coords[i] + otherMatrix.coords[i];
	return result;
	} // addition
	
// Subtract another Matrix
Matrix Matrix::operator - (Matrix otherMatrix)
	{ // addition
	Matrix result;
	for (int i = 0; i < 16; i++)
		result.coords[i] = coords[i] - otherMatrix.coords[i];
	return result;
	} // addition
	
// Scale the matrix by a constant 
Matrix Matrix::operator * (GLfloat scaleFactor)
	{ // scalar right-multiplication
	Matrix result;
	for (int i = 0; i < 16; i++)
		result.coords[i] = coords[i] * scaleFactor;
	return result;
	} // scalar right-multiplication

// Scale the matrix by a constant 
Matrix operator * (GLfloat scaleFactor, Matrix matrix)
	{ // scalar right-multiplication
	Matrix result;
	for (int i = 0; i < 16; i++)
		result.coords[i] = scaleFactor * matrix.coords[i];
	return result;
	} // scalar right-multiplication
	
// Left-multiply a Point
Point Matrix::operator *(Point point)
	{ // left-multiply a point
	Point result;
	// loop through rows of matrix, multiplying by point
	for (int row = 0; row < 4; row++)
		{ // for row
		result.coords[row] = 0.0;
		// loop along row, multiplying and adding
		for (int col = 0; col < 4; col++)
			result.coords[row] += Element(row,col)* point.coords[col]; 
		} // for row
	return result;
	} // left-multiply a point
	
// Left-multiply a Vector
Vector Matrix::operator *(Vector vector)
	{ // left-multiply a vector
	Vector result;
	// loop through rows of matrix, multiplying by point
	for (int row = 0; row < 3; row++)
		{ // for row
		result.coords[row] = 0.0;
		// loop along row, multiplying and adding
		// note that we ignore the fourth (w) coord
		for (int col = 0; col < 3; col++)
			result.coords[row] += Element(row,col)* vector.coords[col]; 
		} // for row
	return result;
	} // left-multiply a vector
	
// multiply by another matrix
Matrix Matrix::operator *(Matrix otherMatrix)
	{ // matrix multiply
	// make a temporary copy of the other
	Matrix result;
	
	// now do the standard loops: it's inefficient, but works
	for (int row = 0; row < 4; row++)
		for(int col = 0; col < 4; col++)
			{ // for each element
			result.Element(row, col) = 0.0;
			for (int entry = 0; entry < 4; entry++)
				{ // for entry
				result.Element(row, col) += Element(row, entry) * 
					otherMatrix.Element(entry, col);
				} // for entry
			} // for each element
	return result;
	} // matrix multiply
	
// Scales a row by a given factor
void Matrix::ScaleRow(int row, GLfloat scaleFactor)
	{ // ScaleRow()
	for (int entry = 0; entry < 4; entry++)
		Element(row, entry) *= scaleFactor;
	} // ScaleRow()

// Swaps two rows
void Matrix::SwapRows(int row1, int row2)
	{ // SwapRows()
//	printf("Swapping rows %d and %d\n", row1, row2);
	GLfloat temp;
	for (int entry = 0; entry < 4; entry++)
		{ // for each entry
		temp = Element(row1, entry);
		Element(row1, entry) = Element(row2, entry);
		Element(row2, entry) = temp;
		} // for each entry	
	} // SwapRows()

// Adds multiple of one row to another row
void Matrix::AddRows(int row1, GLfloat multiple, int row2)
	{ // AddRows()
	// simple loop to add entries
	for (int entry = 0; entry < 4; entry++)
		Element(row1, entry) += multiple * Element(row2, entry);
	} // AddRows()

// Reduces all elements in a column except the diagonal element
// Returns 0 on success, 1 if the column cannot be reduced
int Matrix::ReduceColumn(int column, Matrix &inverse)
	{ // ReduceColumn()
	// loop variable for working down column
	int row, pivot = column;

	// find the first non-zero element in the column 
	// that is not above the diagonal
	for (row = pivot; row < 4; row++)
		if (fabs(Element(row, pivot)) > ERROR_TOLERANCE)
			break;

	// if we didn't find one, return an error code
	if (row == 4) return 1;
	
	// if we did, but it wasn't on the diagonal, swap with the pivot row
	// this makes sure we never get divide by zero errors
	if (row != pivot)
		{ // for both matrices
		SwapRows(pivot, row);
		inverse.SwapRows(pivot, row);
		} // for both matrices

	// now scale the pivot row so the pivot element is one
	GLfloat scaleFactor = 1.0 / Element(pivot, pivot);
	ScaleRow(pivot, scaleFactor);
	inverse.ScaleRow(pivot, scaleFactor);

	// for each row
	for (row = 0; row < 4; row++)
		{ // for each row
		// skip the row we're pivoting on
		if (row == column) continue;		
		scaleFactor = -Element(row, pivot);
		AddRows(row, scaleFactor, pivot);	
		inverse.AddRows(row, scaleFactor, pivot);	
		} // for each row
	
	// and we're done
	return 0;
	} // ReduceColumn()

// Computes a matrix inverse with brute force Gauss-Jordan method
// returns a zero matrix on failure
Matrix Matrix::Inverse()
	{ // Inverse()
	// working copy of the matrix
	Matrix workingCopy = (*this);
	Matrix result = Matrix::Identity();
	
	// now perform Gauss-Jordan reduction on the elements of each column
	if (0 != workingCopy.ReduceColumn(0, result)) return Zero();
	if (0 != workingCopy.ReduceColumn(1, result)) return Zero();
	if (0 != workingCopy.ReduceColumn(2, result)) return Zero();
	if (0 != workingCopy.ReduceColumn(3, result)) return Zero();

	// and we're done!
	return result;
	} // Inverse()

// routines to extract the translation part of the matrix
// assumes no entries in perspective section of matrix
Vector Matrix::TranslationVector()
	{ // TranslationVector()
	Vector result;
	// copy last column into result vector
	for (int i = 0; i < 3; i++)
		result.coords[i] = coords[i,3];
	return result;
	} // TranslationVector()
	
Matrix Matrix::TranslationMatrix()
	{ // TranslationMatrix()
	Matrix result = Identity();

	// copy last column into result
	for (int i = 0; i < 3; i++)
		result.Element(i,3) = Element(i,3);
	return result;
	} // TranslationMatrix()

// routine to extract the rotation part of the matrix
// assumes no entries in perspective section of matrix
Matrix Matrix::RotationMatrix()
	{ // RotationMatrix()
	Matrix result = Identity();

	// copy first three elements of first three columns into result
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			result.Element(i,j) = Element(i,j);
	return result;
	} // RotationMatrix()

// routine to read it in
// returns 0 on success, non-0 on failure
long Matrix::Read(FILE *inFile)
	{ //	Read()
	long nScanned;
	for (int row = 0; row < 4; row++)
		{ // for row
		// simplify life by computing first element in row
		GLfloat *first = coords+row;
		nScanned = fscanf(inFile, "%f %f %f %f ", 
			first, first+4, first+8, first+12);
		if (nScanned != 4) return -1;
		} // for row
	return 0;
	} //	Read()	
	
// routine to print it out
void Matrix::Print(FILE *outFile)
	{ //	Print()
	for (int row = 0; row < 4; row++)
		{ // for row
		// simplify life by computing first element in row
		GLfloat *first = coords+row;
		fprintf(outFile, "%10.6f %10.6f %10.6f %10.6f\n",
			first[0], first[4], first[8], first[12]);
		} // for row
	} //	Print()	

void Matrix::printToConsole(){
	std::cout << "Matrix " << this->coords[0] << " " << this->coords[4] << " " << this->coords[8] << " " << this->coords[12] << std::endl;
	std::cout << "Matrix " << this->coords[1] << " " << this->coords[5] << " " << this->coords[9] << " " << this->coords[13] << std::endl;
	std::cout << "Matrix " << this->coords[2] << " " << this->coords[6] << " " << this->coords[10] << " " << this->coords[14] << std::endl;
	std::cout << "Matrix " << this->coords[3] << " " << this->coords[7] << " " << this->coords[11] << " " << this->coords[15] << std::endl;	
}

bool Matrix::testMatrixValidity(GLfloat *m){
	double epsilon = 0.01; // margin to allow for rounding errors

	if (fabs(m[0]*m[4] + m[4]*m[5] + m[8]*m[9]) > epsilon) return false;
	if (fabs(m[0]*m[2] + m[4]*m[6] + m[8]*m[10]) > epsilon) return false;
	if (fabs(m[1]*m[2] + m[5]*m[6] + m[9]*m[10]) > epsilon) return false;
	if (fabs(m[0]*m[0] + m[4]*m[4] + m[8]*m[8] - 1) > epsilon) return false;
	if (fabs(m[1]*m[1] + m[5]*m[5] + m[9]*m[9] - 1) > epsilon) return false;
	if (fabs(m[2]*m[2] + m[6]*m[6] + m[10]*m[10] - 1) > epsilon) return false;
	GLfloat determinant = m[0]*m[5]*m[10] + m[4]*m[9]*m[2] + m[8]*m[1]*m[6] - m[0]*m[9]*m[6] - m[4]*m[1]*m[10] - m[8]*m[5]*m[2];
	return (fabs(determinant-1) < epsilon);
}

