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

#include "testscenarios.h"

/*******************************************************************************************
 *
 * Function 01
 *
 ******************************************************************************************/
int check_T01_comment (std::string& one_line_from_xml_file)
{

       /* It is checked if one line has at least one complete comment*/
        std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
        std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");

        if(    pos_start_comment == std::string::npos
            || pos_end_comment   == std::string::npos)
        {
            std::cerr << "Error[-101]: The tag of the start of a comment or the tag for the end of"
                      << "a comment had not been found." << std::endl;
            return -101;
        }
        // It is checked if one line does have two comments
        std::string::size_type pos_second_start_comment = one_line_from_xml_file.find("/*", pos_start_comment +1);
        std::string::size_type pos_second_end_comment = one_line_from_xml_file.find("*/", pos_end_comment +1);
        if(    pos_second_start_comment != std::string::npos
            || pos_second_end_comment   != std::string::npos)
        {
            std::cerr << "Error[-103]:More than one tag for for starting or ending a comment had"
                      << " been found." << std::endl;

            return -102;

        }

        return 0; // Test successfull

}

/*******************************************************************************************
 *
 * Function 02
 *
 ******************************************************************************************/


int check_T02_one_comment(std::string& one_line_from_xml_file)
{
    /*
     * It is checked whether the comment is located at the end of the line.
     */

    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
    std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");
    size_t number_of_character = one_line_from_xml_file.length();
    for(size_t pos_idx = pos_end_comment + 2 ; pos_idx < number_of_character; pos_idx++)
    {
        if(    (one_line_from_xml_file.at(pos_idx) !=' ' )
            && (one_line_from_xml_file.at(pos_idx) !='\t') )
        {

            std::cerr << "Error[-201]: It had been checked if additional characters are located"
                      << "in the same line fo the comment and right behind the tag for ending the"
                      << " comment (only space and tab are allowed after the end of the comment)."
                      << std::endl;
            return -201;
        }

    }

    return 0;
}

/*******************************************************************************************
 *
 * Function 03
 *
 ******************************************************************************************/

int check_T03_semicolon(std::string& one_line_from_xml_file)
{
    /*
     * It shall be checked if the a semikolon is located before the start tag of the comment
     * Furthermore shall also be checked if the there is only one semikolon in the line.
     */
    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
    std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");
    size_t pos_semicolon = one_line_from_xml_file.find(";");
    if(pos_semicolon == std::string::npos)
    {

        std::cerr << "Error[-301]:No semikolon had been found in the line of the $CCP tag. The tag "
                  << "can therefore not be referenced to a variable." << std::endl;
        return -301;
    }

    // Check if the semikolon is located before the comment
    if(pos_semicolon > pos_start_comment)
    {
        std::cerr << "Error[-302]:The first semikolon is located inside the comment."
                  << "The CCP variable information can't be extracted." << std::endl;
        return -302;
    }
    // Check of another semikolon in the same line
    pos_semicolon = one_line_from_xml_file.find(";", pos_semicolon +1);
    if(pos_semicolon != std::string::npos)
    {
        std::cerr << "Error[-303]:More than one semikolon had been found in the same line. Two"
                  << "instructions in one line can not be checked by Stockente." << std::endl;
        return -303;
    }

    return 0;
}

/*******************************************************************************************
 *
 * Function 04 - extract the instruction
 *
 ******************************************************************************************/

int check_T04_extract_instruction(std::string& one_line_from_xml_file,std::string& instruction)
{
    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");

    instruction = one_line_from_xml_file.substr(0,pos_start_comment);

    return 0;
}

