/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovoa-a <jnovoa-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 20:24:40 by jnovoa-a          #+#    #+#             */
/*   Updated: 2026/10/15 19:40:52 by jnovoa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <stdexcept>
#include <climits>
#include <cstdlib>
#include <ctime>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    *this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _vector = other._vector;
        _deque = other._deque;
    }
    return (*this);
}

PmergeMe::~PmergeMe()
{
}

void PmergeMe::parseArguments(int argc, char **argv)
{
    if(argc < 2)
        throw std::runtime_error("No numbers");
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '\0')
            throw std::runtime_error("Empty argument");
        for(int j = 0; argv[i][j] != '\0'; j++)
        {
            if(argv[i][j] < '0' || argv[i][j] > '9')
                throw std::runtime_error("No valid number");
        }
        //Convertimos el texto a numero entero
        long number = std::strtol(argv[i],NULL, 10);
        //Comprobamos que no sea mayor que INT
        if (number <= 0 || number > INT_MAX)
            throw std::runtime_error("No valid number");
        //Guardamos el numero en los dos contenedores
        //vector guarda los elemntos consecutivamente
        _vector.push_back(static_cast<int>(number));
        //deque guarda en bloques de memoria
        _deque.push_back(static_cast<int>(number));
        //Guardamos los numeros originales para mostrarlos despues
        _original.push_back(static_cast<int>(number));
    }
}

std::vector<std::pair<int, int> > PmergeMe::sortPairsVector(
    std::vector<std::pair<int, int> > pairs)
{
    //Si hay cero o una pareja ya esta ordenado
    if(pairs.size() <= 1)
        return pairs;
    //Guardamos los numeros mayores de cada pareja
    std::vector<int> maxima;
    for(std::size_t i = 0; i < pairs.size(); i++)
        maxima.push_back(pairs[i].second);
    //Ordenamos las numeros mauores con ford johnson
    std::vector<int> sortedMaxima = fordJohnsonVector(maxima);
    //Marcamos las parejas que ya hemos colocado
    std::vector<bool> used(pairs.size(), false);
    std::vector<std::pair<int, int> > sortedPairs;
    //Reconstruimos las parejas sin serparar sus elementos
    for(std::size_t i = 0; i < sortedMaxima.size(); i++)
    {
        for(std::size_t j = 0; j < pairs.size(); j++)
        {
            if(!used[j] && pairs[j].second == sortedMaxima[i])
            {
                sortedPairs.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    //Devolvemos las parejas ordenadas
    return sortedPairs;
}


/* Ordenamos los numeros utilizando Ford-Johson */
std::vector<int> PmergeMe::fordJohnsonVector(
    const std::vector<int> &numbers)
{
    //Si hay cero o un numero, ya esta ordenado
    if (numbers.size() <= 1)
        return (numbers);

    //Guardamos las parejas de numeros
    std::vector<std::pair<int, int> > pairs;

    //Guardamos el posible numero que quede suelto
    bool hasStraggler = false;
    int straggler = 0;

    //Agrupamos los numeros de dos en dos
    for (std::size_t i = 0; i + 1 < numbers.size(); i += 2)
    {
        int first = numbers[i];
        int second = numbers[i + 1];

        //Colocamos el menor antes del mayor
        if (first > second)
        {
            int temp = first;
            first = second;
            second = temp;
        }

        //Guardamos la pareja
        pairs.push_back(std::make_pair(first, second));
    }

    //Comprobamos si queda un numero sin pareja
    if (numbers.size() % 2 != 0)
    {
        hasStraggler = true;
        straggler = numbers[numbers.size() - 1];
    }

    //Ordenamos las parejas segun su numero mayor
    pairs = sortPairsVector(pairs);

    //Creamos la secuencia ordenada que iremos construyendo
    std::vector<int> chain;

    //Si hay parejas, empezamos con el menor de la primera
    if (!pairs.empty())
        chain.push_back(pairs[0].first);

    //Añadimos los numeros mayores de las parejas ordenadas
    for (std::size_t i = 0; i < pairs.size(); i++)
        chain.push_back(pairs[i].second);

    //Guardamos el orden en el que insertaremos los numeros pendientes
    std::vector<std::size_t> order;
    std::size_t boundary = 1;
    std::size_t previousJacobsthal = 1;
    std::size_t jacobsthal = 3;

    //Calculamos el orden de insercion usando numeros de Jacobsthal
    while (boundary < pairs.size())
    {
        std::size_t end = jacobsthal;

        //Evitamos superar el numero de parejas disponibles
        if (end > pairs.size())
            end = pairs.size();

        //Insertamos los indices de mayor a menor dentro de cada bloque
        for (std::size_t i = end; i > boundary; i--)
            order.push_back(i);

        //Actualizamos el limite del bloque
        boundary = end;

        //Si ya hemos llegado al final, terminamos
        if (jacobsthal >= pairs.size())
            break;

        //Calculamos el siguiente numero de Jacobsthal
        std::size_t next = jacobsthal + 2 * previousJacobsthal;
        previousJacobsthal = jacobsthal;
        jacobsthal = next;
    }

    //Insertamos los numeros menores siguiendo el orden calculado
    for (std::size_t i = 0; i < order.size(); i++)
    {
        //Obtenemos la pareja correspondiente
        std::size_t pairIndex = order[i] - 1;
        int value = pairs[pairIndex].first;

        //Buscamos donde esta el numero mayor de su pareja
        std::size_t partner = 0;
        while (partner < chain.size()
            && chain[partner] != pairs[pairIndex].second)
            partner++;

        //Limitamos la busqueda a la posicion de su pareja
        std::size_t low = 0;
        std::size_t high = partner;

        //Buscamos la posicion correcta mediante busqueda binaria
        while (low < high)
        {
            std::size_t middle = low + (high - low) / 2;

            if (chain[middle] < value)
                low = middle + 1;
            else
                high = middle;
        }

        //Insertamos el numero en la posicion encontrada
        chain.insert(chain.begin() + low, value);
    }

    //Insertamos el numero suelto si la cantidad era impar
    if (hasStraggler)
    {
        //Preparamos la busqueda en toda la secuencia
        std::size_t low = 0;
        std::size_t high = chain.size();

        //Buscamos su posicion mediante busqueda binaria
        while (low < high)
        {
            std::size_t middle = low + (high - low) / 2;

            if (chain[middle] < straggler)
                low = middle + 1;
            else
                high = middle;
        }

        //Insertamos el numero suelto en su posicion
        chain.insert(chain.begin() + low, straggler);
    }

    //Devolvemos todos los numeros ordenados
    return (chain);
}

