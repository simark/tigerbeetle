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
#ifndef _ARGUMENTS_HPP
#define _ARGUMENTS_HPP

#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>

namespace tibee
{

/**
 * Program arguments.
 *
 * @author Philippe Proulx
 */
struct Arguments
{
    std::vector<boost::filesystem::path> traces;
    std::vector<boost::filesystem::path> stateProviders;
    std::string bindProgress;
    boost::filesystem::path cacheDir;
    bool verbose;
    bool force;
};

}

#endif // _ARGUMENTS_HPP
