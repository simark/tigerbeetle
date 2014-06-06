/* Copyright (c) 2014 Philippe Proulx <eepp.ca>
 *
 * This file is part of tigerbeetle.
 *
 * tigerbeetle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tigerbeetle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tigerbeetle.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _TIBEE_COMMON_ABSTRACTINTEGEREVENTVALUE_HPP
#define _TIBEE_COMMON_ABSTRACTINTEGEREVENTVALUE_HPP

#include <cstdint>
#include <babeltrace/ctf/events.h>

#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/EventValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Abstract integer event value, with common operations for both
 * signed and unsigned integers.
 *
 * @author Philippe Proulx
 */
class AbstractIntegerEventValue :
    public AbstractEventValue
{
public:
    /**
     * Builds an abstract integer event value.
     *
     * @param def  BT field definition
     * @param type Concrete event value type
     */
    AbstractIntegerEventValue(const ::bt_definition* def,
                              EventValueType type) :
        AbstractEventValue {type},
        _btDef {def}
    {
    }

    /**
     * Returns the expected display base (radix) or -1 when not
     * available.
     *
     * @returns Expected display base or -1
     */
    int getDisplayBase() const
    {
        auto decl = ::bt_ctf_get_decl_from_def(_btDef);
        auto base = ::bt_ctf_get_int_base(decl);

        if (base < 0) {
            return -1;
        }

        return base;
    }

protected:
    const ::bt_definition* getDef() const {
        return _btDef;
    }

private:
    const ::bt_definition* _btDef;
};

}
}

#endif // _TIBEE_COMMON_ABSTRACTINTEGEREVENTVALUE_HPP