//Ordenamos los numeros guardados en el vector
void PmergeMe::sortVector()
{
    //Guardamos el momento en el que empieza la ordenacion
    std::clock_t start = std::clock();

    //Guardamos los numeros ordenados que devuelve Ford-Johnson
    _vector = fordJohnsonVector(_vector);

    //Calculamos el tiempo que ha tardado la ordenacion
    std::clock_t end = std::clock();
    _vectorTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

//Ordenamos las parejas segun su numero mayor
std::deque<std::pair<int, int> > PmergeMe::sortPairsDeque(
    std::deque<std::pair<int, int> > pairs)
{
    //Si hay cero o una pareja, ya esta ordenado
    if (pairs.size() <= 1)
        return (pairs);

    //Guardamos los numeros mayores de cada pareja
    std::deque<int> maxima;
    for (std::size_t i = 0; i < pairs.size(); i++)
        maxima.push_back(pairs[i].second);

    //Ordenamos los numeros mayores con Ford-Johnson
    std::deque<int> sortedMaxima = fordJohnsonDeque(maxima);

    //Marcamos las parejas que ya hemos colocado
    std::vector<bool> used(pairs.size(), false);
    std::deque<std::pair<int, int> > sortedPairs;

    //Reconstruimos las parejas sin separar sus elementos
    for (std::size_t i = 0; i < sortedMaxima.size(); i++)
    {
        for (std::size_t j = 0; j < pairs.size(); j++)
        {
            if (!used[j] && pairs[j].second == sortedMaxima[i])
            {
                sortedPairs.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }

    //Devolvemos las parejas ordenadas
    return (sortedPairs);
}


//Ordenamos los numeros utilizando Ford-Johnson con deque
std::deque<int> PmergeMe::fordJohnsonDeque(
    const std::deque<int> &numbers)
{
    //Si hay cero o un numero, ya esta ordenado
    if (numbers.size() <= 1)
        return (numbers);

    //Guardamos las parejas de numeros
    std::deque<std::pair<int, int> > pairs;

    //Guardamos el posible numero que quede suelto
    bool hasStraggler = false;
    int straggler = 0;

    //Agrupamos los numeros de dos en dos
    for (std::size_t i = 0; i + 1 < numbers.size(); i += 2)
    {
        int first = numbers[i];
        int second = numbers[i + 1];

        //Colocamos el menor antes del mayor
        if (first > second)
        {
            int temp = first;
            first = second;
            second = temp;
        }

        //Guardamos la pareja
        pairs.push_back(std::make_pair(first, second));
    }

    //Comprobamos si queda un numero sin pareja
    if (numbers.size() % 2 != 0)
    {
        hasStraggler = true;
        straggler = numbers[numbers.size() - 1];
    }

    //Ordenamos las parejas segun su numero mayor
    pairs = sortPairsDeque(pairs);

    //Creamos la secuencia ordenada que iremos construyendo
    std::deque<int> chain;

    //Si hay parejas, empezamos con el menor de la primera
    if (!pairs.empty())
        chain.push_back(pairs[0].first);

    //Añadimos los numeros mayores de las parejas ordenadas
    for (std::size_t i = 0; i < pairs.size(); i++)
        chain.push_back(pairs[i].second);

    //Guardamos el orden en el que insertaremos los numeros pendientes
    std::vector<std::size_t> order;
    std::size_t boundary = 1;
    std::size_t previousJacobsthal = 1;
    std::size_t jacobsthal = 3;

    //Calculamos el orden de insercion usando numeros de Jacobsthal
    while (boundary < pairs.size())
    {
        std::size_t end = jacobsthal;

        //Evitamos superar el numero de parejas disponibles
        if (end > pairs.size())
            end = pairs.size();

        //Insertamos los indices de mayor a menor dentro de cada bloque
        for (std::size_t i = end; i > boundary; i--)
            order.push_back(i);

        //Actualizamos el limite del bloque
        boundary = end;

        //Si ya hemos llegado al final, terminamos
        if (jacobsthal >= pairs.size())
            break;

        //Calculamos el siguiente numero de Jacobsthal
        std::size_t next = jacobsthal + 2 * previousJacobsthal;
        previousJacobsthal = jacobsthal;
        jacobsthal = next;
    }

    //Insertamos los numeros menores siguiendo el orden calculado
    for (std::size_t i = 0; i < order.size(); i++)
    {
        //Obtenemos la pareja correspondiente
        std::size_t pairIndex = order[i] - 1;
        int value = pairs[pairIndex].first;

        //Buscamos donde esta el numero mayor de su pareja
        std::size_t partner = 0;
        while (partner < chain.size()
            && chain[partner] != pairs[pairIndex].second)
            partner++;

        //Limitamos la busqueda a la posicion de su pareja
        std::size_t low = 0;
        std::size_t high = partner;

        //Buscamos la posicion correcta mediante busqueda binaria
        while (low < high)
        {
            std::size_t middle = low + (high - low) / 2;

            if (chain[middle] < value)
                low = middle + 1;
            else
                high = middle;
        }

        //Insertamos el numero en la posicion encontrada
        chain.insert(chain.begin() + low, value);
    }

    //Insertamos el numero suelto si la cantidad era impar
    if (hasStraggler)
    {
        //Preparamos la busqueda en toda la secuencia
        std::size_t low = 0;
        std::size_t high = chain.size();

        //Buscamos su posicion mediante busqueda binaria
        while (low < high)
        {
            std::size_t middle = low + (high - low) / 2;

            if (chain[middle] < straggler)
                low = middle + 1;
            else
                high = middle;
        }

        //Insertamos el numero suelto en su posicion
        chain.insert(chain.begin() + low, straggler);
    }

    //Devolvemos todos los numeros ordenados
    return (chain);
}

//Ordenamos los numeros guardados en el deque
void PmergeMe::sortDeque()
{
    //Guardamos el momento en el que empieza la ordenacion
    std::clock_t start = std::clock();

    //Guardamos los numeros ordenados que devuelve Ford-Johnson
    _deque = fordJohnsonDeque(_deque);

    //Calculamos el tiempo que ha tardado la ordenacion
    std::clock_t end = std::clock();
    _dequeTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

//Mostramos los numeros originales, los ordenados y los tiempos
void PmergeMe::displayResults() const
{
    //Mostramos los numeros antes de ordenarlos
    std::cout << "Before: ";
    for (std::size_t i = 0; i < _original.size(); i++)
        std::cout << _original[i] << " ";
    std::cout << std::endl;

    //Mostramos los numeros despues de ordenarlos
    std::cout << "After:  ";
    for (std::size_t i = 0; i < _vector.size(); i++)
        std::cout << _vector[i] << " ";
    std::cout << std::endl;

    //Mostramos el tiempo utilizado por el vector
    std::cout << "Time to process a range of " << _vector.size()
        << " elements with std::vector : "
        << _vectorTime * 1000000.0 << " us" << std::endl;

    //Mostramos el tiempo utilizado por el deque
    std::cout << "Time to process a range of " << _deque.size()
        << " elements with std::deque : "
        << _dequeTime * 1000000.0 << " us" << std::endl;
}