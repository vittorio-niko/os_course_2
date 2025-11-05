#pragma once

#include "Number.h"

#ifdef _WIN32
#ifdef VECTORLIBRARY_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif
#elif __linux__
#define VECTOR_API __attribute__((visibility("default")))
#else
#define VECTOR_API
#endif


namespace VectorLibrary {
	using namespace NumberLibrary;
	class VECTOR_API Vector {
	public:
		Vector(const Number& x_comp, const Number& y_comp);
		Vector();

		Vector operator + (const Vector& other) const;
		Number operator * (const Vector& other) const; //scalar

		double getR() const;
		double getPhi() const;

	private:
		static const Vector ZERO_VECTOR;
		static const Vector UNIT_VECTOR;

	private:
		Number x_comp;
		Number y_comp;
	};
}

