/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovoa-a <jnovoa-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 19:39:21 by jnovoa-a          #+#    #+#             */
/*   Updated: 2026/10/09 13:04:45 by jnovoa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : _database(other._database)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
        _database = other._database;
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
    //Abrimos el csv
    std::ifstream file(filename.c_str());
    //Comprobamos si lo hemos podido abrir
    if(!file.is_open())
        throw std::runtime_error("could not open database");
    std::string line;
    //Saltamos la primera linea, date,exchange_rate
    std::getline(file, line);
    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string date;
        std::string price;

        //Separamos fecha y precio por la coma
        if(std::getline(ss, date, ',') && std::getline(ss, price))
            _database[date] = std::atof(price.c_str());
    }
    
}

void BitcoinExchange::processInput(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        std::cerr << "Error: could not open file" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string date;
        std::string value;

        if(std::getline(ss, date, '|') && std::getline(ss, value))
        {
            // Quitamos los espacios de la fecha
            //dateStream copia el texto de date, y después leemos 
            //desde dateStream para volver a guardar el resultado en date
            std::stringstream dateStream(date);
            dateStream >> date;

            // Comprobamos si la fecha es valida
            if (!isValidDate(date))
            {
                std::cerr << "Error: bad input => " << date << std::endl;
                continue;
            }

        // Convertimos el valor a double
        std::stringstream valueStream(value);
        double amount;
        valueStream >> amount;

        std::cout << "Date: " << date << " Value: " << amount << std::endl;
        }
    }
}

bool BitcoinExchange::isValidDate(const std::string &date) const
{
    //Comprobamos que tenga 10 caracteres
    if(date.length() != 10)
        return false;
    if(date[4] != '-' || date[7] != '-')
        return false;
    for(int i = 0; i < 10; i++)
    {
        if(i == 4 || i == 7)
            continue;
        if(date[i] < '0' || date[i] > '9')
            return false;
    }
    //Obtenemos año, mes y dia
    int year = std::atoi(date.substr(0,4).c_str());
    int month = std::atoi(date.substr(5,2).c_str());
    int day = std::atoi(date.substr(8,2).c_str());

    if(month < 1 || month > 12)
        return false;
    if(day < 1 || day > 31)
        return false;
    if(month == 4 || month == 6 || month == 9 || month == 11 && day > 30)
        return false;
    //Comprobamos enero
    if(month == 2)
    {
        bool leap = (year % 4 == 0 && year % 100 != 0)
                    || year % 400 == 0;

        if (day > 28 + (leap ? 1 : 0))
            return false;
    }
    return true;
}