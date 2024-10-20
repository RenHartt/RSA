/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 07:30:58 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 18:06:01 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdint>
#include <iomanip>
#include <climits>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

__int128 gcd(__int128 a, __int128 b)
{
    while (b != 0)
    {
        __int128 tmp = b;
        b = a % b;
        a = tmp;
    }

    return (a);
}

__int128 mod_invert(__int128 e, __int128 phi)
{
    __int128 t = 0, new_t = 1;
    __int128 r = phi, new_r = e;

    while (new_r != 0)
    {
        __int128 quotient = r / new_r;

        __int128 temp_t = t;
        t = new_t;
        new_t = temp_t - quotient * new_t;

        __int128 temp_r = r;
        r = new_r;
        new_r = temp_r - quotient * new_r;
    }

    if (t < 0) t += phi;

    return (t);
}

__int128 power_mod(__int128 base, __int128 exp, __int128 mod)
{
	__int128 result = 1;

	base = base % mod;
	while (exp > 0)
	{
		if (exp % 2 == 1)
		{
			result = (result * base) % mod;
		}
		exp = exp >> 1;
		base = (base * base) % mod;
	}

	return (result);
}

bool is_prime(__int128 n, int k = 5)
{
    if (n < 4) return (n == 2 || n == 3);
    if (n % 2 == 0) return (false);

    __int128 s = 0, d = n - 1;
    while (d % 2 == 0)
	{
        d /= 2;
        s++;
    }

    for (int i = 0; i < k; i++)
	{
        __int128 a = 2 + rand() % (n - 3);
        __int128 x = power_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool continue_loop = false;
        for (__int128 r = 0; r < s - 1; r++)
		{
            x = power_mod(x, 2, n);
            if (x == n - 1)
			{
                continue_loop = true;
                break;
            }
        }

        if (!continue_loop) return (false);
    }

    return (true);
}

void generate_prime(__int128 &p, __int128 &q)
{
	__int128 min = (__int128)1 << (32 - 1);
	__int128 max = (__int128(1) << 32) - 1;


    do 
	{
        p = min + rand() % (max - min);
    } while (!is_prime(p));

    do
	{
        q = min + rand() % (max - min);
    } while (!is_prime(q) || p == q);
}

void generate_keys(__int128 &n, __int128 &e, __int128 &d, __int128 &p, __int128 &q)
{
    generate_prime(p, q);
    n = p * q;
    __int128 phi = (p - 1) * (q - 1);

    e = 65537;
    while (gcd(e, phi) != 1)
    {
        e += 2;
    }

    d = mod_invert(e, phi);
}

std::vector<__int128> encrypt_message(const std::string &message, __int128 e, __int128 n)
{
    std::vector<__int128> encrypted;
    
	for (__int128 i = 0; i < message.size(); i++)
    {
        encrypted.push_back(power_mod(message[i], e, n));
    }
    
	return (encrypted);
}

std::string decrypt_message(const std::vector<__int128> &encrypted, __int128 d, __int128 n)
{
    std::string decrypted;

    for (__int128 i = 0; i < encrypted.size(); i++)
    {
		decrypted += power_mod(encrypted[i], d, n);
    }

    return (decrypted);
}

std::ostream &operator<<(std::ostream& os, __int128 value)
{
    if (value == 0) return (os << "0");

    if (value < 0)
	{
        os << "-";
        value = -value;
    }

    uint64_t high = static_cast<uint64_t>(value >> 32);
    uint64_t low = static_cast<uint64_t>(value);

    if (high > 0)
	{
        os << high;
        os << std::setw(19) << std::setfill('0') << low;
    }
	else
	{
        os << low;
    }

    return (os);
}

int main(int argc, char **argv)
{
    srand(time(0));

	std::string message;
	message = argv[1];
	std::cout << "Message : " << message << std::endl;

    __int128 n, e, d, p, q;
	generate_keys(n, e, d, p, q);

	std::cout << "p = " << p << ", q = " << q << std::endl;
	std::cout << "Clé publique : (n = " << n << ", e = " << e << ")" << std::endl;
	std::cout << "Clé privée : (n = " << n << ", d = " << d << ")" << std::endl;

	std::vector<__int128> encrypted = encrypt_message(message, e, n);
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
