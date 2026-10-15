/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovoa-a <jnovoa-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 20:24:29 by jnovoa-a          #+#    #+#             */
/*   Updated: 2026/10/15 14:38:40 by jnovoa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <utility>

class PmergeMe
{
    private:
        std::vector<int> _vector;
        std::deque<int> _deque;

        //Guardamos los numeros originales
        std::vector<int> _original;

        //Guardamos el tiempo de cada ordenacion
        double _vectorTime;
        double _dequeTime;

        //Ordenamos los numeros utilizando Ford-Johnson con vector
        std::vector<int> fordJohnsonVector(const std::vector<int> &numbers);

        //Ordenamos las parejas segun su numero mayor
        std::vector<std::pair<int, int> > sortPairsVector(
            std::vector<std::pair<int, int> > pairs);

        //Ordenamos los numeros utilizando Ford-Johnson con deque
        std::deque<int> fordJohnsonDeque(const std::deque<int> &numbers);

        //Ordenamos las parejas segun su numero mayor
        std::deque<std::pair<int, int> > sortPairsDeque(
            std::deque<std::pair<int, int> > pairs);

    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void parseArguments(int argc, char **argv);
        void sortVector();
        void sortDeque();
        void displayResults() const;
};

#endif