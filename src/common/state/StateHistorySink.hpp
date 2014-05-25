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
#ifndef _STATEHISTORYSINK_HPP
#define _STATEHISTORYSINK_HPP

#include <memory>
#include <cstdint>
#include <array>
#include <functional>
#include <map>
#include <boost/utility.hpp>
#include <boost/filesystem/path.hpp>
#include <delorean/HistoryFileSink.hpp>
#include <delorean/interval/AbstractInterval.hpp>

#include <common/BasicTypes.hpp>
#include <common/state/AbstractStateValue.hpp>
#include <common/state/CurrentState.hpp>

namespace tibee
{
namespace common
{

/**
 * A state history sink.
 *
 * An object of this class must be used to write a state history on
 * disk. A state history comprises a few files: two string databases
 * (one for paths and the other for state values) and a history of
 * state intervals.
 *
 * @author Philippe Proulx
 */
class StateHistorySink :
    boost::noncopyable
{
public:
    /**
     * Builds a state history sink.
     *
     * The current history timestamp is initialized with 0.
     *
     * @param pathStrDbPath  Path to path string database file (to be created)
     * @param valueStrDbPath Path to value string database file (to be created)
     * @param historyPath    Path to history file (to be created)
     */
    StateHistorySink(const boost::filesystem::path& pathStrDbPath,
                     const boost::filesystem::path& valueStrDbPath,
                     const boost::filesystem::path& historyPath);

    ~StateHistorySink();

    /**
     * Sets the history current timestamp. Timestamps should be set
     * in ascending order.
     *
     * @param ts Current timestamp
     */
    void setCurrentTimestamp(timestamp_t ts)
    {
        _ts = ts;
    }

    /**
     * Returns the current history timestamp.
     *
     * @returns Current history timestamp
     */
    timestamp_t getCurrentTimestamp() const
    {
        return _ts;
    }

    /**
     * Closes this state history sink, effectively closing all opened
     * files and marking it as closed.
     *
     * All opened state values are closed with the current history
     * timestamp.
     *
     * The string databases are written here.
     */
    void close();

    /**
     * Returns a quark for a given path string.
     *
     * The quark will always be the same for the same path.
     *
     * @param path Path string for which to get the quark
     * @returns    Quark for given path
     */
    quark_t getPathQuark(const std::string& path);

    /**
     * Returns a quark for a given string state value.
     *
     * The quark will always be the same for the same string.
     *
     * @param value String for which to get the quark
     * @returns     Quark for given value
     */
    quark_t getStringValueQuark(const std::string& value);

    /**
     * Sets a state value.
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
     * Removes a state value.
     *
     * Caller must make sure the path quark exists.
     *
     * @param pathQuark Path quark of state value to remove
     */
    void removeState(quark_t pathQuark);

    /**
     * Returns the current state value for a given path.
     *
     * The returned pointer remains valid as long as no state is set
     * in this sink.
     *
     * @param pathQuark Quark of state value path
     * @returns         State value or \a nullptr if not found
     */
    const AbstractStateValue* getState(quark_t pathQuark) const
    {
        auto it = _stateValues.find(pathQuark);

        if (it == _stateValues.end()) {
            return nullptr;
        }

        return it->second.value.get();
    }

    /**
     * Returns a reference to the current state, which may be used by
     * state providers as a façade of this sink without having access
     * to unrelated methods.
     *
     * The current state remains valid/usable as long as this sink is.
     *
     * @returns Current state reference
     */
    CurrentState& getCurrentState()
    {
        return _currentState;
    }

private:
    // a string database
    typedef std::map<std::string, quark_t> StringDb;

    // this is used to keep the begin timestamp with a state value
    struct StateValueEntry
    {
        timestamp_t beginTs;
        AbstractStateValue::UP value;
    };

    // a (state value -> interval) translator
    typedef std::function<delo::AbstractInterval* (quark_t, const StateValueEntry&)> Translator;

private:
    void initTranslators();
    void open();
    void writeInterval(quark_t pathQuark,
                       const StateValueEntry& stateValueEntry);
    void writeStringDb(const StringDb& stringDb,
                       const boost::filesystem::path& path);
    quark_t getQuark(StringDb& stringDb, const std::string& value);

private:
    // paths to files to create
    boost::filesystem::path _pathStrDbPath;
    boost::filesystem::path _valueStrDbPath;
    boost::filesystem::path _historyPath;

    // current timestamp
    timestamp_t _ts;

    // open state
    bool _opened;

    // string database for state paths
    StringDb _pathsDb;

    // string database for state values
    StringDb _strValuesDb;

    // current state path quark
    quark_t _curPathQuark;

    // current state value quark
    quark_t _curStrValueQuark;

    // current state values
    std::map<quark_t, StateValueEntry> _stateValues;

    // (state value -> interval) translators
    std::array<Translator, 16> _translators;

    // current state façade for state providers
    CurrentState _currentState;

    // interval history sink
    std::unique_ptr<delo::HistoryFileSink> _intervalFileSink;
};

}
}

#endif // _STATEHISTORYSINK_HPP
