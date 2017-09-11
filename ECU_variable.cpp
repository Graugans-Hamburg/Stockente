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

#include "ECU_variable.h"
#include <cstring>

ECU_variable::ECU_variable()
{
    //ctor
    Address = 0;
    Datatype = type_unknown;
    Min_Value_Valid = false;
    Max_Value_Valid = false;
}

ECU_variable::~ECU_variable()
{
    //dtor
}

void ECU_variable::ParseDatatyp(const char * Cstr)
{
    if(strcmp(Cstr,"uint8") == 0)
    {
        SetDataType(type_u8);
        return;
    }
    if(strcmp(Cstr,"int8") == 0)
    {
        SetDataType(type_i8);
        return;
    }
    if(strcmp(Cstr,"uint16") == 0)
    {
        SetDataType(type_u16);
        return;
    }
    if(strcmp(Cstr,"int16") == 0)
    {
        SetDataType(type_i16);
        return;
    }
    if(strcmp(Cstr,"uint32") == 0)
    {
        SetDataType(type_u32);
        return;
    }
    if(strcmp(Cstr,"int32") == 0)
    {
        SetDataType(type_i32);
        return;
    }

    if(strcmp(Cstr,"float") == 0)
    {
        SetDataType(type_f32);
        return;
    }
    if(strcmp(Cstr,"float") == 0)
    {
        SetDataType(type_f64);
        return;
    }

    // defualt
    SetDataType(type_unknown);
}

std::string ECU_variable::GetDatatypAsString(void)
{
    std::string typeAsString;
    switch(Datatype)
    {
        case type_f32:
            typeAsString = "float";
            break;
        case type_u8:
            typeAsString = "uint8_t";
            break;
        case type_i8:
            typeAsString = "int8_t";
            break;
        case type_u16:
            typeAsString = "uint16_t";
            break;
        case type_i16:
            typeAsString = "int16_t";
            break;
        case type_u32:
            typeAsString = "uint32_t";
            break;
        case type_i32:
            typeAsString = "int32_t";
            break;
        default:
            typeAsString = "unkown";
            break;
    }
    return typeAsString;
}
