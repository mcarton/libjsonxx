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

#ifndef JSON_MODEL_H
#define JSON_MODEL_H

#include <ostream>
#include <istream>
#include "json/def.h"
#include "json/object.h"

namespace json
{

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(int &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoi(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(unsigned int &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoi(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(long &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoi(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(unsigned long &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stol(obj);
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  inline void operator>>(const T &field, basic_object<Char, Traits, Allocator> &obj)
  {
    obj = field;
  }
  
  template < typename Class,
             typename Char = char,
             typename Traits = std::char_traits<char>,
             typename Allocator = std::allocator<char> >
  class basic_field
  {

  public:

    typedef basic_char_sequence<Char, Traits>           string_type;
    typedef basic_object<Char, Traits, Allocator>       object_type;
    typedef Class                                       class_type;

    string_type name;

    basic_field(const string_type &name):
      name(std::move(name))
    {
    }

    basic_field(const basic_field &f):
      name(f.name)
    {
    }

    basic_field(basic_field &&f):
      name(std::move(f.name))
    {
    }

    virtual ~basic_field()
    {
    }

    basic_field &operator=(const basic_field &f)
    {
      basic_field(f).swap(*this);
      return *this;
    }

    basic_field &operator=(basic_field &&f)
    {
      f.swap(*this);
      return *this;
    }

    void swap(basic_field &f)
    {
      std::swap(name, f.name);
    }

    virtual void load(class_type &instance, const object_type &object) const = 0;

    virtual void dump(const class_type &instance, object_type &object) const = 0;

  };

  template < typename Class,
             typename Field,
             typename Char = char,
             typename Traits = std::char_traits<Char>,
             typename Allocator = std::allocator<Char> >
  class class_field : public basic_field<Class, Char, Traits, Allocator>
  {

    typedef basic_field<Class, Char, Traits, Allocator> super;

  public:

    typedef typename super::string_type string_type;
    typedef typename super::object_type object_type;
    typedef typename super::class_type  class_type;
    typedef Field                       value_type;

    value_type value;

    class_field(const string_type &name, const value_type &value):
      super(name),
      value(value)
    {
    }

    class_field(const class_field &f):
      super(f),
      value(f.value)
    {
    }

    class_field(class_field &&f):
      super(std::move(f)),
      value(std::move(f.value))
    {
    }

    class_field &operator=(const class_field &f)
    {
      field(f).swap(*this);
      return *this;
    }

    class_field &operator=(class_field &&f)
    {
      f.swap(*this);
      return *this;
    }

    void swap(class_field &f)
    {
      super::swap(f);
      std::swap(value, f.value);
    }

    void load(class_type &instance, const object_type &object) const
    {
      instance.*value << object;
    }

    void dump(const class_type &instance, object_type &object) const
    {
      instance.*value >> object[this->name];
    }

  };

  template < typename Class, typename Field >
  inline class_field<Class, Field Class::*> field(const char *name, Field Class::*member)
  {
    const char_sequence s ( name, std::strlen(name) );
    return class_field<Class, Field Class::*>(s, member);
  }

  template < typename Function >
  inline void static_for_each(const Function &)
  {
  }

  template < typename Function, typename T, typename... Args >
  inline void static_for_each(const Function &function, T &&arg0, Args&&... args)
  {
    function(std::forward<T>(arg0));
    static_for_each(function, std::forward<Args>(args)...);
  }

  template < typename Class,
             typename Char = char,
             typename Traits = std::char_traits<Char>,
             typename Allocator = std::allocator<Char> >
  class model
  {

    class initializer
    {

    public:

      initializer(model &m):
        _model(m)
      {
      }

      template < typename Field >
      void operator()(const Field &f) const
      {
        _model.add_field(f);
      }

    private:
      model & _model;

    };

  public:

    typedef Class                                       class_type;
    typedef Char                                        char_type;
    typedef Traits                                      traits_type;
    typedef Allocator                                   allocator_type;
    typedef basic_field<Class, Char, Traits, Allocator> field_type;
    typedef basic_object<Char, Traits, Allocator>       object_type;

  private:

    typedef hash_map<field_type*, Char, Traits, Allocator> field_map_type;

  public:

    template < typename... Fields >
    model(Fields&&... fields):
      _field_map()
    {
      static_for_each(initializer(*this), std::forward<Fields>(fields)...);
    }

    ~model()
    {
    }

    void load(class_type &instance, const object_type &object) const
    {
      if (object.type() == type_map)
        {
          auto it = object.begin();
          auto jt = object.end();
          while (it != jt)
            {
              auto field = _field_map.find(it->first);
              if (field != _field_map.end())
                {
                  field->second->load(instance, it->second);
                }
              ++it;
            }
        }

      // Doesn't know how to deal with JSON object types other than 'map'.
      else
        {
          // TODO
        }
    }

    void dump(const class_type &instance, object_type &obj) const
    {
      obj.make_map();
      auto it = _field_map.begin();
      auto jt = _field_map.end();
      while (it != jt)
        {
          it->second->dump(instance, obj);
          ++it;
        }
    }

    template < typename Field >
    void add_field(const Field &f)
    {
      auto x = create_field(f);
      try
        {
          x->name = _field_map.emplace(f.name, x).first;
        }
      catch (...)
        {
          destroy_field(x);
          throw;
        }
    }

    allocator_type get_allocator() const
    {
      return _field_map.get_allocator();
    }

  private:
    field_map_type _field_map;

    template < typename Field >
    field_type *create_field(const Field &f)
    {
      static const basic_char_sequence<Char, Traits> zero;
      return new Field(zero, f.value);
    }

    void destroy_field(field_type *f)
    {
      delete f;
    }

  };

  template < typename Class, typename Char, typename Traits, typename Allocator >
  class model_istream
  {

  public:

    typedef typename std::basic_istream<Char, Traits>                   istream_type;
    typedef typename json::model<Class, Char, Traits, Allocator>        model_type;

    istream_type &istream;

    const model_type &model;

    model_istream(istream_type &i, const model_type &m):
      istream(i),
      model(m)
    {
    }

    istream_type &operator()(Class &instance) const
    {
      basic_object<Char, Traits, Allocator> obj ( model.get_allocator() );
      istream >> obj;
      model.load(instance, obj);
      return istream;
    }

  };

  template < typename Class, typename Char, typename Traits, typename Allocator >
  class model_ostream
  {

  public:

    typedef typename std::basic_ostream<Char, Traits>                   ostream_type;
    typedef typename json::model<Class, Char, Traits, Allocator>        model_type;

    ostream_type &ostream;

    const model_type &model;

    model_ostream(ostream_type &o, const model_type &m):
      ostream(o),
      model(m)
    {
    }

    ostream_type &operator()(const Class &instance) const
    {
      basic_object<Char, Traits, Allocator> obj ( model.get_allocator() );
      model.dump(instance, obj);
      return ostream << obj;
    }

  };

}

namespace std
{

  template < typename Class, typename Char, typename Traits, typename Allocator >
  inline json::model_istream<Class, Char, Traits, Allocator>
  operator>>(basic_istream<Char, Traits> &in,
             const json::model<Class, Char, Traits, Allocator> &model)
  {
    return json::model_istream<Class, Char, Traits, Allocator>(in, model);
  }

  template < typename Class, typename Char, typename Traits, typename Allocator >
  inline basic_istream<Char, Traits> &
  operator>>(const json::model_istream<Class, Char, Traits, Allocator> &in,
             Class &instance)
  {
    return in(instance);
  }

  template < typename Class, typename Char, typename Traits, typename Allocator >
  inline json::model_ostream<Class, Char, Traits, Allocator>
  operator<<(basic_ostream<Char, Traits> &out,
             const json::model<Class, Char, Traits, Allocator> &model)
  {
    return json::model_ostream<Class, Char, Traits, Allocator>(out, model);
  }

  template < typename Class, typename Char, typename Traits, typename Allocator >
  inline basic_ostream<Char, Traits> &
  operator<<(const json::model_ostream<Class, Char, Traits, Allocator> &out,
             const Class &instance)
  {
    return out(instance);
  }

}

#endif // JSON_MODEL_H
