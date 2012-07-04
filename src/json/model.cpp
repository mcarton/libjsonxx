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
#include "json/model.h"

namespace json
{

  void error_loading_boolean()
  {
    throw error("json::model<?>: trying to load a boolean from a non-boolean JSON object");
  }

  void error_loading_list()
  {
    throw error("json::model<?>: trying to load a list from a non-list JSON object");
  }

  void error_loading_map()
  {
    throw error("json::model<?>: trying to load a map from a non-map JSON object");
  }

  void error_loading_object()
  {
    throw error("json::model<?>: trying to load an object from a JSON object which is not a dictionnary");
  }

}
