
#include <GL/glut.h>
#include <stdio.h>
#include "Point.h"
#include "Vector.h"

#ifndef MATRIX_H
#define MATRIX_H 1

#define ERROR_TOLERANCE 1E-7

// now declare a matrix class for homogeneous matrices
class Matrix
	{ // class Matrix
	public:
	// remember: this is in column-major form
	GLfloat coords[16];

	// returns an identity matrix
	static Matrix Identity();
	
	// returns a zero matrix
	static Matrix Zero();
	
	// returns a specified element in the matrix
	GLfloat &Element(int row, int col);
	
	// Copy another Matrix & return self
	Matrix &operator = (Matrix otherMatrix);

	// Add another Matrix
	Matrix operator + (Matrix otherMatrix);

	// Subtract another Matrix
	Matrix operator - (Matrix otherMatrix);

	// Scale the matrix by a constant 
	Matrix operator * (GLfloat scaleFactor);
	
	// Left-multiply a Point
	Point operator *(Point point);
	
	// Left-multiply a Vector
	Vector operator *(Vector vector);
	
	// Right-multiply by another matrix
	Matrix operator *(Matrix otherMatrix);

	// Scales a row by a given factor
	void ScaleRow(int row, GLfloat scaleFactor);

	// Swaps two rows
	void SwapRows(int row1, int row2);

	// Adds multiple of one row to another row
	void AddRows(int row1, GLfloat multiple, int row2);

	// Reduces all elements in a column except the diagonal element
	// Returns 0 on success, 1 if the column cannot be reduced
	int ReduceColumn(int column, Matrix &inverse);

	// Computes a matrix inverse with brute force Gauss-Jordan method
	// returns a zero matrix on failure
	Matrix Inverse();

	// routines to extract the translation part of the matrix
	// assumes no entries in perspective section of matrix
	Vector TranslationVector();
	Matrix TranslationMatrix();

	// routine to extract the rotation part of the matrix
	// assumes no entries in perspective section of matrix
	Matrix RotationMatrix();

	// routine to read it in
	// returns 0 on success, non-0 on failure
	long Read(FILE *inFile);	
	
	// routine to print it out
	void Print(FILE *outFile);	
	
	void printToConsole();
	
	bool testMatrixValidity(GLfloat *m);
	}; // class Matrix

// Scale the matrix by a constant 
Matrix operator * (GLfloat scaleFactor, Matrix matrix);

#endif
