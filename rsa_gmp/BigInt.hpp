/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BigInt.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:48:05 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 18:16:03 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <gmp.h>

class BigInt
{
public:
    mpz_t value;

    BigInt();
    BigInt(unsigned long int val);
    BigInt(const BigInt &other);
    ~BigInt();

    BigInt& operator=(const BigInt &other);

    std::string to_string() const;
};

std::ostream& operator<<(std::ostream &os, const BigInt &bigInt);
