#ifndef TESTSCENARIOS_H_INCLUDED
#define TESTSCENARIOS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include "ECU_variable.h"
#include <algorithm>

enum EnumSearchEle
{
    min_value,
    max_value
};


int check_T01_comment (std::string& one_line_from_xml_file);
int check_T02_one_comment(std::string& one_line_from_xml_file);
int check_T03_semicolon(std::string& one_line_from_xml_file);
int check_T04_extract_instruction(std::string& one_line_from_xml_file,std::string& instruction);
int check_T05_get_var_properties(std::string& instruction,ECU_variable& tmp_variable);
int check_T07_extract_comment(std::string& one_line_from_xml_file,std::string& str_comment);
int check_T06_extract_thres(std::string& str_comment,ECU_variable& tmp_variable, EnumSearchEle element);

#endif // TESTSCENARIOS_H_INCLUDED
