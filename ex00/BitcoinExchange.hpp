/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovoa-a <jnovoa-a@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 19:34:33 by jnovoa-a          #+#    #+#             */
/*   Updated: 2026/10/09 19:08:33 by jnovoa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
    private:
        //Guarda fecha -> precio
        std::map<std::string, double> _database;
    
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);
        ~BitcoinExchange();

        //Cargamos los datos del csv
        void loadDatabase(const std::string &filename);
        //Procesamos el archivo de entrada
        void processInput(const std::string &filename);
        bool isValidDate(const std::string &date) const;
        //Buscamos el precio de la fecha indicada
        double getExchangeRate(const std::string &date) const;
        
};


#endif