/*******************************************************************************************
 *
 * Function 05 - find the out the datatype
 *
 ******************************************************************************************/

 int check_T05_get_var_properties(std::string& instruction,ECU_variable& tmp_variable)
{
    int datatypes_found=0;
    std::string::size_type pos_tmp, pos_datatype, pos_variablename, pos_endvarname, pos_semicolon;

    pos_tmp = instruction.find("uint8_t ");
    if(pos_tmp != std::string::npos)
    {
        if(check_if_a_letter_is_right_before(instruction,pos_tmp) == false)
        {
            datatypes_found++;
            pos_datatype = pos_tmp;
            tmp_variable.SetDataType(type_u8);
        }
    }

    pos_tmp = instruction.find("int8_t ");
    if(pos_tmp != std::string::npos)
    {
        if(check_if_a_letter_is_right_before(instruction,pos_tmp) == false)
        {
            datatypes_found++;
            pos_datatype = pos_tmp;
            tmp_variable.SetDataType(type_i8);
        }
    }

    pos_tmp = instruction.find("uint16_t ");
    if(pos_tmp != std::string::npos)
    {
        if(check_if_a_letter_is_right_before(instruction,pos_tmp) == false)
        {
            datatypes_found++;
            pos_datatype = pos_tmp;
            tmp_variable.SetDataType(type_u16);
        }
    }

    pos_tmp = instruction.find("int16_t ");
    if(pos_tmp != std::string::npos)
    {
        if(check_if_a_letter_is_right_before(instruction,pos_tmp) == false)
        {
            datatypes_found++;
            pos_datatype = pos_tmp;
            tmp_variable.SetDataType(type_i16);
        }
    }

    pos_tmp = instruction.find("uint32_t ");
    if(pos_tmp != std::string::npos)
    {
        if(check_if_a_letter_is_right_before(instruction,pos_tmp) == false)
        {
            datatypes_found++;
            pos_datatype = pos_tmp;
            tmp_variable.SetDataType(type_u32);
        }
    }

    pos_tmp = instruction.find("int32_t ");
    if(pos_tmp != std::string::npos)
    {
        if(check_if_a_letter_is_right_before(instruction,pos_tmp) == false)
        {
            datatypes_found++;
            pos_datatype = pos_tmp;
            tmp_variable.SetDataType(type_i32);
        }
    }

    pos_tmp = instruction.find("float ");
    if(pos_tmp != std::string::npos)
    {
        if(check_if_a_letter_is_right_before(instruction,pos_tmp) == false)
        {
            datatypes_found++;
            pos_datatype = pos_tmp;
            tmp_variable.SetDataType(type_f32);
        }
    }

    if(datatypes_found == 0)
    {
        std::cerr << "Error[-501]:Datatype could not be found inside the instruction" << std::endl;
        return -501;
    }
     if(datatypes_found > 1)
    {
        std::cerr << "Error[-502]:To many datatypes had been found inside the instruction"
                  << std::endl;
        return -502;
    }

    //Find the location of the space character which is right befind the datatype.

    pos_variablename = instruction.find(" ",pos_datatype + 1);
    pos_variablename = instruction.find_first_of(character_allowed_as_first_letter,pos_variablename);

    if(pos_variablename == std::string::npos)
    {
        std::cerr << "Error[-503]:Die Position des Variablennames konnte nicht in der Anweisung gefunden werden." << std::endl;
        return -503;
    }


    /*
     * Find the end of the variable name. right behind the variable name is always one of the
     * following signs:
     *
     *  ;  [  =  Space '\t'
     */
    pos_semicolon = instruction.find(";",pos_variablename);
    if(pos_semicolon == std::string::npos)
    {
        std::cerr << "Error[-504]:The instruction does not contain a semikolon." << std::endl;
        return -504;
    }

    std::string::size_type pos_array = instruction.find("[",pos_variablename);
    if(pos_array !=  std::string::npos)
    {
        std::cerr << "Error[-505]:The CCP variable found is defining an array which is "
                  << " currently not supported." << std::endl;
        return -505;
    }

    pos_endvarname = instruction.find_first_not_of(character_allowed_for_variables,pos_variablename);

    if(pos_endvarname == std::string::npos)
    {
        std::cerr << "Error[-506]:Das Ende des Variablennamens konnte nicht bestimmt werden."
                  << std::endl;
        return -506;
    }
    else
    {
        pos_endvarname = pos_endvarname -1;
    }

    if(pos_endvarname < pos_variablename)
    {
        std::cerr << "Error[-507]:Something strange happens the variable name ends before it"
                  << " starts. Magic!" << std::endl;
        return -507;
    }

    tmp_variable.SetName(instruction.substr(pos_variablename,pos_endvarname-pos_variablename+1));

    return 0;
}

