/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 07:30:58 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/19 11:11:36 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long tmp = b;
        b = a % b;
        a = tmp;
    }

    return (a);
}

long long mod_invert(long long e, long long phi)
{
    long long t = 0, new_t = 1;
    long long r = phi, new_r = e;

    while (new_r != 0)
    {
        long long quotient = r / new_r;

        long long temp_t = t;
        t = new_t;
        new_t = temp_t - quotient * new_t;

        long long temp_r = r;
        r = new_r;
        new_r = temp_r - quotient * new_r;
    }

    if (t < 0) t += phi;

    return (t);
}

long long power_mod(long long base, long long exp, long long mod)
{
    long long result = 1;

	for (long long i = 0; i < exp; i++)
	{
        result = (result * base) % mod;
    }

    return (result);
}

bool is_prime(long long number)
{
    if (number <= 1) return (false);
    if (number == 2) return (true);
    if (number % 2 == 0) return (false);

    for (int i = 3; i * i <= number; i += 2)
    {
        if (number % i == 0) return (false);
    }

    return (true);
}

void generate_prime(long long &p, long long &q, int bits)
{
    long long min = 1LL << (bits - 1);
    long long max = (1LL << bits) - 1;

    do 
	{
        p = min + rand() % (max - min);
    } while (!is_prime(p));

    do
	{
        q = min + rand() % (max - min);
    } while (!is_prime(q) || p == q);
}

void generate_keys(long long &n, long long &e, long long &d, long long &p, long long &q)
{
    generate_prime(p, q, 8);
    n = p * q;
    long long phi = (p - 1) * (q - 1);

    e = 3;
    while (gcd(e, phi) != 1)
    {
        e += 2;
    }

    d = mod_invert(e, phi);
}

long long encrypt_char(long long message, long long e, long long n)
{
    return power_mod(message, e, n);
}

long long decrypt_char(long long encrypted, long long d, long long n)
{
    return power_mod(encrypted, d, n);
}

std::vector<long long> encrypt_message(const std::string &message, long long e, long long n)
{
    std::vector<long long> encrypted;
    
	for (std::size_t i = 0; i < message.size(); i++)
    {
        encrypted.push_back(encrypt_char(message[i], e, n));
    }
    
	return (encrypted);
}

std::string decrypt_message(const std::vector<long long> &encrypted, long long d, long long n)
{
    std::string decrypted;
    for (std::size_t i = 0; i < encrypted.size(); i++)
    {
		decrypted += decrypt_char(encrypted[i], d, n);
    }
    return (decrypted);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage : ./rsa [message]" << std::endl;
        return (1);
    }

    srand(time(0));

    long long n, e, d, p, q;
    generate_keys(n, e, d, p, q);

    std::cout << "p = " << p << ", q = " << q << std::endl;
    std::cout << "Clé publique : (n = " << n << ", e = " << e << ")" << std::endl;
    std::cout << "Clé privée : (n = " << n << ", d = " << d << ")" << std::endl;

    std::string message = argv[1];
    std::cout << "Message : " << message << std::endl;

    std::vector<long long> encrypted = encrypt_message(message, e, n);

    std::cout << "Message chiffré : ";
    for (std::size_t i = 0; i < encrypted.size(); ++i)
    {
        std::cout << encrypted[i] << " ";
    }
    std::cout << std::endl;

    std::string decrypted = decrypt_message(encrypted, d, n);
    std::cout << "Message déchiffré : " << decrypted << std::endl;

    return (0);
}
