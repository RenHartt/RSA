/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 07:30:58 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 18:25:18 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BigInt.hpp"
#include <vector>
#include <ctime>

void generate_prime(mpz_t prime, int bits)
{
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, std::time(0));

    mpz_urandomb(prime, state, bits);
    mpz_nextprime(prime, prime);

    gmp_randclear(state);
}

void generate_keys(mpz_t n, mpz_t e, mpz_t d, mpz_t p, mpz_t q)
{
    int bits = 1024;
	do
	{
		generate_prime(p, bits);
		generate_prime(q, bits);
	} while (mpz_cmp(p, q) == 0);

    mpz_mul(n, p, q);

    mpz_t phi, p1, q1;
    mpz_inits(phi, p1, q1, NULL);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    mpz_set_ui(e, 65537);
    while (mpz_gcd_ui(NULL, e, mpz_get_ui(phi)) != 1)
	{
        mpz_add_ui(e, e, 2);
    }

    mpz_invert(d, e, phi);

    mpz_clears(phi, p1, q1, NULL);
}

std::vector<BigInt> encrypt_message(const std::string &message, mpz_t e, mpz_t n)
{
    std::vector<BigInt> encrypted;

    for (std::size_t i = 0; i < message.size(); ++i)
	{
        char c = message[i];
        BigInt enc_char(static_cast<unsigned long>(c));
        mpz_powm(enc_char.value, enc_char.value, e, n);
        encrypted.push_back(enc_char);
    }
    
	return (encrypted);
}

std::string decrypt_message(const std::vector<BigInt> &encrypted, mpz_t d, mpz_t n)
{
    std::string decrypted;

    for (std::vector<BigInt>::const_iterator it = encrypted.begin(); it != encrypted.end(); ++it)
	{
        const BigInt &enc_char = *it;
        BigInt dec_char;
        mpz_powm(dec_char.value, enc_char.value, d, n);
        decrypted += static_cast<char>(mpz_get_ui(dec_char.value));
    }
 
	return (decrypted);
}

int main(int argc, char **argv)
{
    if (argc != 2)
	{
        std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
        return (1);
    }

    std::string message = argv[1];
    std::cout << "Message : " << message << std::endl;

    mpz_t n, e, d, p, q;
    mpz_inits(n, e, d, p, q, NULL);

    generate_keys(n, e, d, p, q);

    std::cout << "p = " << p << ", q = " << q << std::endl;
    std::cout << "Clé publique : (n = " << n << ", e = " << e << ")" << std::endl;
    std::cout << "Clé privée : (n = " << n << ", d = " << d << ")" << std::endl;

    std::vector<BigInt> encrypted = encrypt_message(message, e, n);
    std::cout << "Message chiffré : ";
    for (std::vector<BigInt>::const_iterator it = encrypted.begin(); it != encrypted.end(); ++it)
	{
        const BigInt &c = *it;
        std::cout << c << " ";
    }
    std::cout << std::endl;

    std::string decrypted = decrypt_message(encrypted, d, n);
    std::cout << "Message déchiffré : " << decrypted << std::endl;

    mpz_clears(n, e, d, p, q, NULL);

    return (0);
}
