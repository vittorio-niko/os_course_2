#pragma once

namespace NumberLibrary {
	class Number {
	public:
		Number(long long int number);
		Number();
		long long int getRawNumber() const;

		//Operators
		Number operator + (const Number& other) const;
		Number operator - (const Number& other) const;
		Number operator * (const Number& other) const;
		Number operator / (const Number& other) const;
		Number operator % (const Number& other) const;

		//Gcd and Lcm
		Number GCD(const Number& number) const;
		Number LCM(const Number& number) const;

	public:
		static constexpr long long int ONE = 1;
		static constexpr long long int ZERO = 0;

	private:
		static bool willMultiplicationOverflow(long long int a, long long int b);
		static bool willAdditionOverflow(long long int a, long long int b);
		static bool willSubtractionOverflow(long long int a, long long int b);
		static bool willDivisionOverflow(long long int a, long long int b);

	private:
		long long int number;
	};
}
