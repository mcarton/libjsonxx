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

#include "json/error.h"

namespace json
{

  error::error():
    std::runtime_error("")
  {
  }

  error::error(const std::string &message):
    std::runtime_error(message)
  {
  }

  error::error(std::string &&message):
    std::runtime_error(std::move(message))
  {
  }

  error::error(const error &e):
    std::runtime_error(e)
  {
  }

  error::error(error &&e):
    std::runtime_error(std::move(e))
  {
  }

  error::~error() throw ()
  {
  }

  error &error::operator=(const error &e)
  {
    std::runtime_error::operator=(e);
    return *this;
  }

  error &error::operator=(error &&e)
  {
    std::runtime_error::operator=(std::move(e));
    return *this;
  }

  void error::swap(error &e)
  {
    std::swap(static_cast<std::runtime_error&>(*this), static_cast<std::runtime_error&>(e));
  }

}

