/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovoa-a <jnovoa-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 17:36:26 by jnovoa-a          #+#    #+#             */
/*   Updated: 2026/10/13 18:27:17 by jnovoa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <stack>

class RPN
{
private:
std::stack<int> _stack;

public:
    RPN();
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);
    ~RPN();

    int calculate(const std::string &expression) const;

};

#endif