/*******************************************************************************************
 *
 * Function 06 - find out if the min or max value
 *
 ******************************************************************************************/

int check_T06_extract_thres(std::string& str_comment,ECU_variable& tmp_variable, EnumSearchEle element)
{

    std::string::size_type pos_start_min = str_comment.find("min:");
    std::string::size_type pos_start_max = str_comment.find("max:");
    std::string::size_type pos_start_des = str_comment.find("des-");
    std::string::size_type pos_search_element;
    switch(element)
    {
        case min_value:
            pos_search_element = pos_start_min;
            break;
        case max_value:
            pos_search_element = pos_start_max;
            break;
        default:
            std::cerr << "Error[-602]:Program failure. Wrong value handed over for the "
                      << "third element."<< std::endl;
            return -603;
    }

    if(pos_search_element == std::string::npos)
    {
        // The tag "min:" could not be fouond.
        return -601;
    }
    std::string::size_type pos_start_value = str_comment.find_first_of("-1234567890.",pos_search_element);
    std::string::size_type pos_end_value = str_comment.find_first_not_of("-1234567890.",pos_start_value);

    // TODO different checks need to be added.
    if( pos_start_value == std::string::npos || pos_end_value == std::string::npos)
    {
        std::cerr << "Error[-602]:The start or the end of the string, describing the value, could"
                  << "not be found." << std::endl;
        return -602;
    }

    std::string str_value = str_comment.substr(pos_start_value,pos_end_value - pos_start_value);

    try
    {
        std::stoll(str_value);
        std::stof(str_value);
    }
    catch(...)
    {
        std::cerr << "Error[-604]:The string which should be the min or max value could not be "
                  << "converted into a number." << std::endl;
        return -604;
    }

    switch(element)
    {
        case min_value:
            tmp_variable.SetMinValue_Int(std::stoll(str_value));
            tmp_variable.SetMinValue_Fl(std::stof(str_value));
            tmp_variable.str_MinValue = str_value;
            break;
        case max_value:
            tmp_variable.SetMaxValue_Int(std::stoll(str_value));
            tmp_variable.SetMaxValue_Fl(std::stof(str_value));
            tmp_variable.str_MaxValue = str_value;
            break;
        default:
            std::cerr << "Error[-603]:Programming failure" << std::endl;
            return -603;
    }



    return 0;
}

/*******************************************************************************************
 *
 * Function 07 - extract the comment
 *
 ******************************************************************************************/

int check_T07_extract_comment(std::string& one_line_from_xml_file,std::string& str_comment)
{
    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
    std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");
    if(pos_start_comment == std::string::npos)
    {
        std::cerr << "Error[-701]:No Tag found describing the start of the comment."<< std::endl;
        return -701;
    }
    if(pos_end_comment == std::string::npos)
    {
        std::cerr << "Error[-702]:No Tag found describing the e of the comment."<< std::endl;
        return -702;
    }
    // The +2 at the end of the next source code line is use to include the tag for the end of the
    // comment.
    str_comment = one_line_from_xml_file.substr(pos_start_comment,pos_end_comment-pos_start_comment+2);

    return 0;
}

/*******************************************************************************************
 *
 * Function 08 - find out the unit or the description
 *
 ******************************************************************************************/

