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

#include <sstream>
#include "json/def.h"
#include "json/object.h"

namespace std
{

  // Forward declaration of STL classes, then we don't need to include the
  // entire standard library in this file.
  template < typename, typename > class basic_istream;
  template < typename, typename > class basic_ostream;
  template < typename, typename, typename > class basic_string;
  template < typename, typename > class vector;
  template < typename, typename > class deque;
  template < typename, typename > class list;
  template < typename, typename, typename, typename > class map;
  template < typename, typename, typename, typename, typename > class unordered_map;

}

namespace json
{

  void error_loading_boolean();

  void error_loading_list();

  void error_loading_map();

  void error_loading_object();

  // Overloading of the '<<' operator to provide a way to transform a JSON
  // object into C++ types.

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(short &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoi(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(unsigned short &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoul(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(int &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoi(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(unsigned int &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoul(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(long &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stol(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(unsigned long &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stoul(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(float &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stof(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(double &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stod(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(long double &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    field = std::stold(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline void operator<<(bool &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    if (is_true(obj))
      {
        field = true;
      }
    else if (is_false(obj))
      {
        field = false;
      }
    else
      {
        error_loading_boolean();
      }
  }

  template < typename Char, typename Traits, typename Allocator1, typename Allocator2 >
  inline void operator<<(std::basic_string<Char, Traits, Allocator1> &field,
                         const basic_object<Char, Traits, Allocator2> &obj)
  {
    field = obj;
  }

  template < typename Char, typename Traits, typename Allocator1, typename Allocator2 >
  inline void operator<<(basic_object<Char, Traits, Allocator1> &field,
                         const basic_object<Char, Traits, Allocator2> &obj)
  {
    field = obj;
  }

  template < typename List, typename Char, typename Traits, typename Allocator2 >
  inline void load_list(List &field, const basic_object<Char, Traits, Allocator2> &obj)
  {
    if (obj.type() == type_list)
      {
        auto it = obj.begin();
        auto jt = obj.end();
        while (it != jt)
          {
            const basic_object<Char, Traits, Allocator2> &o = *it;
            field.emplace_back();
            field.back() << o;
            ++it;
          }
      }
    else
      {
        error_loading_list();
      }
  }

  template < typename T, typename Allocator1, typename Char, typename Traits, typename Allocator2 >
  inline void operator<<(std::vector<T, Allocator1> &field,
                         const basic_object<Char, Traits, Allocator2> &obj)
  {
    load_list(field, obj);
  }

  template < typename T, typename Allocator1, typename Char, typename Traits, typename Allocator2 >
  inline void operator<<(std::deque<T, Allocator1> &field,
                         const basic_object<Char, Traits, Allocator2> &obj)
  {
    load_list(field, obj);
  }

  template < typename T, typename Allocator1, typename Char, typename Traits, typename Allocator2 >
  inline void operator<<(std::list<T, Allocator1> &field,
                         const basic_object<Char, Traits, Allocator2> &obj)
  {
    load_list(field, obj);
  }

  template < typename Map, typename Char, typename Traits, typename Allocator >
  inline void load_map(Map &field, const basic_object<Char, Traits, Allocator> &obj)
  {
    if (obj.type() == type_map)
      {
        // TODO: see dump_map.
        std::basic_stringstream<Char, Traits, typename Map::allocator_type> buffer;

        auto it = obj.begin();
        auto jt = obj.end();
        while (it != jt)
          {
            // TODO: I don't think that's very efficient but it does the job and
            // integrates well with the STL...
            // We may want to improve that sometimes.
            typename Map::key_type key;
            buffer << it->first;
            buffer >> key;
            field[key] << it->second;
            ++it;
          }
      }
    else
      {
        error_loading_map();
      }
  }

  template < typename Key,
             typename Value,
             typename Compare,
             typename Allocator1,
             typename Char,
             typename Traits,
             typename Allocator2 >
  inline void operator<<(std::map<Key, Value, Compare, Allocator1> &field,
                         const basic_object<Char, Traits, Allocator2> &obj)
  {
    load_map(field, obj);
  }

  template < typename Key,
             typename Value,
             typename Hash,
             typename Equal,
             typename Allocator1,
             typename Char,
             typename Traits,
             typename Allocator2 >
  inline void operator<<(std::unordered_map<Key, Value, Hash, Equal, Allocator1> &field,
                         const basic_object<Char, Traits, Allocator2> &obj)
  {
    load_map(field, obj);
  }

  // Overloading of the '>>' operator to provide a way to build JSON objects
  // from C++ types.

  template < typename T, typename Char, typename Traits, typename Allocator >
  inline void operator>>(const T &field, basic_object<Char, Traits, Allocator> &obj)
  {
    obj = field;
  }

  template < typename List, typename Char, typename Traits, typename Allocator2 >
  inline void dump_list(const List &field, basic_object<Char, Traits, Allocator2> &obj)
  {
    std::size_t i = 0;
    obj.make_list();
    auto it = field.begin();
    auto jt = field.end();
    while (it != jt)
      {
        *it >> obj[i];
        ++it;
        ++i;
      }
  }

  template < typename T, typename Allocator1, typename Char, typename Traits, typename Allocator2 >
  inline void operator>>(const std::vector<T, Allocator1> &field,
                         basic_object<Char, Traits, Allocator2> &obj)
  {
    dump_list(field, obj);
  }

  template < typename T, typename Allocator1, typename Char, typename Traits, typename Allocator2 >
  inline void operator>>(const std::deque<T, Allocator1> &field,
                         basic_object<Char, Traits, Allocator2> &obj)
  {
    dump_list(field, obj);
  }

  template < typename T, typename Allocator1, typename Char, typename Traits, typename Allocator2 >
  inline void operator>>(const std::list<T, Allocator1> &field,
                         basic_object<Char, Traits, Allocator2> &obj)
  {
    dump_list(field, obj);
  }

  template < typename Map, typename Char, typename Traits, typename Allocator >
  inline void dump_map(const Map &field, basic_object<Char, Traits, Allocator> &obj)
  {
    // TODO: implement a string stream object that allows to specify the allocator
    // when building the object.
    // For now the STL is kind of broken and doesn't let us pass an allocator to
    // the constructor which may be a problem if some custom allocators rely on an
    // underlying state which they should acquire when copied.
    std::basic_stringstream<Char, Traits, typename Map::allocator_type> buffer;

    obj.make_map();
    auto it = field.begin();
    auto jt = field.end();
    while (it != jt)
      {
        buffer << it->first;
        it->second >> obj[buffer.str()];
        ++it;
      }
  }

  template < typename Key,
             typename Value,
             typename Compare,
             typename Allocator1,
             typename Char,
             typename Traits,
             typename Allocator2 >
  inline void operator>>(const std::map<Key, Value, Compare, Allocator1> &field,
                         basic_object<Char, Traits, Allocator2> &obj)
  {
    dump_map(field, obj);
  }

  template < typename Key,
             typename Value,
             typename Hash,
             typename Equal,
             typename Allocator1,
             typename Char,
             typename Traits,
             typename Allocator2 >
  inline void operator>>(const std::unordered_map<Key, Value, Hash, Equal, Allocator1> &field,
                         basic_object<Char, Traits, Allocator2> &obj)
  {
    dump_map(field, obj);
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

  /**
   * @brief Creates a mapping between JSON names and C++ class fields.
   *
   * @param name The name of a JSON field that will be mapped to a class member.
   * @param member A pointer to class memory to be mapped to the given JSON name.
   *
   * @return The function returns an object that can be used in a call to
   * <em>json::make_model</em>
   *
   * @see json::make_model
   */
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

  /**
   * @brief A model contains information on how to convert a JSON object to a
   * C++ type and vice-versa.
   *
   * Models are useful to automate serialization and deserialization of JSON
   * objects.
   * <br/>
   * Instead of writing the code to manually parse and format JSON from / to
   * strings or streams we can create a JSON model and teach it how to build
   * a C++ object.
   *
   * @see json::make_model
   */
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
      void operator()(const class_field<Class, Field> &f) const
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

    model(const model &m):
      _field_map(m._field_map)
    {
    }

    model(model &&m):
      _field_map(std::move(m._field_map))
    {
    }

    ~model()
    {
      auto it = _field_map.begin();
      auto jt = _field_map.end();
      while (it != jt)
        {
          destroy_field(it->second);
          ++it;
        }
    }

    model &operator=(const model &m)
    {
      model(m).swap(*this);
      return *this;
    }

    model &operator=(model &&m)
    {
      m.swap(*this);
      return *this;
    }

    void swap(model &m)
    {
      std::swap(_field_map, m._field_map);
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
      else
        {
          error_loading_object();
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
    void add_field(const class_field<Class, Field> &f)
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

  /**
   * @brief Creates a <em>json::model</em> out of a list of fields.
   *
   * @param field0 An object obtained by a call to the <em>json::field</em>
   * function which represents a mapping between a JSON name and the field of
   * a C++ class.
   * @param fields More fields (optional), they all should be the result to a
   * call to the <em>json::field</em> function.
   *
   * @return The function returns an instance of <em>json::model</em> templated
   * on the class type it contains pointer to fields of.
   *
   * @see json::field
   * @see json::model
   */
  template < typename Class,
             typename Field,
             typename... Fields >
  inline model<Class> make_model(const class_field<Class, Field> &field0, Fields&&... fields)
  {
    return model<Class>(field0, std::forward<Fields>(fields)...);
  }

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
  inline void swap(json::model<Class, Char, Traits, Allocator> &m1,
                   json::model<Class, Char, Traits, Allocator> &m2)
  {
    m1.swap(m2);
  }

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
