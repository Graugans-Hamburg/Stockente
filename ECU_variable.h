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

#ifndef ECU_VARIABLE_H
#define ECU_VARIABLE_H
#include <string>
#include "type_definition.h"
#include <stdint.h>


class ECU_variable
{
    public:
        ECU_variable();
        virtual ~ECU_variable();
        uint32_t GetAddress() { return Address; }
        void SetAddress(uint32_t val) { Address = val; }
        EnumDataType GetDataType() { return Datatype; }
        void SetDataType(EnumDataType val) { Datatype = val; }
        std::string GetName() { return Name; }
        void SetName(std::string val) { Name = val; }
        std::string GetUnit() { return str_Unit; }
        void SetUnit(std::string val) { str_Unit = val; }
        std::string GetDescription() { return str_Description; }
        void SetDescription(std::string val) { str_Description = val; }
        void ParseDatatyp(const char *);
        std::string GetDatatypAsString(void);
        bool MinValueValid(void) { return Min_Value_Valid; };
        bool MaxValueValid(void) { return Max_Value_Valid; };
        void SetMinValue_Fl(float val) { Min_Value_Float = val; }
        float GetMinValue_F() { return Min_Value_Float; }
        void SetMaxValue_Fl(float val) { Max_Value_Float = val; }
        float GetMaxValue_F() { return Max_Value_Float; }
        void SetMaxValue_Int(float val) { Max_Value_Int = val; }
        float GetMaxValue_Int() { return Max_Value_Int; }
        void SetMinValue_Int(int64_t val) { Max_Value_Int = val; }
        int64_t GetMinValue_Int() { return Max_Value_Int; }
        // only available in this code
        std::string str_MinValue;
        std::string str_MaxValue;
        std::string str_Description;
        std::string str_Unit;

    protected:
    private:
        uint32_t Address;
        EnumDataType Datatype;
        float Min_Value_Float;
        float Max_Value_Float;
        int64_t Min_Value_Int;
        int64_t Max_Value_Int;
        bool Max_Value_Valid;
        bool Min_Value_Valid;
        std::string Name;


};

#endif // ECU_VARIABLE_H
