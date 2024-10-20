/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:48:05 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 17:56:47 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <gmp.h>
#include <cstdlib>
#include <ctime>

class BigInt {
public:
    mpz_t value;

    BigInt() {
        mpz_init(value);
    }

    BigInt(unsigned long int val) {
        mpz_init_set_ui(value, val);
    }

    BigInt(const BigInt &other) {
        mpz_init_set(value, other.value);
    }

    ~BigInt() {
        mpz_clear(value);
    }

    BigInt& operator=(const BigInt &other) {
        if (this != &other) {
            mpz_set(value, other.value);
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream &os, const BigInt &bigInt) {
        char *str = mpz_get_str(NULL, 10, bigInt.value);
        os << str;
        free(str);
        return os;
    }
};
