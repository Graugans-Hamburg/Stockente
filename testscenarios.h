/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with CCP driver.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: Sep 9, 2017
 *      Author: Matthias Baumann
 */

#ifndef TESTSCENARIOS_H_INCLUDED
#define TESTSCENARIOS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include "ECU_variable.h"
#include <algorithm>

const char character_allowed_as_first_letter[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char character_allowed_for_variables[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";

enum EnumSearchEle
{
    min_value,
    max_value,
    unit_value,
    des_value
};


int check_T01_comment (std::string& one_line_from_xml_file);
int check_T02_one_comment(std::string& one_line_from_xml_file);
int check_T03_semicolon(std::string& one_line_from_xml_file);
int check_T04_extract_instruction(std::string& one_line_from_xml_file,std::string& instruction);
int check_T05_get_var_properties(std::string& instruction,ECU_variable& tmp_variable);
int check_T07_extract_comment(std::string& one_line_from_xml_file,std::string& str_comment);
int check_T06_extract_thres(std::string& str_comment,ECU_variable& tmp_variable, EnumSearchEle element);
int check_T08_extract_str_element(std::string& str_comment,ECU_variable& tmp_variable, EnumSearchEle element);

bool check_if_a_letter_is_right_before(std::string& line,std::string::size_type pos_tmp);
bool check_if_a_letter_is_right_after(std::string& line,std::string::size_type pos_tmp);

#endif // TESTSCENARIOS_H_INCLUDED
