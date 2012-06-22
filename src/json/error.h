/*
 * Copyright 2012 Achille Roussel.
 *
 * This file is part of Libjson++.
 *
 * Libjson++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Libjson++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Libjson++.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSON_ERROR_H
#define JSON_ERROR_H

#include <stdexcept>
#include "json/def.h"

namespace json
{

  class error : public std::runtime_error
  {

  public:

    error();

    error(const std::string &message);

    error(std::string &&message);

    error(const error &e);

    error(error &&e);

    ~error() throw ();

    error &operator=(const error &e);

    error &operator=(error &&e);

    void swap(error &e);

  };

}

namespace std
{

  inline void swap(json::error &e1, json::error &e2)
  {
    e1.swap(e2);
  }

}

#endif // JSON_ERROR_H
