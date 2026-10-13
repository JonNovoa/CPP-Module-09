/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovoa-a <jnovoa-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 17:43:57 by jnovoa-a          #+#    #+#             */
/*   Updated: 2026/10/13 19:22:56 by jnovoa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <sstream>
#include <stdexcept>

RPN::RPN()
{
}

RPN::RPN(const RPN &other)
    : _stack(other._stack)
{
}

RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
        _stack = other._stack;
    return *this;
}

RPN::~RPN()
{
}

int RPN::calculate(const std::string &expression) const
{
    std::stack<int> numbers;
    //Convertimos la expresion(argv) en un flujo para leerla
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token)
    {
        if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            if (numbers.size() < 2)
                throw std::runtime_error("Error");
            //Guardamos el de la derecha y lo eliminamos
            int right = numbers.top();
            numbers.pop();
            //Guardamos el de la izquierda y lo eliminamos
            int left = numbers.top();
            numbers.pop();

            if (token == "+")
                numbers.push(left + right);
            else if (token == "-")
                numbers.push(left - right);
            else if (token == "*")
                numbers.push(left * right);
            else
            {
                if (right == 0)
                    throw std::runtime_error("Error");
                numbers.push(left / right);
            }
        }
        else
        {
            if(token.length() != 1 || token[0] < '0' || token[0] > '9')
                throw std::runtime_error("Error");
            numbers.push(token[0] - '0');
        }
    }
    if(numbers.size() != 1)
        throw std::runtime_error("Error");
    return (numbers.top());
}