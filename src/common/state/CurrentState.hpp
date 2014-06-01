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
#ifndef _TIBEE_COMMON_CURRENTSTATE_HPP
#define _TIBEE_COMMON_CURRENTSTATE_HPP

#include <memory>
#include <cstdint>
#include <boost/utility.hpp>

#include <common/state/AbstractStateValue.hpp>
#include <common/BasicTypes.hpp>

namespace tibee
{
namespace common
{

class StateHistorySink;

/**
 * Current state (during a state history construction); façade of a
 * StateHistorySink for state providers.
 *
 * An object of this class is the main one any state provider have
 * access to. They cannot open or close a state history, but are able
 * to set state values of different paths, triggering interval
 * creation/recording behind the scenes.
 *
 * @author Philippe Proulx
 */
class CurrentState final :
    boost::noncopyable
{
    friend class StateHistorySink;

public:
    /**
     * Returns a quark for a given path string.
     *
     * The quark will always be the same for the same path.
     *
     * @param path Path string for which to get the quark
     * @returns    Quark for given path
     */
    quark_t getPathQuark(const char* path) const;

    /**
     * Returns a quark for a given path string.
     *
     * The quark will always be the same for the same path.
     *
     * @param path Path string for which to get the quark
     * @returns    Quark for given path
     */
    quark_t getPathQuark(const std::string& path) const;

    /**
     * Returns a quark for a given string state value.
     *
     * The quark will always be the same for the same string.
     *
     * @param path String for which to get the quark
     * @returns    Quark for given path
     */
    quark_t getStringValueQuark(const char* path) const;

    /**
     * Returns a quark for a given string state value.
     *
     * The quark will always be the same for the same string.
     *
     * @param path String for which to get the quark
     * @returns    Quark for given path
     */
    quark_t getStringValueQuark(const std::string& path) const;

    /**
     * Sets a 32-bit signed integer state value \p value for a specific
     * path \p pathQuark.
     *
     * Caller must make sure the path quark exists.
     *
     * See getPathQuark() to obtain a quark out of a raw path string.
     *
     * @param pathQuark Quark of state value path
     * @param value     Value to set
     */
    void setInt32State(quark_t pathQuark, std::int32_t value);

    /**
     * Sets a 32-bit unsigned integer state value \p value for a specific
     * path \p pathQuark.
     *
     * Caller must make sure the path quark exists.
     *
     * See getPathQuark() to obtain a quark out of a raw path string.
     *
     * @param pathQuark Quark of state value path
     * @param value     Value to set
     */
    void setUint32State(quark_t pathQuark, std::uint32_t value);

    /**
     * Sets a 64-bit signed integer state value \p value for a specific
     * path \p pathQuark.
     *
     * Caller must make sure the path quark exists.
     *
     * See getPathQuark() to obtain a quark out of a raw path string.
     *
     * @param pathQuark Quark of state value path
     * @param value     Value to set
     */
    void setInt64State(quark_t pathQuark, std::int64_t value);

    /**
     * Sets a 64-bit unsigned integer state value \p value for a specific
     * path \p pathQuark.
     *
     * Caller must make sure the path quark exists.
     *
     * See getPathQuark() to obtain a quark out of a raw path string.
     *
     * @param pathQuark Quark of state value path
     * @param value     Value to set
     */
    void setUint64State(quark_t pathQuark, std::uint64_t value);

    /**
     * Sets a 32-bit floating point number state value \p value for a
     * specific path \p pathQuark.
     *
     * Caller must make sure the path quark exists.
     *
     * See getPathQuark() to obtain a quark out of a raw path string.
     *
     * @param pathQuark Quark of state value path
     * @param value     Value to set
     */
    void setFloat32State(quark_t pathQuark, float value);

    /**
     * Sets a quark state value \p value for a specific path \p pathQuark.
     *
     * Caller must make sure both quarks exist.
     *
     * See getStringValueQuark() to obtain a quark out of a raw string.
     *
     * See getPathQuark() to obtain a quark out of a raw path string.
     *
     * @param pathQuark Quark of state value path
     * @param value     Value to set
     */
    void setQuarkState(quark_t pathQuark, quark_t value);

    /**
     * Convenience method to increment an integer state value.
     *
     * A type check is done here, and this method returns \a false if
     * the state value at \p pathQuark is not an integer state value.
     *
     * @param pathQuark Path quark of state value to increment
     * @param value     Value to use for incrementation
     * @returns         True if the incrementation was done
     */
    bool incState(quark_t pathQuark, std::int64_t value = 1);

    /**
     * Convenience method to decrement an integer state value.
     *
     * A type check is done here, and this method returns \a false if
     * the state value at \p pathQuark is not an integer state value.
     *
     * @param pathQuark Path quark of state value to decrement
     * @param value     Value (positive) to use for decrementation
     * @returns         True if the decrementation was done
     */
    bool decState(quark_t pathQuark, std::int64_t value = 1);

    /**
     * Removes a state value.
     *
     * Caller must make sure the path quark exists.
     *
     * @param pathQuark Path quark of state value to remove
     */
    void removeState(quark_t pathQuark);

    /**
     * Sets a state value \p value for a specific path \p pathQuark.
     *
     * Caller must make sure the path quark exists.
     *
     * See getPathQuark() to obtain a quark out of a raw path string.
     *
     * @param pathQuark Quark of state value path
     * @param value     Value to set
     */
    void setState(quark_t pathQuark, AbstractStateValue::UP value);

    /**
     * Returns the current state value for a given path.
     *
     * The returned pointer remains valid as long as no state is set
     * in this current state.
     *
     * @param pathQuark Quark of state value path
     * @returns         State value or \a nullptr if not found
     */
    const AbstractStateValue* getState(quark_t pathQuark) const;

private:
    // only StateHistorySink may build a CurrentState object
    CurrentState(StateHistorySink* sink);

private:
    StateHistorySink* _sink;
};

}
}

#endif // _TIBEE_COMMON_CURRENTSTATE_HPP