int check_T08_extract_str_element(std::string& str_comment,ECU_variable& tmp_variable, EnumSearchEle element)
{

    std::string::size_type pos_start_unit = str_comment.find("unit:");
    std::string::size_type pos_start_des = str_comment.find("des:");
    std::string::size_type pos_search_element;
    switch(element)
    {
        case unit_value:
            pos_search_element = pos_start_unit;
            break;
        case des_value:
            pos_search_element = pos_start_des;
            break;
        default:
            // Programming failure the wrong value had been handed over as the third element.
            std::cerr << "Error[-803]: function call with wrong element. Only the enum unit_value or"
                      << "des_value makes sense" << std::endl;
            return -803;
    }

    if(pos_search_element == std::string::npos)
    {
        // The tag "min:" could not be found.
        return -801;
    }
    std::string::size_type pos_start_value = str_comment.find_first_of('"',pos_search_element);
    pos_start_value = pos_start_value +1; //First char beginns with a '
    std::string::size_type pos_end_value = str_comment.find_first_of('"',pos_start_value);

    // TODO add additional checks
    if( pos_start_value == std::string::npos || pos_end_value == std::string::npos)
    {
        // Der Anfang oder das Ende der Zeichenfolge, welche den Zahlenwert beschreibt konnte
        // nicht gefunden werden.
        std::cerr << "Error[-802]:No high comma which should be located before and after the  Unit/Des."
                  << std::endl;
        return -802;
    }

    std::string str_value = str_comment.substr(pos_start_value,pos_end_value - pos_start_value);

    switch(element)
    {
        case unit_value:
            tmp_variable.SetUnit(str_value);
            break;
        case des_value:
            tmp_variable.SetDescription(str_value);
            break;
        default:
            std::cerr << "Error[-803]: function call with wrong element. Only the enum unit_value or"
                      << "des_value makes sense" << std::endl;
            return -803;
    }
    return 0;
}

/*******************************************************************************************
 *
 * Function 09 - Check if there is a letter (or any other printable character) right before
 *               the position that is handed over as the second argument.
 *
 *               The function can
 *               be used to determine if a string is on its own or whether it is a part of
 *               longer string. Example: If you want to find the data type
 *               then you can search for int8_t but the same string is also contained in
 *               uint8_t. Both cases will match the string int8_t. This check will allow to
 *               determine if there is any letter before the string or if it is just a
 *               space or tab.
 *
 ******************************************************************************************/

bool check_if_a_letter_is_right_before(std::string& line,std::string::size_type pos_tmp)
{
    if(pos_tmp == 0)
    {
        /* Check it the position points to the first letter of the row, if so then it there is no
           leading letter */
        return false;
    }
    if(pos_tmp >= 1)
    {
        /* If the position is not pointing to the first charater of the row, then it need to be
           checked if the letter before the position is a space or a tab-sign.  */
        if(   (line.at(pos_tmp - 1) ==' ')
           || (line.at(pos_tmp - 1) =='\t'))
        {
            return false;
        }
    }
    /* If both checked before fail, than a printable character must be right before the position*/
    return true;
}


/*******************************************************************************************
 *
 * Function 10 - Check if there is a letter (or any other printable character) right after
 *               the position that is handed over as the second argument.
 *
 *               The function can
 *               be used to determine if a string is on its own or whether it is a part of
 *               longer string. Example: If you want to find a variable name
 *               then you can search for test_var but the same string can also be contained in
 *               test_var2. Both cases will match the string test_var. This check will allow to
 *               determine if there is any letter after the string or if it is just a
 *               space or tab.
 *
 ******************************************************************************************/

bool check_if_a_letter_is_right_after(std::string& line,std::string::size_type pos_tmp)
{
    if(pos_tmp + 1 == line.length())
    {
        /* Check it the position points to the last letter of the line, if so then it there is no
           following letter. 1 is added to the postion as the position counter starts to count with
           with 0. */
        return false;
    }
    if(pos_tmp + 1 < line.length())
    {
        /* If the position is not pointing to the last charater of the row, then it need to be
           checked if the letter after the position is a space or a tab-sign.  */
        if(   (line.at(pos_tmp + 1) ==' ')
           || (line.at(pos_tmp + 1) =='\t'))
        {
            return false;
        }
    }
    /* If both checked before fail, then a printable character must be right after the position*/
    return true;
}
