/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 07:30:58 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/19 19:09:41 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#define BITS 8

int gcd(int a, int b)
{
    while (b != 0)
    {
        int tmp = b;
        b = a % b;
        a = tmp;
    }

    return (a);
}

int mod_invert(int e, int phi)
{
    int t = 0, new_t = 1;
    int r = phi, new_r = e;

    while (new_r != 0)
    {
        int quotient = r / new_r;

        int temp_t = t;
        t = new_t;
        new_t = temp_t - quotient * new_t;

        int temp_r = r;
        r = new_r;
        new_r = temp_r - quotient * new_r;
    }

    if (t < 0) t += phi;

    return (t);
}

int power_mod(int base, int exp, int mod)
{
    int result = 1;

	for (int i = 0; i < exp; i++)
	{
        result = (result * base) % mod;
    }

    return (result);
}

bool is_prime(int number)
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

void generate_prime(int &p, int &q, int bits)
{
    int min = 1 << (bits - 1);
    int max = (1 << bits) - 1;

    do 
	{
        p = min + rand() % (max - min);
    } while (!is_prime(p));

    do
	{
        q = min + rand() % (max - min);
    } while (!is_prime(q) || p == q);
}

void generate_keys(int &n, int &e, int &d, int &p, int &q)
{
    generate_prime(p, q, BITS);
    n = p * q;
    int phi = (p - 1) * (q - 1);

    e = 65537;
    while (gcd(e, phi) != 1)
    {
        e += 2;
    }

    d = mod_invert(e, phi);
}

std::vector<int> encrypt_message(const std::string &message, int e, int n)
{
    std::vector<int> encrypted;
    
	for (std::size_t i = 0; i < message.size(); i++)
    {
        encrypted.push_back(power_mod(message[i], e, n));
    }
    
	return (encrypted);
}

std::string decrypt_message(const std::vector<int> &encrypted, int d, int n)
{
    std::string decrypted;
    for (std::size_t i = 0; i < encrypted.size(); i++)
    {
		decrypted += power_mod(encrypted[i], d, n);
    }
    return (decrypted);
}

int main(int argc, char **argv)
{
    srand(time(0));

	std::string message;
	if (argc == 2)
	{
		message = argv[1];
	}
	else
	{
		std::string line;
		while (std::getline(std::cin, line))
		{
			message += line;
		}
		std::cout << "Message : " << message << std::endl;
	}

    int n, e, d, p, q;
	generate_keys(n, e, d, p, q);

	std::cout << "p = " << p << ", q = " << q << std::endl;
	std::cout << "Clé publique : (n = " << n << ", e = " << e << ")" << std::endl;
	std::cout << "Clé privée : (n = " << n << ", d = " << d << ")" << std::endl;

	std::vector<int> encrypted = encrypt_message(message, e, n);
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
