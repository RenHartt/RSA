/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BigInt.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 18:14:18 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 18:15:55 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BigInt.hpp"
#include <cstdlib>

BigInt::BigInt() { mpz_init(value); }
BigInt::BigInt(unsigned long int val) { mpz_init_set_ui(value, val); }
BigInt::BigInt(const BigInt &other) { mpz_init_set(value, other.value); }
BigInt::~BigInt() { mpz_clear(value); }

BigInt& BigInt::operator=(const BigInt &other)
{
    if (this != &other) 
	{
        mpz_set(value, other.value);
    }
    
	return (*this);
}

std::string BigInt::to_string() const
{
    char *str = mpz_get_str(NULL, 10, value);
    std::string result(str);
    free(str);

	return (result);
}

std::ostream& operator<<(std::ostream &os, const BigInt &bigInt)
{
    os << bigInt.to_string();

	return (os);
}
