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

#ifndef JSON_HASH_SLOT_H
#define JSON_HASH_SLOT_H

#include <utility>
#include "json/def.h"

namespace json
{

  template < typename T >
  class hash_slot
  {

  public:

    typedef T		value_type;
    typedef T &		reference;
    typedef T *		pointer;
    typedef const T &	const_reference;
    typedef const T *	const_pointer;

    enum status_type
      {
	free,
	busy,
	erased,
      };

    status_type status;

    hash_slot();

    hash_slot(value_type &&value);

    hash_slot(const_reference value);

    hash_slot(hash_slot &&slot);

    hash_slot(const hash_slot &slot);

    ~hash_slot();

    hash_slot &operator=(value_type &&value);

    hash_slot &operator=(const_reference value);

    hash_slot &operator=(hash_slot &&slot);

    hash_slot &operator=(const hash_slot &slot);

    void swap(hash_slot &slot);

    void clear();

    void erase();

    reference value();

    const_reference value() const;

  private:
    unsigned char _memory[sizeof(value_type)];

    template < typename... Args >
    void construct(Args&&... args)
    {
      new (_memory) value_type (std::forward<Args>(args)...);
    }

    void destroy();
    void swap_body_if_busy(hash_slot &);
    void swap_body(hash_slot &);
    void swap_move_into(hash_slot &);
    void swap_bodies(hash_slot &);
    void swap_status(hash_slot &);

  };

}

namespace std
{

  template < typename T >
  inline void swap(json::hash_slot<T> &slot1, json::hash_slot<T> &slot2)
  {
    slot1.swap(slot2);
  }

}

#endif // JSON_HASH_SLOT_H
