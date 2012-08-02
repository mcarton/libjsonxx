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

#ifndef JSON_HASH_SLOT_HPP
#define JSON_HASH_SLOT_HPP

#include <utility>
#include "json/hash_slot.h"

namespace json
{

  template < typename T >
  hash_slot<T>::body::
  body():
    dummy(false)
  {
  }

  template < typename T >
  hash_slot<T>::body::
  ~body()
  {
  }

  template < typename T >
  void
  hash_slot<T>::body::construct(const value_type &x)
  {
    new (&object) value_type (x);
  }

  template < typename T >
  void
  hash_slot<T>::body::construct(value_type &&x)
  {
    new (&object) value_type (std::move(x));
  }

  template < typename T >
  void
  hash_slot<T>::body::destroy()
  {
    object.~value_type();
  }

  template < typename T >
  hash_slot<T>::hash_slot():
    status(free)
  {
  }

  template < typename T >
  hash_slot<T>::hash_slot(value_type &&value):
    status(busy)
  {
    construct(std::move(value));
  }

  template < typename T >
  hash_slot<T>::hash_slot(const_reference value):
    status(busy)
  {
    construct(value);
  }

  template < typename T >
  hash_slot<T>::hash_slot(hash_slot &&slot):
    status(free)
  {
    slot.swap(*this);
  }

  template < typename T >
  hash_slot<T>::hash_slot(const hash_slot &slot):
    status(slot.status)
  {
    construct(slot.value());
  }

  template < typename T >
  hash_slot<T>::~hash_slot()
  {
    if (status == busy)
      {
	destroy();
      }
  }

  template < typename T >
  hash_slot<T> &
  hash_slot<T>::operator=(value_type &&value)
  {
    hash_slot(std::move(value)).swap(*this);
    return *this;
  }

  template < typename T >
  hash_slot<T> &
  hash_slot<T>::operator=(const_reference value)
  {
    hash_slot(value).swap(*this);
    return *this;
  }

  template < typename T >
  hash_slot<T> &
  hash_slot<T>::operator=(hash_slot &&slot)
  {
    slot.swap(*this);
    return *this;
  }

  template < typename T >
  hash_slot<T> &
  hash_slot<T>::operator=(const hash_slot &slot)
  {
    hash_slot(slot).swap(*this);
    return *this;
  }

  template < typename T >
  void
  hash_slot<T>::swap(hash_slot &slot)
  {
    swap_body_if_busy(slot);
    swap_status(slot);
  }

  template < typename T >
  void
  hash_slot<T>::clear()
  {
    if (status == busy)
      {
	destroy();
      }
    status = free;
  }

  template < typename T >
  void
  hash_slot<T>::erase()
  {
    if (status == busy)
      {
	destroy();
	status = erased;
      }
  }

  template < typename T >
  typename hash_slot<T>::reference
  hash_slot<T>::value()
  {
    return _body.object;
  }

  template < typename T >
  typename hash_slot<T>::const_reference
  hash_slot<T>::value() const
  {
    return _body.object;
  }

  template < typename T >
  void
  hash_slot<T>::construct(const value_type &x)
  {
    _body.construct(x);
  }

  template < typename T >
  void
  hash_slot<T>::construct(value_type &&x)
  {
    _body.construct(std::move(x));
  }

  template < typename T >
  void
  hash_slot<T>::destroy()
  {
    _body.destroy();
  }

  template < typename T >
  void
  hash_slot<T>::swap_body_if_busy(hash_slot &slot)
  {
    if ((status == busy) || (slot.status == busy))
      {
	swap_body(slot);
      }
  }

  template < typename T >
  void
  hash_slot<T>::swap_body(hash_slot &slot)
  {
    if (status != busy)
      {
	return slot.swap_body(*this);
      }
    if (slot.status != busy)
      {
	swap_move_into(slot);
      }
    else
      {
	swap_bodies(slot);
      }
  }

  template < typename T >
  void
  hash_slot<T>::swap_move_into(hash_slot &slot)
  {
    slot.construct(std::move(value()));
    destroy();
  }

  template < typename T >
  void
  hash_slot<T>::swap_bodies(hash_slot &slot)
  {
    std::swap(value(), slot.value());
  }

  template < typename T >
  void
  hash_slot<T>::swap_status(hash_slot &slot)
  {
    std::swap(status, slot.status);
  }

}

#endif // JSON_HASH_SLOT_HPP
