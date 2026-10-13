/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovoa-a <jnovoa-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 19:38:46 by jnovoa-a          #+#    #+#             */
/*   Updated: 2026/10/13 19:43:10 by jnovoa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    try
    {
        RPN calculator;
        std::cout << calculator.calculate(argv[1]) << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}