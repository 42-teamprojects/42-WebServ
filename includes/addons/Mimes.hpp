/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:57:54 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/29 10:36:10 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

class Mimes
{
    private:
        std::map<std::vector<std::string>, std::string>	mimes;

    public:
        Mimes();
        ~Mimes();
        void print();
        std::string operator[](std::string const &key);
};
